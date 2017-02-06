/*
 ---------------------------------------------------------------------------
 Created by Edwin Croissant
 Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html

 See "SMT172.h" for purpose, syntax, version history, links, and more.
 ---------------------------------------------------------------------------
 */

#ifndef SMT172_T5_h
#define SMT172_T5_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

namespace SMT172_T5 {

void startTemperature(float sensorError);

uint8_t getStatus(void);

float getTemperature(void);

float getError(void);

float getFrequency(void);

float getDutyCycle(void);

float getTime(void);

}

#endif	
