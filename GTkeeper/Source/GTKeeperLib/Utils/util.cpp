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

#if DEBUG
void LogTime(time_t hora)
{
		TimeElements timeEl;
		 breakTime( hora, &timeEl);
		 //LOG_INFO7
		 LOG_INFO_ARGS("Chk->%02i/%s/%02i %02i:%02i:%02i",timeEl.Day, monthStr(timeEl.Month),timeEl.Year, timeEl.Hour,timeEl.Minute,timeEl.Second);	
	
}
#endif