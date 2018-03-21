/*
 * variables.h
 *
 * Created: 21/03/2018 16:14:04
 *  Author: gtame
 */ 


 #include "Clases/Configuracion.h"
 #include "Clases/Riegos.h"
 #include "Clases/GSM.h"


 #include "settings.h"
 //#include <SIM900.h>
 #include <Keypad.h>
 #include <LiquidCrystal_I2C.h>
 #include <LogSD.h>


// 
/*
#include <StateMachineLib.h>
#include <SD.h>
#include <DS1307RTC.h>
#include <LowPower.h>
#include <Hora.h>
*/
#ifndef VARIABLES_H_
#define VARIABLES_H_



//Variables
extern char bufferapp[MAIN_BUFFER_SIZE];


//extern variables
extern Keypad keypad;
extern LiquidCrystal_I2C lcd;
extern LogSD SDCard;
extern Configuracion Config;
extern Riegos Riego;
extern GSM GSMModem;

//Interrupciones variables
extern volatile bool int_input_user;//El user pulso una key
extern volatile bool int_input_gsm;//El user pulso una key





#endif /* VARIABLES_H_ */