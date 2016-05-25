// ---------------------------------------------------------------------------
// Created by Edwin Croissant
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "SMT172.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

#include <SMT172.h>

namespace {
volatile uint16_t sensorCycleCount;
volatile uint8_t status; // 0 = busy, 1 = success, 2 = not connected
volatile uint16_t overflowCount;
volatile uint32_t startCycle;
volatile uint32_t tempCycle;
volatile uint32_t endCycle;
volatile uint32_t highCycles;
volatile uint32_t minClockCycles;
volatile float interimSensorError;
}

// timer overflows every 65536 counts
ISR (TIMER1_OVF_vect) {
	overflowCount++;
}  // end of TIMER1_OVF_vect

//	Frequency is lower then 400 Hz. Disconnected sensor?
ISR (TIMER1_COMPA_vect) {
	TIMSK1 = 0;    // no more interrupts for now from timer 1
	TIFR1 = _BV(ICF1) | _BV(TOV1) | _BV(OCF1A); // clear pending interrupts as we are done
	status = 2;
}	// end of TIMER1_COMPA_vect

ISR (TIMER1_CAPT_vect) {
	union twoword {
		uint32_t stamp;
		struct {
			uint16_t counter;
			uint16_t overflow;
		};
	} cycle;

	//  See "Using the Input Capture Unit" in the Atmel documentation
	cycle.counter = ICR1;//	The ICR1 Register should be read as early in the interrupt handler routine as possible.
	TCCR1B ^= _BV(ICES1);//	Toggle Input Capture Edge Select directly after ICR1 is read
	TIFR1 |= _BV(ICF1);		//	Clear ICF1 directly after edge direction change

	//	The Input capture interrupt priority is higher then the overflow interrupt. So it is possible
	//	that the overflow interrupt is still pending to be processed. If an overflow is just missed then
	//	synchronize cycle.overflow with overflowCount, the latter will be processed later by the overflow interrupt
	cycle.overflow = overflowCount;
	if ((TIFR1 & _BV(TOV1)) && cycle.counter < 0x7FFF)
		cycle.overflow++;

	if (TCCR1B & _BV(ICES1)) // test if ICES1 is raising as SMT172 datasheet dictates starting with falling edge
	{						 // note that ICES1 is already toggled
		switch (sensorCycleCount) {
		case 0:
			startCycle = cycle.stamp;
			break;
		case 1:
			highCycles += cycle.stamp - tempCycle;
			minClockCycles = interimSensorError / (cycle.stamp - startCycle);
			break;
		default:
			highCycles += cycle.stamp - tempCycle;
			if (((sensorCycleCount) % 8 == 0) // measure a multiple of 8 sensor cycles
			&& ((cycle.stamp - startCycle) >= minClockCycles)) {
				endCycle = cycle.stamp;
				TIMSK1 = 0;    // no more interrupts for now from timer 1
				TIFR1 = _BV(ICF1) | _BV(TOV1) | _BV(OCF1A); // clear pending interrupts as we are done
				status = 1;
				return;
			};
		};
		sensorCycleCount++;
	} else {
		// this is executed at the raising edge
		OCR1A = cycle.counter + (F_CPU / 400); // advance the output compare register
		tempCycle = cycle.stamp;
	}
}  // end of TIMER1_CAPT_vect

void SMT172::startTemperature(float sensorError) {
	// initialize Timer1 to measure the duty cycle at the Input Capture Pin 1

	interimSensorError = 20000 / 3 / pow(sensorError,2);

	status = 0;  // measuring duty cycle
	sensorCycleCount = 0;
	highCycles = 0;
	overflowCount = 0;

	// reset timer 1
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;          // Timer/Counter to zero

	// Set up Timer 1
	OCR1A = (F_CPU / 400); // ~ 400 Hz minimum frequency
	TCCR1B = _BV(CS10) | _BV(ICES1); // start Timer 1, no prescaler and Input Capture Edge Select rising on D8
	TIFR1 = _BV(ICF1) | _BV(TOV1) | _BV(OCF1A); // clear pending interrupts so we don't get a bogus one
	TIMSK1 = _BV(ICIE1) | _BV(TOIE1)| _BV(OCIE1A); // and enable interrupt on Timer 1 input capture, overflow and compare A

}  // end of startTemperature

uint8_t SMT172::getStatus(void) {
	// return 0 when busy, 1 when success, 2 when not connected
	return status;
}

float SMT172::getTemperature(void) {
	// return the temperature from the previous startTemperature command
	// in degrees Celsius
	float dutyCycle = (float(highCycles) / float(endCycle - startCycle));
	return -1.43 * pow(dutyCycle, 2) + 214.56 * dutyCycle - 68.6;
}

float SMT172::getError(void) {
	// return the standard deviation of the sampling noise
	float sensorPeriod = getTime() / (sensorCycleCount);
	float clockCycleTime = 1 / float(F_CPU);
	return (200 * clockCycleTime) / sqrt(6 * getTime() * sensorPeriod);
}

float SMT172::getFrequency(void) {
	// return the frequency of the sensor
	return (sensorCycleCount) / getTime();
}

float SMT172::getDutyCycle(void) {
	// return the duty cycle of the sensor
	return (float(highCycles) / float(endCycle - startCycle));
}

float SMT172::getTime(void) {
	// return the measuring time in seconds
	return float(endCycle - startCycle) / float(F_CPU);
}
