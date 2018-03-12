/*
 * maintest.h
 *
 *  Created on: 18 de abr. de 2017
 *      Author: Gabi
 */
//Referencia a la libreria comun de avr

#include <gtkeeper.h>
#include "gtkeepertest.h"
#include <ArduinoUnit.h>
#include <DS1307RTC.h>


#ifndef MAINTEST_H_
#define MAINTEST_H_



#define SCREEN_ACTIVE() false
//definida en ScreenManager.h
#define ELAPSED_SECONDS(X)  (now()-X)


//Buffer para tests
struct definedtest
{
	char text[25];
	void (*Test)();
};
definedtest tests[25];
uint8_t contador=0;
char buffercommand[25];
char buffer_test[MAIN_BUFFER_SIZE];



//Funciones de test
void CargaProgramas();
void CheckWeb();
void RtcCheck();
void ListarSIM();
void CargaDesdeSIM();
void ShowConfigInfo();
void Config4String();
void Config2EEPROM();
void Config4EEPROM();
void Config4Web();
void ShowInfoProgramas();
void CheckReles();
void LanzaRiego();
void ShowInfoSalidas();





//Testing Functions
void ExecuteTest(uint8_t test);
void AddTest(char * menu,void (*Test)());
void PrintMenu();


//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}

//Main functions
int main();
void setup();
void loop(); //----( LOOP: RUNS CONSTANTLY )----

 

#endif /* MAINTEST_H_ */
