/*
 * pins_arduino.h
 *
 *  Created on: 7 de feb. de 2017
 *      Author: Gabi
 */

#ifndef PINS_MAPPING_ARDUINO_H_
#define PINS_MAPPING_ARDUINO_H_

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#include "variants/mega/pins_arduino.h"

#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#include "variants/standard/pins_arduino.h"


#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
//Apuntar al pins_arduino.h que se requiera

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
//Apuntar al pins_arduino.h que se requiera

#endif /* PINS_ARDUINO_H_ */


 #endif
