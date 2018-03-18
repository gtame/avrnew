/*
 * settings.h
 *
 *  Created on: 06/01/2016
 *      Author: Admin
 */
 #include "types.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

//Longitud de la eeprom
#define EEPROM_LENGTH E2END + 1

//Posiciones eeprom
#define GET_ADDRES_CONFIG 0
#define GET_ADDRES_PROGRAM(X) (LEN_PROGRAMA_STRING*X)+sizeof(tConfiguracion)
#define GET_ADDRES_ESTADISTICA(X) ((LEN_PROGRAMA_STRING*MAX_PROGRAMAS)+sizeof(Configuracion))+ (sizeof(Estadistica)*(X-1))
//EMPIEZAN LAS SALIDAS WEB EN -> PROGRAMAS + CONFIGURACION + ESTADISTICA
#define GET_ADDRES_SALIDAS_WEB ((LEN_PROGRAMA_STRING*MAX_PROGRAMAS)+sizeof(tConfiguracion))+ (sizeof(Estadistica)*PORTS_NUM)

#define LEN_CONFIG_STRING 20
#define LEN_PROGRAMA_STRING 17
#define LEN_SALIDA_STRING 22
#define LEN_PROGRAMA_STRING_CR_LF LEN_PROGRAMA_STRING+2


#define PARSE_TO_SECONDS(a) ((a)/1000)
#define MAX_SECONDS_ELAPSED 120


#ifdef PROTEUS
	#define GTKEEPER_SLEEP(x)  delay(500)
	#define MILLISECONDS_DELAY(x) x/100
	#define SECONDS_DELAY(x) x
	#define SCREEN_TIMEOUT 3 //En segundos
	#define ONINIT_REINTENTOS 1
#else
	#define GTKEEPER_SLEEP(x) LowPower.powerDown(x, ADC_OFF, BOD_OFF)
	#define MILLISECONDS_DELAY(x) x
	#define SECONDS_DELAY(x) x*1000
	#define SCREEN_TIMEOUT 60  //segundos
	#define ONINIT_REINTENTOS 5
#endif


//WEB
#define MAX_ERROR_WEB 20 //Numero de errores M�x para intentar realizar la comprobaci�n Web
#define SETTING_URL_PROGRAMACION "http://clubciclistautebo.es/custom/programas.php?id=%s"
#define SETTING_URL_CONFIG "http://clubciclistautebo.es/custom/config.txt"
#define URL_SEND_SALIDAS "http://posttestserver.com/post.php?dir=example"
//#define URL_SEND_SALIDAS "http://clubciclistautebo.es/custom/post.php?SN=%s"
#define TIME_CHECK_WEB_SECONDS 300 //Tiempo que indica que estara en el estado WEB
#define WEB_NOACTIVITY_SECONDS 600 // Segundos  que no debe a ver actividad a la vista para realizar el envio web.


//CALL
#define MAX_SECONDS_DTMF_TONE SECONDS_DELAY(10)

//ON
#define HOLD_TO_RESET 10 //Al arrancar numero de segundos que hay que mantener la tecla A-D pulsadas para entrar en RESET


//KEYPAD
#define KEYPAD_ROWS  4 //four rows
#define KEYPAD_COLUMNS  4//four columns



//Numero de programas , sectores 
#define MAX_PROGRAMAS 5//Numero m�ximo de programas


//Define de constantes  depuertos
#define PORTS_NUM 1//15 Numero max de puertos:)
#define PORTS_ABONO 2 //Numero de puertos de abono





#define MAIN_BUFFER_PARSE 50
#define  MAIN_BUFFER_SIZE 150
//#define  SERIAL_RX_BUFFER_SIZE 64//ESto es para evitar overflows de puerto serie
//#define  SERIAL_TX_BUFFER_SIZE 256 //ESto es para evitar overflows de puerto serie



/*
char * PBB (char * p1);
char * P2B(const __FlashStringHelper * __fmt,...);
*/

#include <Logger.h>
#include "errors.h"







 

#endif /* SETTINGS_H_ */
