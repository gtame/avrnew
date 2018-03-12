/*
 * settings.h
 *
 *  Created on: 06/01/2016
 *      Author: Admin
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_




#define MAX_PROGRAMAS 5//Numero máximo de programas



#define MAX_ERROR_WEB 20 //Numero de errores Máx para intentar realizar la comprobación Web


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


#define PIN_RESET 2 // Pin reset para modo simple (sin pantalla)

//PROGRAMACION WEB
#define SETTING_URL_PROGRAMACION "http://clubciclistautebo.es/custom/programas.php"
#define SETTING_URL_CONFIG "http://clubciclistautebo.es/custom/config.txt"
//#define URL_SEND_SALIDAS "http://posttestserver.com/post.php?dir=example"
#define URL_SEND_SALIDAS "http://clubciclistautebo.es/custom/post.php?SN=%s"

//Variable para indicar si va a comprobar cada X tiempo si hay programación nueva en la web
//#ifndef DEBUG
	#define NO_CHECK_WEB 1
//#endif

#ifndef NO_CHECK_WEB
	#define TIME_CHECK_WEB_SECONDS 300 //Tiempo donde se define cada cuanto debe chequear programas
#else
	#define TIME_CHECK_WEB_SECONDS 0 //Tiempo donde se define cada cuanto debe chequear programas
#endif







// Print a string from Program Memory directly to save RAM

/*
void print_Px(const char *str)
{
  uint8_t val;
  while (true) {
    val=pgm_read_byte(str);
    if (!val) break;
    Log.Data(val);
    str++;
  }
}*/


#endif /* SETTINGS_H_ */
