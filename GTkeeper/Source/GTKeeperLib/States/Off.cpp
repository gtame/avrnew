/*
 * Off.cpp
 *
 * Created: 13/03/2018 1:25:12
 *  Author: Gabi
 */ 

  #include <gtkeeper.h>



 bool GTKeeper::CheckOff()
 {
		return false;
 }
 
  void GTKeeper::OnOff()
  {
	  LOG_DEBUG("OnOff");
	  
	  /*
	  //Apaga lcd
	  lcd->off();
	  
	  if (IsGSMEnable())
	  {
		//Desconecta GSM  
		  
	  }
	  
	  
	  LOG_DEBUG("Desconectando....");
	   LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

	   */
	  //Desconectara todo rele activo
	  
	  
	  //Dejara Ard. en Sleep mode KO.
	  
	  
}
	  
	  
 void GTKeeper::OnLeaveOff()
 {
	LOG_DEBUG("OnLeaveOff");

 /*
	 //Inicializacion para I2C
	 //Esta llamada es necesaria pq sino da un error el linker en la lib LiquidCrystal_I2C
	 //./LiquidCrystal_I2C.cpp:41: undefined reference to `Wire'
	 Wire.begin();
	 //Inicializacion LCD
	 lcd->init();
	 */
	 //Callbacks o punteros a funcion
	 //ProcessResultPtr = ProcessATMensajesCallback;
	 //gtKeeper.ChangeStatus=setLed;

	 
 }

