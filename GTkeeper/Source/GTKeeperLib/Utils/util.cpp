/*
 * CPPFile1.cpp
 *
 * Created: 20/03/2018 9:23:40
 *  Author: gtame
 */ 

#include "util.h"

bool isValidNumber(char* inputstr){
	uint8_t strl=strlen(inputstr);
	
	if (strl==0)
		return false;
		
	for(byte i=0;i<strl;i++)
		if(!isDigit(inputstr[i])) return false;
	
	return true;
}