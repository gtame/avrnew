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
//Referencia al core de arduino
#include <Arduino.h>
#include <string.h>
#include "settings.h"
#include "gtKeeper.h"
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "main.h"

#ifdef TEST

#ifndef MAINTEST_H_
#define MAINTEST_H_


void AddTest(char * menu,void (*Test)());


void CargaProgramas();
void CheckWeb();


void ListarSIM();

void CargaDesdeSIM();


void ShowConfigInfo();
void Config4String();
void setupTest();



void ExecuteTest(uint8_t test);

void PrintMenu();



void loop(); //----( LOOP: RUNS CONSTANTLY )----

#endif

#endif /* MAINTEST_H_ */
