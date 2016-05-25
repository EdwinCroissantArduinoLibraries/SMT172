/*
 SMT172 - Smartec SMT172 library for the Arduino microcontroller

 Copyright (C) 2016 Edwin Croissant

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 PURPOSE:
 The SMT172 is a ultra-low power, high accuracy temperature sensor. The output
 signal is pulse width modulated (PWM) and the duty cycle is a function of
 the measured temperature. For more details see the home page of the
 manufacturer: http://smartec-sensors.com/ and the documentation.
 Accuracy can be up to 0.1 degree Celsius for a limited temperature range and
 the resolution up to 0.001 degree Celsius.
 The time the measurement takes is depending on the frequency of the sensor
 and the required sampling noise.
 With a 16 MHz CPU and a sampling noise of 0.002 C:
			 Sensor frequency [Hz]	Measurement time [ms] 
					  500					16 
					 1000					 8
					 4000					28
					 7000					46 

 SYNTAX:
 startTemperature(float sensorError);
	 initialize Timer1 to measure the duty cycle at the Input Capture Pin 1
	 the minimum required clock cycles is calculated on the fly for the required
	 standard deviation of the sampling noise after the first complete cycle of the sensor.
 uint8_t getStatus();
	 return 0 when busy, 1 when success, 2 when not connected
	 Status returns 2 after 2.5 ms after loss of signal
 float getTemperature();
	 return the temperature from the previous startTemperature command
	 in degrees Celsius
 float getError();
 	 return the standard deviation of the sampling noise
 float getFrequency();
 	 return the frequency of the sensor
 float getDutyCycle();
 	 return the duty cycle of the sensor
 float getTime();
 	 return the measuring time in seconds

 CONNECTION:
 Connect the SMT172 to the Input Capture Pin 1 (ICP1)
 
 CREDITS:
 Nick Gammon: Timing an interval using the input capture unit
 Michael Dreher: High-Speed capture mit ATmega Timer
 Michael P. Flaga: InputCapture.ino

 HISTORY:
 version 1.3 2016/02/12 minimum required amount of clock cycles is calculated
						on the fly after the first complete cycle of the sensor
 version 1.2 2016/01/30	added "sensor not connected" and the timer starts now
 	 	 	 	 	 	 with a raising edge capture and some rework of the code 
 version 1.1 2016/01/26 major rework of the code
 version 1.0 2016/01/18 initial version
  */

#ifndef SMT172_h
#define SMT172_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

namespace SMT172{

void startTemperature(float sensorError);

uint8_t getStatus(void);

float getTemperature(void);

float getError(void);

float getFrequency(void);

float getDutyCycle(void);

float getTime(void);

}

#endif	
