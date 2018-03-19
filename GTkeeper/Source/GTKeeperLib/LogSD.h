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

#define MAX_FILE_LOG_SIZE 1024*8*300 //(1024 bytes * 8 ) 1024 KBytes * 300 = 300 Kbytes

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
	LogSD(char * internalbuffer,uint8_t sizebuffer); //constructor del log
	int8_t WriteLog(char *logtxt); //escribe un text al log
	uint32_t SizeLogs(); // Tamaño de los archivos de logs
	bool ClearLogs(); //Elimina los logs
	uint32_t WriteToStream(Stream *stream);//Escribe el contenido de los logs al stream
protected:
	bool SDInitializate(); //Incializa SDCard
private:
	bool isInitializated;
	File dataFile;
	

}; //Log

#endif //__LOGSD_H__
