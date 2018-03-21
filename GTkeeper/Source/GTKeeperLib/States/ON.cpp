/*
 * CPPFile1.cpp
 *
 * Created: 13/03/2018 17:43:09
 *  Author: Gabi
 */ 
 #include <gtkeeper.h>

 
bool GTKeeper::CheckON()
{
	return true;
}

void GTKeeper::OnON()
  {
		//Inicializamos los puertos series
		Serial.begin(9600);
		Serial1.begin(9600);
		Serial2.begin(9600);

		LOG_DEBUG("OnON");
		
	 
		//Inicializamos variables
		t_last_web=0; //Tiempo para controlar los tiempos de la web.

		//stop_abono=0;
		error_web=0;//Numero de errores que se producen al intentar acceder al dispositivo
		

		bSetupCompleted=false; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
		//bWebInProcess=false;//Flag para indicar que estamos actualmente refrescando desde la web
		bpendingWeb=false;//Flag para indicar que es necesario el update de web
		//bRebootSIM=false;//Flag para indicar que hemos reiniciado el modulo GSM, y necesitamos reconfigurarlo
		
		//salidas_web=0; //Numero de salidas pendientes de ser enviadas x WEB

		//Limpiamos buffer
		memset(buffer,0,MAIN_BUFFER_SIZE);
		memset(buff_parse,0,MAIN_BUFFER_PARSE);

		configuracion->ResetConfig();


		Wire.begin();//Necesario para comunicacion I2C
#ifdef PANTALLA_TECLADO
		//Inicializacion LCD
		lcd.init();
		//Inicializamos el gestor de ventanas
		screenManager.Initializate(&lcd,20,4,&keypad);
#endif
	 
  }
	 
	  
	  
 void GTKeeper::OnLeaveON()
 {
	LOG_DEBUG("OnLeaveOn");

 /*
	
	 */
	 //Callbacks o punteros a funcion
	 //ProcessResultPtr = ProcessATMensajesCallback;
	 //gtKeeper.ChangeStatus=setLed;

	 
 }