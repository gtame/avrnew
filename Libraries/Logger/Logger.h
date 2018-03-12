/*
 * Logger.h
 *
 *  Created on: 14 de ene. de 2016
 *      Author: gtame
 */
/**
 *
 *  Clase utilizada para funciones de Log
 *  Usando SoftwareSerial o HardwareSerial
 *
 *  Defines utilizados
 *  LOG -> Para indicar que queremos utilizar la clase de LOG
 *  LOG_SOFTSERIAL -> Para indicar que queremos utilizar un SoftwareSerial
 *
 */

//--Mostramos la info leida



#ifndef LOGGER_H_
#define LOGGER_H_



#include <string.h>
#include <WString.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>



#ifndef LOG
	//Constantes y vitales
	#define LOG_DEBUG(X)
	#define LOG_INFO(X)
	#define LOG_CRITICAL(X)
	#define LOG_ERROR(X)
	#define LOG_WARN(X)

	#define LOG_DEBUG_ARGS(X,args...)
	#define LOG_INFO_ARGS(X,args...)
	#define LOG_CRITICAL_ARGS(X,args...)
	#define LOG_ERROR_ARGS(X,args...)
	#define LOG_WARN_ARGS(X,args...)


	#define LOG_DEBUG_B(X)
	#define LOG_INFO_B(X)
	#define LOG_CRITICAL_B(X)
	#define LOG_ERROR_B(X)
	#define LOG_WARN_B(X)


	#define LOG_DEBUG_ARGS_B(X,args...)
	#define LOG_INFO_ARGS_B(X,args...)
	#define LOG_CRITICAL_ARGS_B(X,args...)
	#define LOG_ERROR_ARGS_B(X,args...)
	#define LOG_WARN_ARGS_B(X,args...)



#else
	//Constantes y vitales
	#define LOG_DEBUG(X) Log.Debug(F(X))
	#define LOG_INFO(X) Log.Debug(F(X))
	#define LOG_CRITICAL(X) Log.Critical(F(X))
	#define LOG_ERROR(X) Log.Error(F(X))
	#define LOG_WARN(X) Log.Warning(F(X))

	#define LOG_DEBUG_ARGS(X,args...) Log.Debug(F(X),args)
	#define LOG_INFO_ARGS(X,args...) Log.Debug(F(X),args)
	#define LOG_CRITICAL_ARGS(X,args...) Log.Critical(F(X),args)
	#define LOG_ERROR_ARGS(X,args...) Log.Error(F(X),args)
	#define LOG_WARN_ARGS(X,args...) Log.Warning(F(X),args)


	#define LOG_DEBUG_B(X) Log.Debug(X)
	#define LOG_INFO_B(X) Log.Debug(X)
	#define LOG_CRITICAL_B(X) Log.Critical(X)
	#define LOG_ERROR_B(X) Log.Error(X)
	#define LOG_WARN_B(X) Log.Warning(X)

	#define LOG_DEBUG_ARGS_B(X,args...) Log.Debug(X,args)
	#define LOG_INFO_ARGS_B(X,args...) Log.Debug(X,args)
	#define LOG_CRITICAL_ARGS_B(X,args...) Log.Critical(X,args)
	#define LOG_ERROR_ARGS_B(X,args...) Log.Error(X,args)
	#define LOG_WARN_ARGS_B(X,args...) Log.Warning(X,args)



#endif




#define LOG_BUFFER_SIZE 50//TAMAÑO DEL BUFFER DEL LOG
//#define LOG_BUFFER_PARSE 128 //TAMAÑO DEL BUFFER DEL LOG

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_CRITICAL 4

class Logger {

public:

	Logger(Stream *serial);

	virtual ~Logger();


	//Pone el nivel
    inline uint8_t  getLevel(void)      { return _level;  }
    inline void setLevel(uint8_t level) { _level = level; }
/*
    void Info (char *log);
    void Debug (char *log);
    void Warning (char *log);
    void Error (char *log);
    void Critical (char *log);*/


    void Info (char *log,...);
    void Debug (char *log,...);
    void Warning (char *log,...);
    void Error (char *log,...);
    void Critical (char *log,...);


    void Info (const __FlashStringHelper *log,...);
    void Debug (const __FlashStringHelper *log,...);
    void Warning (const __FlashStringHelper *log,...);
    void Error (const __FlashStringHelper *log,...);
    void Critical (const __FlashStringHelper *log,...);

    void LogCommand(const char*, char*);


  private:
    char * ParseToBuff_P(const __FlashStringHelper * __fmt,...);
    char * ParseToBuff(const char * __fmt,...);


    char buffer_log[LOG_BUFFER_SIZE];
    uint8_t _level;
	Stream* mySerial;


};

extern Logger Log;

#endif /* LOGGER_H_ */

