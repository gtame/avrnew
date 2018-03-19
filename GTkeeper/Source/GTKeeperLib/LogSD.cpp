/* 
* Log.cpp
*
* Created: 18/03/2018 21:52:37
* Author: Gabi
*/


#include "LogSD.h"

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
	
	 
	if (isInitializated || SDInitializate())
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
				sprintf_P(internalbuffer,PSTR("datalog%i.txt"),fileid);
				fileid++;
			}
			while (SD.exists(internalbuffer) && fileid<100);
				
			LOG_DEBUG_ARGS("Creating file %s",internalbuffer);
			
			if (!SD.exists(internalbuffer))
			{
				 
				
				dataFile=SD.open(internalbuffer,FILE_WRITE);
				if (dataFile)
					LOG_DEBUG("Abierto archivo data");				
				else
					LOG_DEBUG_ARGS("Error creando file %s",internalbuffer);
				
			}
			else 
				LOG_DEBUG("No se pudo dar de alta el archivo");
		}
			
		if (dataFile)
			result=	dataFile.println(txt); 
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
		 
		 return isInitializated;
	 }
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
	bool result=true;
	bool blnexit=false;
	if (isInitializated)
	{
		uint8_t fileid=1;
		do
		{
			fileid++;
			memset(internalbuffer,0,sizebuffer);
			sprintf_P(internalbuffer,"datalog%i.log",fileid);

			if (SD.exists(internalbuffer))
			{
				if (!SD.remove(internalbuffer))
				result=false;
			}
			else
			blnexit=true;
		}
		while (!blnexit && fileid<255);
	}
	else
	result=false;
}

bool  LogSD::ClearLogs()
{
	
	bool result=true;
	bool blnexit=false;
	if (isInitializated)
	{
		uint8_t fileid=1;
		do
		{
			fileid++;
			memset(internalbuffer,0,sizebuffer);
			sprintf_P(internalbuffer,"datalog%i.log",fileid);

			if (SD.exists(internalbuffer))
			{
				if (!SD.remove(internalbuffer))
					result=false;
			}
			else
				blnexit=true;
		}
		while (!blnexit && fileid<255);
	}
	else
		result=false;
	
	
}


uint32_t LogSD::WriteToStream(Stream *stream)
{
	uint32_t result=0;
	uint16_t readed=0;
	
	if (isInitializated)
	{	
		if (stream!=NULL)
		{
			if (dataFile && dataFile.available())
				dataFile.close();
			
			//Recorremos todos los archivos de log
			bool blnexit=false;
			uint8_t fileid=1;
			do
			{
				fileid++;
				memset(internalbuffer,0,sizebuffer);
				sprintf_P(internalbuffer,"datalog%i.log",fileid);

				if (SD.exists(internalbuffer))
				{
					File logFile=SD.open(internalbuffer,FILE_READ);
					do 
					{			
						memset(internalbuffer,0,sizebuffer);
						readed=logFile.read(internalbuffer,sizebuffer);
						if (readed!=0)
							stream->write(internalbuffer,result);
						else					
							logFile.close();
					}
					while(readed>0);
				}
				else
					blnexit=true;
			}
			while (!blnexit && fileid<255);
		
		}
	}	
	
	return result;
	
}