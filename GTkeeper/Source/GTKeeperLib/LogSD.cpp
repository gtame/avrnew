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
 
 internalbuffer=ibuffer;
 sizebuffer=isizebuffer;
} //Log


void LogSD::OpenFile()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("datalog.txt", FILE_WRITE);
	
}
 
 void LogSD::Write(char * txt)
 {
	
	if (isInitializated && !SDInitializate())
	{
		if (dataFile && dataFile.available())
		{
			if (dataFile.size()>(1024*8*3))
			{
				dataFile.close();
				uint8_t fileid=1;

				while (SD.exists() && fileid<255)
					fileid++;

				if (!SD.exists())
					dataFile=SD.open("datalog_%s.txt",FILE_WRITE);
			}
			else
				dataFile.println(txt); 
		}
		else
			LOG_DEBUG("Error grabando log");

	}
	else
		LOG_DEBUG("Error grabando log");
 }
 
 bool LogSD::SDInitializate()
 {
	 
	 if (!isInitializated)
	 {
		 LOG_DEBUG_ARGS("PIN SELECT %i",SD_CHIP_SELECT_PIN);

		 if ( SD.begin(SPI_QUARTER_SPEED, SD_CHIP_SELECT_PIN))
		 //if ( SD.begin( SD_CHIP_SELECT_PIN))
		 isInitializated=true;
		 return isInitializated;
	 }
	 else
	 {
		 if (!SD.exists("log"))
		 SD.mkdir("log");

		 return SD.exists("log");
	 }
 }

