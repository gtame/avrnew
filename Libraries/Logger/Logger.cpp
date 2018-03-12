/*
 * Logger.cpp

 *
 *  Created on: 14 de ene. de 2016
 *      Author: gtame
 */


#define VA_LOG(X,Y) 	if (_level<=X)\
	{\
		va_list ap;\
		va_start(ap,log);\
		memset(buffer_log,0,LOG_BUFFER_SIZE);\
		vsnprintf(buffer_log, LOG_BUFFER_SIZE,log, ap);\
		LogCommand(Y,buffer_log);\
		va_end(ap);\
	}\


#define VA_LOG_P(X,Y) 	if (_level<=X)\
	{\
		va_list ap;\
		va_start(ap,log);\
		memset(buffer_log,0,LOG_BUFFER_SIZE);\
		vsnprintf_P(buffer_log, LOG_BUFFER_SIZE,(char*)log, ap);\
		LogCommand(Y,buffer_log);\
		va_end(ap);\
	}\



#include "Logger.h"




	Logger::Logger(Stream *serial)
	{
		_level=LOG_LEVEL_DEBUG;
		mySerial=serial;
	}


Logger::~Logger() {
	// TODO Auto-generated destructor stub

}


//char * Logger::ParseToBuff_P(const __FlashStringHelper * __fmt,...)
//{
//	memset(buffer_log,0,LOG_BUFFER_SIZE);
//
//	vsnprintf_P(buffer_log, LOG_BUFFER_SIZE,__fmt, ap);
//	va_end(ap);
//
//	return buffer_log;
//}
//



void Logger::Warning (const __FlashStringHelper *log,...)
{

	VA_LOG_P(LOG_LEVEL_WARNING,"WARN:");

}

void Logger::Info (const __FlashStringHelper *log,...)
{
	VA_LOG_P(LOG_LEVEL_INFO,"INFO:");

}


void Logger::Error (const __FlashStringHelper *log,...)
{
	VA_LOG_P(LOG_LEVEL_ERROR,"ERROR:");
}


void Logger::Debug (const __FlashStringHelper  *log,...)
{
	VA_LOG_P(LOG_LEVEL_DEBUG,"DEBUG:");
}


 void Logger::Critical (const __FlashStringHelper  *log,...)
 {
	 VA_LOG_P(LOG_LEVEL_CRITICAL,"CRITICAL:");
 }


void Logger::Warning (char *log,...)
{

	VA_LOG(LOG_LEVEL_WARNING,"WARN:");

}

void Logger::Info (char *log,...)
{
	VA_LOG(LOG_LEVEL_INFO,"INFO:");

}


void Logger::Error (char *log,...)
{
	VA_LOG(LOG_LEVEL_ERROR,"ERROR:");
}


void Logger::Debug (char *log,...)
{
	VA_LOG(LOG_LEVEL_DEBUG,"DEBUG:");
}


 void Logger::Critical (char *log,...)
 {
	 VA_LOG(LOG_LEVEL_CRITICAL,"CRITICAL:");
 }


 void Logger::LogCommand (const char* tipo, char *log)
 {
		Logger::mySerial->print(tipo);
		Logger::mySerial->print("->");
		Logger::mySerial->println(log);

 }


