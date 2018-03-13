/*
 * CPPFile1.cpp
 *
 * Created: 13/03/2018 1:30:22
 *  Author: Gabi
 */ 
 //##Estado --- Error ---
  
  #include <gtkeeper.h>

 //CHECK
 bool GTKeeper::CheckError()
 {
	  LOG_DEBUG("CHECK ERROR");
	//Hay error¿?
	 LOG_DEBUG("CHECK ERROR TERMINADO");
	 return false;
 }

 //ACCION
 void GTKeeper::OnError()
 {
	  //Que hacemos en caso de error
	  //Mostramos al usua¿?

 }

 //SALE
 void GTKeeper::OnLeaveError()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de Error");

	 //Flag para salir del bucle loop.
	 // blnResetLoop=false;
 }