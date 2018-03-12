/*
 * maintest.h
 *
 *  Created on: 18 de abr. de 2017
 *      Author: Gabi
 */
//Referencia a la libreria comun de avr
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>



#include <Arduino.h>
#include <string.h>
#include <Logger.h>
#include <AltSoftSerial.h>
#include <Neoway590.h>
#include <ArduinoUnit.h>
 

#ifndef MAINTEST_H_
#define MAINTEST_H_


#define MAIN_BUFFER_SIZE 25
//AltSoftSerial utiliza pines 8 y 9!!
#define PIN_INTERRUPT_RING 2
#define PIN_CONTROL_BATTERY 4
#define PIN_CONTROL_WATER 5
#define PIN_CONTROL_GSM 6
#define PIN_READ_BATTERY A0
#define PIN_READ_WATER  10

extern AltSoftSerial mySerial;
   
#endif /* MAINTEST_H_ */
