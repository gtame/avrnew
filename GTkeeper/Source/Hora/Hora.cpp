/* 
* Hora.cpp
*
* Created: 21/03/2018 8:46:54
* Author: gtame
*/


#include "Hora.h"

// default constructor
Hora::Hora()
{
	isHourSet=false;
} //Hora

 //Trata de comprobar la hora utilizando RTC module
 bool Hora::FijarHoraRTC()
 {

	
	 if (RTC.read(tm))
	 {
		 time_t hora_actual =makeTime(tm);
		 this->SetHora(hora_actual);
		 return true;
	 }
	 else
		return false;

 }

 //Trata de fijar la hora utilizando la red GSM
 bool Hora::FijarHoraGSM(ATModem *modem)
 {
	 //Obtenemos hora del telefono
	 time_t hora_actual=modem->GetTime();

	 //LOG_INFO_ARGS_B("Set hora->%02i/%s/%02i %02i:%02i:%02i",tm.Day, monthStr(tm.Month),tm.Year, tm.Hour,tm.Minute,tm.Second);

	 if (hora_actual!=0 && tm.Year!=01)
	 {
		 this->SetHora(hora_actual);
		 return true;
	 }
	 else
		 return false;


 }

 //Indica si la hora ha sido fijada
 bool Hora::EstaEnHora()
 {
	 return isHourSet;
 }

 //Fija la hora :)
 void Hora::SetHora(time_t time)
 {
	 if (time!=0)
	 isHourSet=true;
	 
	 //Fijamos la hora en arduino
	 setTime(time);
 }



