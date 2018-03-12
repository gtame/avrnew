/*
 * errors.h
 *
 *  Created on: 15 de ene. de 2016
 *      Author: gtame
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#define ERROR_BASE 0
#define ERROR_NO_REGISTER ERROR_BASE-1 // "No Registrado en la red movil"
#define ERROR_SIM ERROR_BASE-2//Sim no esta lista.
#define ERROR_NO_RESPOND ERROR_BASE-3//"Modulo no responde - Reiniciando modulo"




//Carga de WEB
#define LOAD_WEB_OK 0
#define LOAD_WEB_ERR_NO_RESPONSE 1
#define LOAD_WEB_ERR_SAVE_SIM 2
#define LOAD_WEB_ERR_LOAD_PROG 3
#define LOAD_WEB_ERR_UNKNOWN_STRING 4
#define LOAD_WEB_ERR_DELETE_PROGRAM 5


#define INFO_BASE 0
#define INFO_HTTP_DOWNLOAD INFO_BASE +1
#endif /* ERRORS_H_ */
