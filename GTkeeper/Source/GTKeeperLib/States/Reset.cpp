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
	 
	 LOG_DEBUG("CheckReset");
	 bool result=false;
	 return false;

	 int indexA= keypad.findInList('A');
	 int indexD= keypad.findInList('D');


	 

	 //Marca de tiempo
	 time_t time=now();

	 //Obtenemos teclas pulsadas
	 keypad.getKeys();

	 //Si pulsamos XX SEG estas dos teclas se resetearan TODAS configs. :)
	 while ( !result &&
			(keypad.key[indexA].kstate==PRESSED || keypad.key[indexA].kstate==HOLD)  &&
			(keypad.key[indexD].kstate==PRESSED || keypad.key[indexD].kstate==HOLD)
	 )
	 {	  
		 if (ELAPSED_SECONDS(time)>HOLD_TO_RESET)
		 {
			result=true;
		
		 }
		 delay(500);
		 keypad.getKeys();
	 }


	 LOG_DEBUG_B("CHECK TERMINADO");
	 
	 
	 return result;

 }

 //ACCION
 void GTKeeper::OnReset()
 {

	//Enciende Screen
	screenManager.Encender();

	if (screenManager.ShowMsgBox_P(PSTR("Resetear todas configuraciones?"), MsgYesNoButton)==0)
	{
		riegos->ResetProgramas();
		riegos->ResetearEstadisticas();
		configuracion->ResetConfig();
	}
	screenManager.Apagar();
 }

 //SALE
 void GTKeeper::OnLeaveReset()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("OnLeaveReset");

	 //Flag para salir del bucle loop.
	 // blnResetLoop=false;
 }
 
 
 