/* 
* GSM.h
*
* Created: 21/03/2018 16:21:20
* Author: gtame
*/

#include <Logger.h>
#include <Time.h>
#include <SIM900.h>
#include "settings.h"

#ifndef __GSM_H__
#define __GSM_H__


class GSM:public SIM900
{
//variables
public:
protected:
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;

//functions
public:
	GSM(char *ibuffer,uint8_t isizebuff, Stream * stream);



	//SMS
	void SendSmsFinReinicio();
	void SendSmsIniReinicio();
	void SendSmsProgramacion();
	void SendSmsSectoresEjecucion();
	void SendSmsHora();


protected:
	char * PBB (const __FlashStringHelper * p1,...)
private:


}; //GSM

#endif //__GSM_H__
