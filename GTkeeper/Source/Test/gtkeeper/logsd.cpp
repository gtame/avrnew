/*
 * sdlog.cpp
 *
 * Created: 19/03/2018 12:36:50
 *  Author: gtame
 */ 
#include "..\main.h"

void printDirectory(File dir, int numTabs) {
	while (true) {

		
		File entry =  dir.openNextFile();
		if (! entry) {
			// no more files
			  // return to the first file in the directory
			  dir.rewindDirectory();
			  break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
			} else {
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}

uint8_t CountFilesDir(char * dir) {
	uint8_t result=0;

	File path = SD.open(dir);
	if (path)
	{
		LOG_DEBUG_ARGS("abierto dir %s",dir);
		path.rewindDirectory();
		File entry= path.openNextFile();
		if (!entry	)
			LOG_DEBUG_ARGS("Entry no tiene files %s",dir);

		while (entry)
		{
			 LOG_DEBUG("Entry");

			if(!entry.isDirectory())
				result++;
				
			entry.close();
			

			entry= path.openNextFile();
		}
		
		path.close();
	}
	else 
		LOG_DEBUG_ARGS("No abierto dir %s",dir);

	return result;
}

//Variable usada para saber lo que hemos escrito con el test sdlog_WriteLog_Massive ya que lo usamos en otros test
uint32_t totalwrite=0;

test (sdlog_WriteLog)
{
	
	assertTrue(gtKeeper.ClearLogs());
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	uint8_t numfiles= CountFilesDir("/");
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	uint8_t numfiles2= CountFilesDir("/");
	//Chequeamos que pese a grabar varios logs se graban en el mismo file
	assertTrue(numfiles==numfiles2);
}



test (sdlog_ClearLogs)
{
	//ASeguramos que al menos hay un file 
	assertTrue(gtKeeper.WriteLog("claro que hace")!=-1);
	

	//Check en SD >=1 files
	assertTrue(CountFilesDir("/")>=1);

	//Limpiamos
	assertTrue(gtKeeper.ClearLogs());

	//Check en SD 0 files
	assertTrue(CountFilesDir("/")==0);
}



test (sdlog_WriteLog_Massive)
{

 if (checkTestPass(sdlog_WriteLog) && checkTestPass(sdlog_ClearLogs)) {

	
	//Limpiamos
	assertTrue(gtKeeper.ClearLogs());

	//Check en SD 0 files
	assertTrue(CountFilesDir("/")==0);
	
	//Si lanzamos el for hasta esta cant tienen que salir 3 files de log  ((MAX_FILE_LOG_SIZE * 2 ) / 12) +3  //(12 es la longitud de 0123456789 + <CR><LF>)
	//3 kbytes

	for(int i=0;i<=(((MAX_FILE_LOG_SIZE * 2 ) / 12) +3);i++)
	{
		uint8_t writed= gtKeeper.WriteLog("0123456789");
		
		totalwrite+=writed;
		
		assertTrue(writed!=-1);
	}
	
	gtKeeper.FlushLog();

	assertTrue(CountFilesDir("/")==3);
	
	pass();
	//assertTrue(gtKeeper.ClearLogs());

	}

}



testing  (sdlog_FileSize)
{

  if (checkTestPass(sdlog_WriteLog_Massive)) {

	uint32_t size2=gtKeeper.SizeLogs();
	LOG_DEBUG_ARGS_B("totalsizes FileSize %lu - %lu",totalwrite,size2);
	//Tiene que coincidir el tamaño guardado
	assertTrue(size2==totalwrite);
	pass();
	}

}



testing  (sdlog_WriteToStream)
{


  if (checkTestPass(sdlog_WriteLog_Massive)) {

		 

			uint32_t size2=gtKeeper.SizeLogs();
	
			FakeStream mystream;
			//Ahora escribimos sobre el stream
			uint32_t  sizetostream= gtKeeper.WriteLogToStream(&mystream);
			LOG_DEBUG_ARGS_B("totalsizes WriteToStream %lu - %lu ",size2,sizetostream);
			
			//Tiene que coincidir el tamaño guardado
			assertTrue(size2==sizetostream);

			pass();
	}

}