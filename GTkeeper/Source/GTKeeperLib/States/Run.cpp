 
/*
 * Run.cpp
 *
 * Created: 13/03/2018 16:21:00
 *  Author: Gabi
 */ 
   #include <gtkeeper.h>



    //Interrupcion lanzada cuando el usuario pulsa una tecla y esta en modo Run 
	//PAra que despierte
    void wakeUpUser()
    {
	    int_input_user=true;
    }

	//Chequea si vamos a realizar el envio pendiente Web
 

   

	bool GTKeeper::CheckRun()
	{
		//Si hemos arrancado, no toca tecla usuario, ni recibimos SMS ó Calls
		return !CheckUser() && !CheckSMS() && !CheckCall();
	}


   void GTKeeper::OnRun()
   {
	   
	   ////Activamos la interrupcion para capturar pulsaciones de teclado
	   //attachInterrupt(digitalPinToInterrupt(2),wakeUp,HIGH);
	   ////Activamos la interrupcion para capturar llamadas/sms
	   //attachInterrupt(digitalPinToInterrupt(2),wakeUp,HIGH);
	   
	   LOG_DEBUG("OnRun");

	   	// Allow wake up pin to trigger interrupt on low.
	   	attachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT), wakeUpUser, HIGH);
		

	   int_input_user=false;
	   int_input_gsm=false;
		//Mientras no haya UserInput ó Call ó SMS
		while(!int_input_user && !int_input_gsm && !CheckWeb())
		{

			//Dormimos procesador y modulo GSM
			GTKEEPER_SLEEP(SLEEP_8S);
			
			//Chequeamos programacion
			riegos->CheckRiegos();

			//Chequeamos si es necesario guardar estadisticas 
			if (ELAPSED_SECONDS(Riego.GetLastEstadisticasSaved())>SECS_PER_HOUR)
					riegos->GuardarEstadisticasEEPROM();

		}
		// Disable external pin interrupt on wake up pin.
		detachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT));
		
   }
   
   
   void GTKeeper::OnLeaveRun()
   {
   	   LOG_DEBUG("OnLeaveRun");
   }
