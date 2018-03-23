/*
 * CPPFile1.cpp
 *
 * Created: 20/03/2018 9:23:40
 *  Author: gtame
 */ 

#include "util.h"

//Valida si la entrada es un numero
bool isValidNumber(char* inputstr){
	uint8_t strl=strlen(inputstr);
	
	if (strl==0)
		return false;
		
	for(byte i=0;i<strl;i++)
		if(!isDigit(inputstr[i])) return false;
	
	return true;
}


char * PBB (char * mybuffer, int8_t sizeofbuffer, const __FlashStringHelper * p1,...)
{

	va_list ap;
	va_start(ap,p1);
	memset(mybuffer,0,sizeofbuffer);
	vsnprintf_P(mybuffer, sizeofbuffer,(char*)p1, ap);
	va_end(ap);
	return mybuffer;
}



#if DEBUG
void LogTime(time_t hora)
{
		TimeElements timeEl ;
		breakTime( hora, timeEl);
		//LOG_INFO7
		LOG_INFO_ARGS("Chk->(%i) %02i/%s/%02i %02i:%02i:%02i",dayToDiasSemana(dayOfWeek2(hora)) ,timeEl.Day, monthStr(timeEl.Month),tmYearToCalendar( timeEl.Year), timeEl.Hour,timeEl.Minute,timeEl.Second);
		
}
#endif