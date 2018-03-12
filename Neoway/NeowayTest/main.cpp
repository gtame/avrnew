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
#include "maintest.h"


//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}


 
 


AltSoftSerial mySerial;
Neoway590 tNeoway590(&mySerial,PIN_CONTROL_GSM);
Logger Log(&Serial);

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


void setup()
{
 

	mySerial.begin(9600);
	Serial.begin(9600); 

	pinMode(PIN_CONTROL_BATTERY ,OUTPUT);
	digitalWrite(PIN_CONTROL_BATTERY,LOW);

	pinMode(PIN_CONTROL_WATER ,OUTPUT);
	digitalWrite(PIN_CONTROL_WATER,LOW);

	pinMode(PIN_READ_WATER ,INPUT );
	digitalWrite(PIN_READ_WATER,LOW);

	Test::exclude("*");

//Test basicos, condicionantes para el resto de tests
	Test::include("estaactivo");
	Test::include("tcp_config");


//Tests definidos para http
#if HTTP
	Test::include("http_*");
	//Test::include("http_get");
	//Test::include("http_put");
	//Test::include("http_post");
#endif


//Tests definidos para ftp
#if FTP
	Test::include("ftp*");
#endif

//Tests definidos para ftp
#if WSOCKETS
	Test::include("ws*");
#endif



/*	Test::exclude("*");
	Test::exclude("onoff_*");
	Test::exclude("ftp*");
	Test::exclude("http*");
	Test::exclude("sim*");
	Test::exclude("phone*");
	
	Test::include("expectedResult");
	Test::include("estaactivo");
	*/

	//Test::max_verbosity = TEST_VERBOSITY_TESTS_FAILED | TEST_VERBOSITY_TESTS_PASSED;
	 /*
	   Test::exclude("*");
	   Test::include("dev_*");
	   Test::exclude("*_slow");
	   Test::exclude("*_skip");
	   Test::exclude("*eeprom*");*/

	LOG_DEBUG("Inicializando");
	 

  }


void loop() //----( LOOP: RUNS CONSTANTLY )----
{
	
	Test::run();

}

