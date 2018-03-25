/*
 * utils.cpp
 *
 * Created: 24/03/2018 7:49:25
 *  Author: Gabi
 */ 
 #include "utils.h"

time_t  GetTime(int hr,int min,int sec,int day, int month, int yr)
{
	//Fijamos la fecha a 1/1/2018 0:00 Lunes
	TimeElements time;
	time.Year=CalendarYrToTm(yr);
	time.Month=month;
	time.Day=day;
	time.Hour=hr;
	time.Minute=min;
	time.Second=sec;
	time_t ahora=makeTime(time);
	
	return ahora;
	
}


void FijarFecha(int year, uint8_t month ,uint8_t day)
{
	//Fijamos la fecha a 1/1/2018 0:00 Lunes
	setTime(GetTime(0,0,0,day,month,year));
}

