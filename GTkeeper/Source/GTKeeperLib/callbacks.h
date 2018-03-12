/*
 * callbacks.h
 *
 * Created: 12/03/2018 21:01:54
 *  Author: Gabi
 */ 

 
 #include <stdlib.h>
 #include <string.h>
 #include <inttypes.h>
 #include <stdio.h>
 #include "gtkeeper.h"

#ifndef XX_CALLBACKS_H_
#define XX_CALLBACKS_H_


void ResetConfigsCallBack(uint8_t button,uint8_t func);
bool ProcessATMensajesCallback(char * msg);


 

#endif /* CALLBACKS_H_ */