/*
 * main.cpp


 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
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



/*
#include "settings.h"
#include "gtKeeper.h"
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "main.h"
#include "maintest.h"
*/


//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}


#include <ArduinoUnit.h>


 test(correct)
 {
	 int x=1;
	 assertEqual(x,1);
 }

 test(incorrect)
 {
	 int x=1;
	 assertNotEqual(x,1);
 }


 
 test(supu)
 {
	 int x=1;
	 assertNotEqual(x,1);
 }


 
 test(pepo)
 {
	 int x=1;
	 assertNotEqual(x,1);
 }



 void setup()
 {
	 Serial.begin(9600);
	 while(!Serial); // for the Arduino Leonardo/Micro only
 }

 void loop()
 {
	 Test::run();
 }
  

int main() {

	//Necesario para la inicializacion de arduino
	init();

	//Setup
	setup();


	delay(100);
	//Loop
	while (true)
	loop();

}