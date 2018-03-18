/*
 * gtKeeperHora.cpp
 *
 * Created: 18/03/2018 18:31:17
 *  Author: Gabi
 */ 

 #include "../gtkeeper.h"


 
 //Trata de comprobar si esta registrado en la red GSM
 bool GTKeeper::FijarHoraRTC()
 {

	 tmElements_t tm;
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
 bool GTKeeper::FijarHoraGSM()
 {
	 //Obtenemos hora del telefono
	 time_t hora_actual=GetTime(timeEl);

	 LOG_INFO_ARGS_B("Set hora->%02i/%s/%02i %02i:%02i:%02i",timeEl.Day, monthStr(timeEl.Month),timeEl.Year, timeEl.Hour,timeEl.Minute,timeEl.Second);

	 if (hora_actual!=0 && timeEl.Year!=01)
	 {
		 this->SetHora(hora_actual);
		 return true;
	 }
	 else
	 return false;


 }

 //Indica si la hora ha sido fijada
 bool GTKeeper::EstaEnHora()
 {
	 return isHourSet;
 }

 //Fija la hora :)
 void GTKeeper::SetHora(time_t time)
 {
	 if (time!=0)
	 isHourSet=true;
	 //Fijamos la hora en arduino
	 t_last_web=time;
	 setTime(time);
 }




