/*
 * CPPFile1.cpp
 *
 * Created: 13/03/2018 1:01:44
 *  Author: Gabi
 */ 

  #include <gtkeeper.h>


//##Estado --- Reset ---

 //CHECK
 bool GTKeeper::CheckReset()
 {

	 static bool blnResetLoop=false;

	 int indexA= keypad->findInList('A');
	 int indexD= keypad->findInList('D');


	 LOG_DEBUG_B("CHECK RESET");

	 //Marca de tiempo
	 time_t time=now();

	 //Obtenemos teclas pulsadas
	 keypad->getKeys();

	 //Si pulsamos XX SEG estas dos teclas se resetearan TODAS configs. :)
	 while (blnResetLoop &&
	 (keypad->key[indexA].kstate==PRESSED || keypad->key[indexA].kstate==HOLD) &&
	 (keypad->key[indexD].kstate==PRESSED || keypad->key[indexD].kstate==HOLD)
	 )
	 {
		 if (ELAPSED_SECONDS(time)>10)
		 {
			 screenManager.ShowMsgBox_P(PSTR("Desea resetear las \nconfiguraciones?"),MsgYesNoButton, ResetConfigsCallBack);

			 while (blnResetLoop)
			 screenManager.Loop();
		 }
		 delay(500);
		 keypad->getKeys();
	 }


	 LOG_DEBUG_B("CHECK TERMINADO");

 }

 //ACCION
 void GTKeeper::OnReset()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 gtKeeper.ResetConfig();
	 gtKeeper.EEPROMGuardaConfig();
	 gtKeeper.ResetProgramas();
	 gtKeeper.ResetearEstadisticas();

	 //Flag para salir del bucle loop.
	 // blnResetLoop=false;
 }

 //SALE
 void GTKeeper::OnLeaveReset()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de Reset");

	 //Flag para salir del bucle loop.
	 // blnResetLoop=false;
 }