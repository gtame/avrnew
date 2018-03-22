/*
 * CPPFile1.cpp
 *
 * Created: 22/03/2018 1:04:05
 *  Author: Gabi
 */ 
 #include "main.h"

 testing(riegos_check)
 {
	//Ejecutamos un riego simple
	//LogTime(now());
	Riego.CheckRiegos();
	Riego.ShowInfoSalidas();
 }

 //Cuando se produce un delay imprevisto tiene que chequear que no se colo ninguno por el medio.
 test (riego_anade_simple)
 {
	time_t ahora=now();
	ahora+=60;
	Riego.programas[0].Dias=127;
	Riego.programas[0].Sector=1;

	Riego.programas[0].HoraInicio=hour(ahora);
	Riego.programas[0].MinutoInicio=minute(ahora);
	Riego.programas[0].TiempoAbono=200;
	Riego.programas[0].TiempoRiego=300;
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
 }


 

 //Cuando se produce un delay imprevisto tiene que chequear que no se colo ninguno por el medio.
 test (riego_delay_imprevisto)
 {
	 time_t ahora=now();
	 ahora+=60;
	 Riego.programas[0].Dias=127;
	 Riego.programas[0].Sector=1;

	 Riego.programas[0].HoraInicio=hour(ahora);
	 Riego.programas[0].MinutoInicio=minute(ahora);
	 Riego.programas[0].TiempoAbono=200;
	 Riego.programas[0].TiempoRiego=300;
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 Riego.ProgramaToDisplay(0,buffer_test);
	 LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
 }