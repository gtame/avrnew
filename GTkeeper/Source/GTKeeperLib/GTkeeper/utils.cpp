/*
 * utils.cpp
 *
 * Created: 18/03/2018 18:41:33
 *  Author: Gabi
 */ 

///UTils...
#include "../gtkeeper.h"

char * GTKeeper::PBB (const __FlashStringHelper * p1,...)
{

	va_list ap;
	va_start(ap,p1);
	memset(buffer,0,MAIN_BUFFER_SIZE);
	vsnprintf_P(buffer, MAIN_BUFFER_SIZE,(char*)p1, ap);
	va_end(ap);
	return buffer;
}

time_t GTKeeper::GetTimeWithoutSeconds(time_t tiempo)
{
	breakTime(tiempo, timeEl); // break time_t into elements
	timeEl.Second = 0; //Quitamos los segundos para que sea hora exacta..
	return makeTime(timeEl); // convert time elements into time_
}
