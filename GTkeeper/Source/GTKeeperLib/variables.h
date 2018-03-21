/*
 * variables.h
 *
 * Created: 21/03/2018 16:14:04
 *  Author: gtame
 */ 

#include "settings.h"


//#include <SIM900.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
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

//Constantes
extern const uint8_t ports[PORTS_NUM] ;
extern  const uint8_t ports_abono[PORTS_ABONO];

//extern variables
extern Keypad keypad;
extern LiquidCrystal_I2C lcd;


//Interrupciones variables
extern volatile bool int_input_user;//El user pulso una key
extern volatile bool int_input_gsm;//El user pulso una key





#endif /* VARIABLES_H_ */