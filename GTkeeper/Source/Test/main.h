/*
 * maintest.h
 *
 *  Created on: 18 de abr. de 2017
 *      Author: Gabi
 */
//Referencia a la libreria comun de avr

#ifndef MAINTEST_H_
#define MAINTEST_H_


#include <ArduinoUnit.h>
#include <DS1307RTC.h>
#include <gtkeeper.h>
#include "variables.h"

#ifndef MAX_FILE_LOG_SIZE
	#define MAX_FILE_LOG_SIZE 100 //MAX log size for SD
#endif
 

#define SCREEN_ACTIVE() false
//definida en ScreenManager.h
#define ELAPSED_SECONDS(X)  (now()-X)


//Buffer para tests
struct definedtest
{
	char text[25];
	void (*Test)();
};

extern definedtest tests[25];
extern uint8_t contador;
extern char buffercommand[25];
extern char buffer_test[MAIN_BUFFER_SIZE];



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


//Main functions
int main();
void setup();
void loop(); //----( LOOP: RUNS CONSTANTLY )----

 

#endif /* MAINTEST_H_ */
