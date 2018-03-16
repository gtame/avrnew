
/*
 * Run.cpp
 *
 * Created: 13/03/2018 16:21:00
 *  Author: Gabi
 */ 
   #include <gtkeeper.h>

   bool GTKeeper::CheckRun()
   {
 	 //Si hemos arrancado, no toca tecla usuario, ni recibimos SMS ó Calls
	 return !CheckUser() && !CheckSMS() && !CheckCall();
   }

   
   
   void wakeUpUser()
   {
     int_input_user=true;
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
		while(!int_input_user && !int_input_gsm)
		{
			//LOG_DEBUG("A DORMIR COOO!");
			// Enter power down state with ADC and BOD module disabled.
			// Wake up when wake up pin is low.SLEEP_8S
			//SLEEP_FOREVER
			//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
			GTKEEPER_SLEEP(SLEEP_8S);
			// Do something here
			// Example: Read sensor, data logging, data transmission.
			//Lo dormimos durante 1 min
			//Sleep();	
			//LOG_DEBUG("DESPIERTAAAAAAAAAAAAAAAAA COOO!");
			//Chequeamos programacion
			//Chequeamos si debemos realizar el envio Stats via Web

		
		}
		// Disable external pin interrupt on wake up pin.
		detachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT));
		
   }
   
   
   void GTKeeper::OnLeaveRun()
   {
   	   LOG_DEBUG("OnLeaveRun");
   }
