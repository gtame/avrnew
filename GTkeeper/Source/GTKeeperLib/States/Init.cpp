/*
 * Init.cpp
 *
 * Created: 13/03/2018 1:17:58
 *  Author: Gabi
 */ 
 
 #include <gtkeeper.h>



 //##Estado --- Reset ---

 //CHECK
 bool GTKeeper::CheckInit()
 {
	return true;
	

 }


 //ACCION
void GTKeeper::OnInit()
{
	LOG_DEBUG("OnInitializating");
	
	bSetupCompleted=false;

	//No se podia pasar el puntero a funcion dentro de una clase, requiere redefinir el puntero
	//TTPGet=GetURL;
	//ProcessResultPtr = ProcessATMensajes;

	//Fijamos los puertos conectados a los sectores
	//Como salidas digitales
	for(uint8_t i=0;i<PORTS_NUM;i++)
	{
		pinMode(ports[i], OUTPUT);
		APAGA_RELE(ports[i]);
	}

	//Puertos de abono
	for(uint8_t i=0;i<PORTS_ABONO;i++)
	{
		pinMode(ports_abono[i], OUTPUT);
		APAGA_RELE(ports_abono[i]);
	}
	//Puerto de motor
	pinMode(PORT_MOTOR, OUTPUT);
	APAGA_RELE(PORT_MOTOR);

	//Puerto comun
	pinMode(PORT_COMUN_PIN, OUTPUT);
	APAGA_RELE(PORT_COMUN_PIN);

	//Puente H
	pinMode(PORT_PUENTEH1_PIN, OUTPUT);
	pinMode(PORT_PUENTEH2_PIN, OUTPUT);
	digitalWrite(PORT_PUENTEH2_PIN, LOW);
	digitalWrite(PORT_PUENTEH2_PIN, LOW);


	//Arraay de salidas activas a 0
	for (uint8_t i = 0; i < MAX_PROGRAMAS; i++)
	{
		salidas[i].Tipo=actNone;
	}

	//Ponemos como salida los dos pines para el control
	pinMode( LED_ERROR_MODULE_PIN, OUTPUT);
	pinMode(GSM_ONOFF_PIN, OUTPUT);


	WakeUp();
	SetMode(SleepDisable);

	SetupPantallaTeclado();
}

 //SALE
 void GTKeeper::OnLeaveInit()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de Init");
 }


 //fUNCION ADICIONAL
 void GTKeeper::SetupPantallaTeclado()
 {


	 
	 uint8_t line_num=0;

	 const uint8_t NUM_INTENTOS=5;
	 



	 //Inicializamos el gestor de ventanas
	 screenManager.Initializate(lcd,20,4,keypad);
	 screenManager.Encender();


	 //Cargamos la configuracion del terminal
	 bool config_error=!gtKeeper.EEPROMCargaConfig();

	 //Inicializamos gtKeeper
	 if (!config_error)
	 screenManager.PrintTextLine_P(line_num,TXT_CONFIG, TXT_OK);
	 else
	 {
		 gtKeeper.ResetConfig();
		 screenManager.PrintTextLine_P(line_num,TXT_CONFIG, TXT_ERROR);
	 }
	 line_num++;



 

	 //Comprobamos si tiene gsm configurado
	 if (IsGSMEnable())
	 {

		 LOG_DEBUG("ACTIVANDO GSM");
		 bool blnOK=true;
		 uint8_t num_veces=0;

		 //Arranca Modulo
		 while(!(blnOK=ActivaModulo()) &&  num_veces<NUM_INTENTOS)
		 {
			 gtKeeper.setLed(LED_ERROR_MODULE_PIN);
			 delay(500);
			 num_veces++;
		 }

		 if (blnOK)
		 screenManager.PrintTextLine_P(line_num,TXT_MOD_GSM, TXT_OK);
		 else
		 screenManager.PrintTextLine_P(line_num,TXT_MOD_GSM, TXT_ERROR);

		 line_num++;


		 //SIM -- Ready¿?
		 if (blnOK)
		 {

			 num_veces=0;
			 while (!(blnOK=gtKeeper.CheckSIM())  &&  num_veces<NUM_INTENTOS)
			 {
				 setLed(LED_ERROR_SIM_PIN);
				 delay(500);
				 num_veces++;
			 }

			 if (blnOK)
			 screenManager.PrintTextLine_P(line_num,TXT_SIM, TXT_OK);
			 else

			 screenManager.PrintTextLine_P(line_num,TXT_SIM, TXT_OK);

			 line_num++;
		 }
		 else
		 LOG_DEBUG_B("GSM NO ACTIVO");



		 //Esperamos 2 seg, para que coga red..
		 delay(2000);


		 if (blnOK)
		 {

			 num_veces=0;
			 //Network
			 while (!(blnOK=gtKeeper.EstaRegistradoGSM()) &&  num_veces<NUM_INTENTOS)
			 {
				 setLed(LED_ERROR_NETWORK_PIN);
				 delay(500);
				 num_veces++;
			 }

			 if (blnOK)
				 screenManager.PrintTextLine_P(line_num,TXT_CONECTIVIDAD,TXT_OK);
			 else
				 screenManager.PrintTextLine_P(line_num,TXT_CONECTIVIDAD,TXT_ERROR);

			 line_num++;
		 }







		 //eNVIA SMS REINICIO

		 //Antes de hacer un clear de la pantalla esperamos 1 seg.


		 //Fijar Hora desde red GSM
		 if (blnOK)
		 {


			 lcd->clear();
			 line_num=0;
			 if (gtKeeper.config.AvisosSMS & SMSReset)
			 {
				 //Esperamos 2 seg SMS
				 delay(2000);

				 screenManager.PrintTextLine_P(line_num,TXT_SMS, PSTR(""));
				 //Envio mail de aviso
				 gtKeeper.SendSmsIniReinicio();
				 screenManager.PrintTextLine_P(line_num,TXT_SMS, TXT_OK);
				 line_num++;
			 }


			 num_veces=0;
			 delay(5000);
			 //Network
			 while (!(blnOK=gtKeeper.FijarHoraGSM()) &&  num_veces<NUM_INTENTOS)
			 {
				 delay(500);
				 num_veces++;
			 }



			 if (blnOK)
			 screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_OK);
			 else
			 screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_ERROR);

			 line_num++;
		 }
	 }
	 else
	 {
		 screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_ERROR);
		 line_num++;
	 }


	 if (line_num==4)
	 lcd->clear();



	 //Cargamos programas
	 if (gtKeeper.EEPROMCargaProgramas())
	 screenManager.PrintTextLine_P(line_num,TXT_PROGRAMAS, TXT_OK);
	 else
	 screenManager.PrintTextLine_P(line_num,TXT_PROGRAMAS, TXT_ERROR);


	 #ifdef PROTEUS
	 if (!gtKeeper.EstaEnHora())
	 {
		 TimeElements timeEl;
		 timeEl.Day=8;
		 timeEl.Month=4;
		 timeEl.Year=y2kYearToTm(16);
		 timeEl.Hour=23;
		 timeEl.Minute=38;

		 gtKeeper.SetHora(makeTime(timeEl));

	 }

	 screenManager.SetTimeInactivity(0);
	 #endif


	 //Avisa que ya se ha acabado la incializacion de gtkeeper
		if (this->IsGSMEnable())
			getIMEI(config.Imei);


	SetMode(SleepEnableAuto);

	bSetupCompleted=true;

	 if (strlen(gtKeeper.config.APN)==0)
	 {
		 strcpy(gtKeeper.config.APN,"gprs-service.com");
	 }

	 //Si no hemos conseguido ponerlo en hora , la solicitaremos manualmente
	 if (gtKeeper.EstaEnHora())
	 screenManager.SetPantallaActual((ScreenBase*)&menuScreen);
	 else
	 screenManager.SetPantallaActual((ScreenBase*)&fechahoraScreen);


	 if (config_error)
	 screenManager.ShowMsgBox_P(PSTR("Error configuracion\nParametros->Sistema"),MsgOkButton,NULL);



	 //Fijamos la pantalla actual..
	 if (!config_error)
	 //Ajustamos Config Web
	 gtKeeper.CargaConfigWeb();


	 //eNVIA SMS REINICIO ALL
	 if (gtKeeper.IsGSMEnable())
	 gtKeeper.SendSmsFinReinicio();


 }
