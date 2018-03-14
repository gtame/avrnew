
/*
 * Run.cpp
 *
 * Created: 13/03/2018 16:21:00
 *  Author: Gabi
 */ 
   #include <gtkeeper.h>

   bool GTKeeper::CheckRun()
   {
		//Comprobaremos si toca parar// lanzar 
	   return true;
   }
   
   void GTKeeper::OnRun()
   {
	   
	   ////Activamos la interrupcion para capturar pulsaciones de teclado
	   //attachInterrupt(digitalPinToInterrupt(2),wakeUpInt,HIGH);
	   ////Activamos la interrupcion para capturar llamadas/sms
	   //attachInterrupt(digitalPinToInterrupt(2),wakeUpInt,HIGH);
	   
	   LOG_DEBUG("OnRun");
	   
		//Mientras no haya UserInput ó Call ó SMS
		while(false)
		{
			
			/*
			int wakeUpPin=2;
		   // Configure wake up pin as input.
		   // This will consumes few uA of current.
		   pinMode(wakeUpPin, INPUT);
			
			// Allow wake up pin to trigger interrupt on low.
			attachInterrupt(0, wakeUp, LOW);
   
			// Enter power down state with ADC and BOD module disabled.
			// Wake up when wake up pin is low.
			LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
   
			// Disable external pin interrupt on wake up pin.
			detachInterrupt(0);
   */
			// Do something here
			// Example: Read sensor, data logging, data transmission.
			//Lo dormimos durante 1 min
			Sleep();	
			//Chequeamos programacion
			
			//Chequeamos si debemos realizar el envio Stats via Web
		}

		
   }
   
   
   void GTKeeper::OnLeaveRun()
   {
   	   LOG_DEBUG("OnLeaveRun");
   }
