/*
 Demo sketch for the SMT172 library. 
 This sketch is intended for the ATmega328P (Uno, Nano, Pro Mini etc.)
 There is no timer2 on the ATmega32U4 (Pro Micro)
 The ICP1 pin is not exposed on the Mega2560R3 board so timer4 or timer5 must be
 used requiring a modification of the SMT172 library. This board also uses
 different pins for timer2.
 This sketch will output to serial.
 Connect the output of the SMT172 to pin 8 (Input Capture Pin of timer 1)
 Timer 2 is set up in phase correct PWM and output a duty cycle of
 10.98% ~-45.06 C on pin 3 and a duty cycle of 92.94% ~139.58 C on pin 11
 Connect pin 8 to pin 3 or pin 11 to check the working if no SMT172 is available
*/

#include<arduino.h>
#include <SMT172.h>

uint32_t LastSensorUpdate;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(115200);
	pinMode(8, INPUT);
	pinMode(12, OUTPUT);
	Serial.println(F("Demo sketch SMT172"));

//	The following code fragment sets up phase-correct PWM on pins 3 and 11 (Timer 2).
//	The waveform generation mode bits WGM are set to to 001 for phase-correct PWM.
//	The other bits are the same as for fast PWM.

	pinMode(3, OUTPUT);
	pinMode(11, OUTPUT);
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
	// TCCR2B = _BV(CS22); // Output frequency: 16 MHz / 64 / 255 / 2 =  490.196 Hz
	TCCR2B = _BV(CS21); // Output frequency: 16 MHz /  8 / 255 / 2 = 3921.569 Hz
	OCR2A = 237; // Output A duty cycle: 237 / 255 = 92.94%	= 129.58 C	on pin 11
	OCR2B = 28;	 // Output B duty cycle:  28 / 255 = 10.98%	= -45.06 C	on pin 3
}

// The loop function is called in an endless loop
void loop() {
	// read the sensor every 250 millisecond
	if ((unsigned long) (millis() - LastSensorUpdate) >= 250) {
		LastSensorUpdate = millis();

		SMT172::startTemperature(0.002);

		repeat:
		switch (SMT172::getStatus()) {
		case 0: goto repeat; // O Dijkstra, be merciful onto me, for I have sinned against you :)
		case 1: Serial.print(F("Measuring time   [ms]: "));
				Serial.println(SMT172::getTime() * 1e3, 2); // convert to milliseconds
				Serial.print(F("Sensor frequency [Hz]: "));
				Serial.println(SMT172::getFrequency(), 2);
				Serial.print(F("Duty cycle        [%]: "));
				Serial.println(SMT172::getDutyCycle() * 100, 2);
				Serial.print(F("Temperature       [C]: "));
				Serial.println(SMT172::getTemperature(), 2);
				Serial.print(F("Error            [mK]: "));
				Serial.println(SMT172::getError() * 1000, 2);
				Serial.println();
				break;
		case 2: Serial.println(F("**** Sensor disconnected ****"));
				Serial.println();
		}
	}
}
