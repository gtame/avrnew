/* 
* Log.cpp
*
* Created: 18/03/2018 21:52:37
* Author: Gabi
*/


#include "LogSD.h"

static const char LOG_FILE_PATTERN [] PROGMEM ="/log%i.txt";

// default constructor
LogSD::LogSD(char * ibuffer,uint8_t isizebuffer)
{
	 isInitializated=false;
	 
	 internalbuffer=ibuffer;
	 sizebuffer=isizebuffer;
} //Log

 
 int8_t LogSD::WriteLog(char * txt)
 {
	 int8_t result=-1;
	
	 
	if (SDInitializate())
	{

 		if (dataFile && dataFile.size()>MAX_FILE_LOG_SIZE)
			dataFile.close();
		
 
		if (!dataFile)
		{ 
			//obtenemos el nuevo nombre del file
			uint8_t fileid=1;			
			do
			{	
				memset(internalbuffer,0,sizebuffer);
				sprintf_P(internalbuffer,LOG_FILE_PATTERN,fileid);
				fileid++;
			}
			while (SD.exists(internalbuffer) && fileid<100);
				
			LOG_DEBUG_ARGS("Creating file %s",internalbuffer);
			
			if (!SD.exists(internalbuffer))
			{
				dataFile=SD.open(internalbuffer,FILE_WRITE);
				if (dataFile)
					LOG_DEBUG_ARGS("Abierto archivo %s",internalbuffer);				
				else
					LOG_DEBUG_ARGS("Error creando file %s",internalbuffer);
				
			}
			else 
				LOG_DEBUG("No se pudo dar de alta el archivo");
		}
			
		if (dataFile)
		{
			result=	dataFile.println(txt); 
			LOG_DEBUG_ARGS("Write Log %s",txt);	
		}
		else
			LOG_DEBUG("No se pudo escribir el log");	
				 
	}
	else
		LOG_DEBUG("Error grabando log");
		
		
		return result;
 }
 
 bool LogSD::SDInitializate()
 {
	 
 
	 if (!isInitializated)
	 {
		 LOG_DEBUG_ARGS("PIN SELECT %i",SD_CHIP_SELECT_PIN);
		 if ( SD.begin(SPI_QUARTER_SPEED, SD_CHIP_SELECT_PIN))
		 {
				isInitializated=true;	 
				LOG_DEBUG("Inicializada SD");
		 } 
	 }
	 
	return isInitializated;
	 //else
	 //{
		 //if (!SD.exists("log"))
		 //SD.mkdir("log");
//
		 //return SD.exists("log");
	 //}
 }

uint32_t LogSD::SizeLogs()
{
	uint32_t result=0;
	bool blnexit=false;
	if (SDInitializate())
	{

		FlushLog();

		uint8_t fileid=1;
		do
		{
			
			memset(internalbuffer,0,sizebuffer);
			sprintf_P(internalbuffer,LOG_FILE_PATTERN,fileid);

			if (SD.exists(internalbuffer))
			{
				File filelog=SD.open(internalbuffer, FILE_READ);
				if (filelog)
				{
					result+=filelog.size();
					filelog.close();
				}
				else
				{					
					LOG_DEBUG_ARGS("No se pudo abrir %s",internalbuffer);
					result=-1;
				}

			}
			else
				blnexit=true;

			 fileid++;
		}
		while (!blnexit && fileid<255);
	}
	else
		result=-1;

	return result;
}


void LogSD::FlushLog()
{

if (dataFile)
	dataFile.close();
}

bool  LogSD::ClearLogs()
{
	
	bool result=true;
	bool blnexit=false;
	if (SDInitializate())
	{

		FlushLog();

		uint8_t fileid=1;
		do
		{
		
			memset(internalbuffer,0,sizebuffer);
			sprintf_P(internalbuffer,LOG_FILE_PATTERN,fileid);
			//LOG_DEBUG_ARGS("ARchivo %s",internalbuffer);
			if (SD.exists(internalbuffer))
			{
				//LOG_DEBUG_ARGS("Eliminando %s",internalbuffer);
				if (!SD.remove(internalbuffer))
				{
					//LOG_DEBUG_ARGS("No pudo eliminarse %s",internalbuffer);
					result=false;
				}

			}
			else
			{
				//LOG_DEBUG_ARGS("No existe %s",internalbuffer);
				blnexit=true;
			}

			fileid++;
		}
		while (!blnexit && fileid<255);
	}
	else
		result=false;
	

	return result;
	
}


uint32_t LogSD::WriteLogToStream(Stream *stream)
{
	uint32_t result=0;
	uint16_t readed=0;
	
	if (SDInitializate())
	{	
		if (stream!=NULL)
		{
			FlushLog();
			
			//Recorremos todos los archivos de log
			bool blnexit=false;
			uint8_t fileid=1;
			do
			{
				
				memset(internalbuffer,0,sizebuffer);
				sprintf_P(internalbuffer,LOG_FILE_PATTERN,fileid);

				if (SD.exists(internalbuffer))
				{
					File logFile=SD.open(internalbuffer,FILE_READ);
					if (logFile)
					{					
						do
						{
							memset(internalbuffer,0,sizebuffer);
							readed=logFile.read(internalbuffer,sizebuffer);
							if (readed!=0)
								result+=stream->write(internalbuffer,readed);
							else
							logFile.close();
						}
						while(readed>0);

						logFile.close();
					}

				}
				else
					blnexit=true;

				fileid++;
			}
			while (!blnexit && fileid<255);
		
		}
	}	
	
	return result;
	
}