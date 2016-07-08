/*
 Demo sketch for the SMT172 library. 
 This sketch is intended for the ATmega2660
 The ICP1 pin is not exposed on the Mega2560R3 board so timer4 or timer5 must be
 used.
 This sketch will output to serial.
 Connect the output of the SMT172 to pin 49 (Input Capture Pin of timer 4)
 Connect the output of the SMT172 to pin 48 (Input Capture Pin of timer 5)
 Timer 2 is set up in phase correct PWM and output a duty cycle of
 10.98% ~-45.06 C on pin 9 and a duty cycle of 92.94% ~139.58 C on pin 10
 Connect pin 48 or pin 49 to pin 9 or pin 10 to check the working if no SMT172 is available
*/

#include<arduino.h>
#include <SMT172_T4.h>
#include <SMT172_T5.h>
uint32_t LastSensorUpdate;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(115200);
	pinMode(48, INPUT);
	pinMode(49, INPUT);
	Serial.println(F("Demo sketch SMT172"));

//	The following code fragment sets up phase-correct PWM on pins 9 and 10 (Timer 2).
//	The waveform generation mode bits WGM are set to to 001 for phase-correct PWM.
//	The other bits are the same as for fast PWM.

	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
	// TCCR2B = _BV(CS22); // Output frequency: 16 MHz / 64 / 255 / 2 =  490.196 Hz
	TCCR2B = _BV(CS21); // Output frequency: 16 MHz /  8 / 255 / 2 = 3921.569 Hz
	OCR2A = 237; // Output A duty cycle: 237 / 255 = 92.94%	= 129.58 C	on pin 10
	OCR2B = 28;	 // Output B duty cycle:  28 / 255 = 10.98%	= -45.06 C	on pin 9


	SMT172_T4::startTemperature(0.002);
	SMT172_T5::startTemperature(0.002);
	delay(30);
}

// The loop function is called in an endless loop
void loop() {
	// read the sensor every 250 millisecond
	if ((unsigned long) (millis() - LastSensorUpdate) >= 250) {
		LastSensorUpdate = millis();

		repeat_T4:
		switch (SMT172_T4::getStatus()) {
		case 0: goto repeat_T4; // O Dijkstra, be merciful onto me, for I have sinned against you :)
		case 1: Serial.print(F("Measuring time   [ms]: "));
				Serial.println(SMT172_T4::getTime() * 1e3, 2); // convert to milliseconds
				Serial.print(F("Sensor frequency [Hz]: "));
				Serial.println(SMT172_T4::getFrequency(), 2);
				Serial.print(F("Duty cycle        [%]: "));
				Serial.println(SMT172_T4::getDutyCycle() * 100, 2);
				Serial.print(F("Temperature       [C]: "));
				Serial.println(SMT172_T4::getTemperature(), 2);
				Serial.print(F("Error            [mK]: "));
				Serial.println(SMT172_T4::getError() * 1000, 2);
				Serial.println();
				break;
		case 2: Serial.println(F("**** Sensor disconnected ****"));
				Serial.println();
		}

		repeat_T5:
		switch (SMT172_T5::getStatus()) {
		case 0: goto repeat_T5; // O Dijkstra, be merciful onto me, for I have sinned against you :)
		case 1: Serial.print(F("Measuring time   [ms]: "));
				Serial.println(SMT172_T5::getTime() * 1e3, 2); // convert to milliseconds
				Serial.print(F("Sensor frequency [Hz]: "));
				Serial.println(SMT172_T5::getFrequency(), 2);
				Serial.print(F("Duty cycle        [%]: "));
				Serial.println(SMT172_T5::getDutyCycle() * 100, 2);
				Serial.print(F("Temperature       [C]: "));
				Serial.println(SMT172_T5::getTemperature(), 2);
				Serial.print(F("Error            [mK]: "));
				Serial.println(SMT172_T5::getError() * 1000, 2);
				Serial.println();
				break;
		case 2: Serial.println(F("**** Sensor disconnected ****"));
				Serial.println();
		}

		SMT172_T4::startTemperature(0.002);
		SMT172_T5::startTemperature(0.002);
	}
}
