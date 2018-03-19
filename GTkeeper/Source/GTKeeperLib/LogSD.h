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

#ifndef MAX_FILE_LOG_SIZE 
	//#define MAX_FILE_LOG_SIZE 1024 //(1024 bytes =1 kbyte - Ficheros de log de 1kb max.
	#define MAX_FILE_LOG_SIZE 100
#endif

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
	uint32_t WriteLogToStream(Stream *stream);//Escribe el contenido de los logs al stream
	void FlushLog();//Flush cierra archivos internamiente y asegura cambios pdtes.
protected:
	bool SDInitializate(); //Incializa SDCard
private:
	bool isInitializated;
	File dataFile;
	

}; //Log

#endif //__LOGSD_H__
