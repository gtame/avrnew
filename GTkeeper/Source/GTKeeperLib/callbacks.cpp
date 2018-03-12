/*
 * callbacks.cpp
 *
 * Created: 12/03/2018 21:01:11
 *  Author: Gabi
 */ 

 #include "callbacks.h"

 bool ProcessATMensajesCallback(char * msg)
 {
	 gtKeeper.ProcessATMensajes(msg);
 }

 void ResetConfigsCallBack(uint8_t button,uint8_t func)
 {
	 if (button==0)
	 {
		 //REseteamos todo, config, programas y estadisticas.
		 gtKeeper.ResetConfig();
		 gtKeeper.EEPROMGuardaConfig();
		 gtKeeper.ResetProgramas();
		 gtKeeper.ResetearEstadisticas();
	 }
	 //Flag para salir del bucle loop.
	// blnResetLoop=false;
 }