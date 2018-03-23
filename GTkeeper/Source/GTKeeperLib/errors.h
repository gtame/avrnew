/*
 * errors.h
 *
 *  Created on: 15 de ene. de 2016
 *      Author: gtame
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#define NO_ERROR	0
#define ERROR_BASE -1
#define WARN_BASE 100
//ERRORES
#define ERROR_NO_CONFIG		ERROR_BASE+1	// "La configuracion no se cargo correctamente"
#define ERROR_NO_PROG		ERROR_BASE+2	// "Los programas no se cargaron correctamente"
#define ERROR_NO_HORA		ERROR_BASE+3	// "No se pudo cargar la hora"

//ERRORES GSM;
#define ERROR_NO_REGISTER	ERROR_BASE-1	// "No Registrado en la red movil"
#define ERROR_SIM			ERROR_BASE-2	//Sim no esta lista.
#define ERROR_NO_RESPOND	ERROR_BASE-3		//"Modulo no responde - Reiniciando modulo"

//ERRORES SD CARD
#define ERROR_NO_SDCARD		ERROR_BASE-4		//"No hay memoria SD CARD"
#define ERROR_IO_SDCARD		ERROR_BASE-5		//"Error en operacion I/O SD CARD"

//ERRORES RTC
#define ERROR_NO_RTC		ERROR_BASE-10			//"Modulo RTC NO Responde"
#define ERROR_RTC_NOINIT	ERROR_BASE-3			//"Modulo RTC sin inicializar"

//ERRORES PROGRAMA;
#define ERROR_OFF_NOEXPECTED		ERROR_BASE-10			//"Corte electrico"
#define ERROR_OFF_UNKNOW		ERROR_BASE-10			//"Error desconocido"

//ERRORES WEB;
#define LOAD_WEB_OK 0
#define LOAD_WEB_ERR_NO_RESPONSE 1
#define LOAD_WEB_ERR_SAVE_SIM 2
#define LOAD_WEB_ERR_LOAD_PROG 3
#define LOAD_WEB_ERR_UNKNOWN_STRING 4
#define LOAD_WEB_ERR_DELETE_PROGRAM 5
#define LOAD_WEB_ERR_MALFORMED_FILE  6
#define LOAD_WEB_ERR_UNKNOWN_RESPONSE 7
#define LOAD_WEB_ERR_SERVER_RESPONSE 8
#define LOAD_WEB_ERR_MALFORMED_CONFIG 9
#define LOAD_WEB_ERR_CANT_LOAD_CONFIG 10
#define LOAD_WEB_ERR_UNKNOWN_DELETE_RESPONSE 11
#define LOAD_WEB_ERR_CANT_LOAD_PROGRAM 12
#define LOAD_WEB_ERR_MALFORMED_PROGRAM 13



#define INFO_BASE 0
#define INFO_HTTP_DOWNLOAD INFO_BASE +1
#endif /* ERRORS_H_ */
