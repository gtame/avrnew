/* 
* Hora.h
*
* Created: 21/03/2018 8:46:55
* Author: gtame
*/

#include <Time.h>
#include <Logger.h>
#include <DS1307RTC.h>
#include <ATModem.h>

#ifndef __HORA_H__
#define __HORA_H__


class Hora
{
//variables
public:
protected:
private:
	bool isHourSet;//Flag para saber si se ha fijado la hora
	tmElements_t tm; //Elementos de tiempo
	
//functions
public:
	Hora();
	bool FijarHoraRTC();
	bool FijarHoraGSM(ATModem* modem);
	bool EstaEnHora();
	void SetHora(time_t time);
protected:
	
private:
	

}; //Hora

#endif //__HORA_H__
