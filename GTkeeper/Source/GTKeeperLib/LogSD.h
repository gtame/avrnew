/* 
* Log.h
*
* Created: 18/03/2018 21:52:37
* Author: Gabi
*/

#include <SD.h>
#include <Logger.h>
#ifndef __LOGSD_H__
#define __LOGSD_H__


class LogSD
{
//variables
public:
protected:
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;
//functions
public:
	LogSD(char * internalbuffer,uint8_t sizebuffer);
	void Write(char *logtxt);
	
	
protected:
	bool SDInitializate();
private:
	void OpenFile();
	bool isInitializated;
	File dataFile;
	

}; //Log

#endif //__LOGSD_H__
