#include "gtkeepertest.h"

/*
 * gtkeeper.c
 *
 * Created: 12/03/2018 20:31:21
 *  Author: Gabi
 */ 
 void SDCheck()
 {
	 //Flag indicador si se inicializo la sd previamente
	 static bool initializatedSD=false;

	 if (!initializatedSD)
	 {
		 if (!SD.begin(SPI_QUARTER_SPEED, SD_CHIP_SELECT_PIN))
		 {
			 
			 LOG_DEBUG("No se pudo inicializar SD");
			 return;
		 }
	 }
	 
	 initializatedSD=true;

	 //Si existe borramos fichero
	 if (SD.exists("datalog.txt"))
	 SD.remove("datalog.txt");
	 

	 //Op con directorios
	 //// Crear un directorio
	 SD.mkdir("test");
	 // Eliminar un directorio
	 SD.rmdir("test");
	 
	 
	 // Abrir archivo y escribir valor
	 File  logFile = SD.open("datalog.txt", FILE_WRITE);
	 
	 if (logFile) {
		 int value = 22;
		 logFile.print("Time(ms)=");
		 logFile.print(millis());
		 logFile.print(", value=");
		 logFile.println(value);
		 
		 logFile.close();
		 LOG_DEBUG("Grabado log..");
	 }
	 else {
		 LOG_DEBUG("Error al abrir el archivo");
	 }
	 
	 // Abrir fichero y mostrar el resultado
	 File dataFile = SD.open("datalog.txt");
	 if (dataFile)
	 {
		 int dataLine;
		 while (dataFile.available())
		 {
			 dataLine = dataFile.read();
		 }
		 LOG_DEBUG("Leido archivo");
		 dataFile.close();
	 }
	 else
	 {
		 LOG_DEBUG("Error al abrir el archivo");
	 }

 }

