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

	 bool result=false;

	 int indexA= keypad->findInList('A');
	 int indexD= keypad->findInList('D');


	 LOG_DEBUG_B("CHECK RESET");

	 //Marca de tiempo
	 time_t time=now();

	 //Obtenemos teclas pulsadas
	 keypad->getKeys();

	 //Si pulsamos XX SEG estas dos teclas se resetearan TODAS configs. :)
	 while ( !result &&
			(keypad->key[indexA].kstate==PRESSED || keypad->key[indexA].kstate==HOLD)  &&
			(keypad->key[indexD].kstate==PRESSED || keypad->key[indexD].kstate==HOLD)
	 )
	 {	  
		 if (ELAPSED_SECONDS(time)>HOLD_TO_RESET)
		 {
			result=true;
		
		 }
		 delay(500);
		 keypad->getKeys();
	 }


	 LOG_DEBUG_B("CHECK TERMINADO");
	 
	 
	 return result;

 }

 //ACCION
 void GTKeeper::OnReset()
 {
	 static bool blnResetLoop=false;
 
	  //Solicitamos confirmación 
	  screenManager.ShowMsgBox_P(
	  PSTR("Desea resetear las \nconfiguraciones?"),
	  MsgYesNoButton,
	  [](uint8_t button,uint8_t func) {
		  if (button==0)
		  {
			  //REseteamos todo, config, programas y estadisticas.
			  gtKeeper.ResetConfig();
			  gtKeeper.EEPROMGuardaConfig();
			  gtKeeper.ResetProgramas();
			  gtKeeper.ResetearEstadisticas();
		  }
		  //Flag para salir del bucle loop.
		  blnResetLoop=false;
	  }
	  );
	 
	 //Bucle para reaccionar a eventos de pantalla
	 while (blnResetLoop)
		screenManager.Loop();

 }

 //SALE
 void GTKeeper::OnLeaveReset()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de Reset");

	 //Flag para salir del bucle loop.
	 // blnResetLoop=false;
 }
 
 
 