/*
 * Init.cpp
 *
 * Created: 13/03/2018 1:17:58
 *  Author: Gabi
 */ 
 
 #include <gtkeeper.h>
 

 void wakeUpGSM()
 {
	int_input_gsm=true;
 }

 //##Estado --- Reset ---

 //CHECK
 bool GTKeeper::CheckInit()
 {
	return true;
	

 }

bool Check(bool (*checkfuntion)(),PGM_P txt ,uint8_t linenum,int delayms=0,uint8_t repeat=0)
{
	bool result=false;
	uint8_t numintentos=0;
	screenManager.PrintTextLine_P(linenum,txt, TXT_UNKNOW);	

	while (!result && repeat>=numintentos)
	{
		delay(delayms);
		result=(*checkfuntion)();
		numintentos++;
	}
 
	if (result)
		screenManager.PrintTextLine_P(linenum,txt, TXT_OK);		
	else
		screenManager.PrintTextLine_P(linenum,txt, TXT_ERROR);
	return result;

}

 //ACCION
void GTKeeper::OnInit()
{
	LOG_DEBUG("OnInit");



	error_code=NO_ERROR;
	
	bSetupCompleted=false;

	//Punto cero de todos pines ¿?
	//noInterrupts();


	//Fijamos los puertos conectados a los sectores
	//Como salidas digitales
	for(uint8_t i=0;i<PORTS_NUM;i++)
	{
		pinMode(ports[i], OUTPUT);
		APAGA_RELE(ports[i]);
	}

	//Puerto de motor
	pinMode(PORT_MOTOR_PIN, OUTPUT);
	APAGA_RELE(PORT_MOTOR_PIN);

	//Puertos de abono
	for(uint8_t i=0;i<PORTS_ABONO;i++)
	{
		pinMode(ports_abono[i], OUTPUT);
		APAGA_RELE(ports_abono[i]);
	}

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
	pinMode(LED_ERROR_MODULE_PIN, OUTPUT);
	pinMode(GSM_ONOFF_PIN, OUTPUT);

	//Pin para poner en sleep mode el modulo
	pinMode( GSM_SLEEP_PIN,OUTPUT);
	digitalWrite(GSM_SLEEP_PIN, LOW);


	//Pin interrupciones
	// Configure wake up pin as input.	// This will consumes few uA of current.
	pinMode(INTERRUPT_USER_INPUT, INPUT); // La interrupcion ocurre cuando cambia a HIGH
	pinMode(INTERRUPT_GSM_INPUT, INPUT_PULLUP); //La interrupcion ocurre cuando cambia a LOW

	

	//Enciende Screen	 
	screenManager.Encender();

	//Realiza los checks pertinentes
	
	uint8_t line_num=0;

    LOG_DEBUG("Fijando hora");
	//Chequeamos hora, para asi grabar logs coherentes
	if (gtKeeper.FijarHoraRTC())
		LOG_DEBUG("Hora fijada correctamente");
	else
		LOG_DEBUG("Error fijando hora RTC");
	 
	 //Chequeamos SD
	if (!Check([](){return gtKeeper.SDInitializate();},TXT_SD,line_num++))
	{
		LOG_DEBUG("No se pudo inicializar SD");
		error_code=ERROR_NO_SDCARD;
		screenManager.ShowMsgBox_P(PSTR("Tarjeta SD no responde. Verificar."),MsgOkButton,30);
		
	}
	



	//Cargamos la configuracion del terminal
	if (!Check([](){ return gtKeeper.EEPROMCargaConfig();},TXT_CONFIG,line_num++))
	{
		error_code=ERROR_NO_CONFIG;
		//Grabamos log¿?

		screenManager.ShowMsgBox_P(PSTR("Configuracion corrupta, ajuste manual"),MsgOkButton,30);

		//Reseteamos config
		gtKeeper.ResetConfig();
		gtKeeper.EEPROMGuardaConfig();
	

		//Salimos para que sea vuelto a llamar el estado
		return;
	}

	//Cargamos programas
	if (!Check([](){ return gtKeeper.EEPROMCargaProgramas();},TXT_PROGRAMAS,line_num++))
	{
		error_code=ERROR_NO_PROG;
		//Grabamos log
		screenManager.ShowMsgBox_P(PSTR("NO SE PUDO CARGAR LA PROGRAMACIÓN, LA PROGRAMACIÓN SERÁ RESETEADA"),MsgOkButton,30);
		//REseteamos todo, config, programas y estadisticas.
		gtKeeper.ResetProgramas();
		//Salimos para que sea vuelto a llamar el estado
		return;
	}


	//Comprobamos si tiene gsm configurado
	if (IsGSMEnable())
	{
#ifndef PROTEUS
		LOG_DEBUG("ACTIVANDO GSM");
		//Chequea si esta encendido el modulo GSM, sino lo apaga
		if (EstaArrancado())
			//apaganding........
			this->SwitchModule();

		if (Check([](){ return gtKeeper.ActivaModulo();},TXT_MOD_GSM,line_num++,50,ONINIT_REINTENTOS))
		{

			gtKeeper.getIMEI(config.Imei);
			lcd.clear();
			line_num=0;

			delay(SECONDS_DELAY(2));
			if (!Check([](){ return gtKeeper.SIMEstaLista();},TXT_SIM,line_num++,500,ONINIT_REINTENTOS))
			{
				error_code=ERROR_SIM;
				//Solicitamos confirmación
				screenManager.ShowMsgBox_P(PSTR("Tarjeta SIM no disponible"),MsgOkButton,30);
				//Grabamos log
						
			}
		}
		else
		{
				error_code=ERROR_NO_RESPOND;
				screenManager.ShowMsgBox_P(PSTR("Modulo gsm no responde"),MsgOkButton,30);
				//Grabamos log
		}
#endif
	}
	
	if (EstaArrancado())
		attachInterrupt(digitalPinToInterrupt(3),wakeUpGSM, LOW);

	//Cargamos hora
	if (!Check([](){ return gtKeeper.EstaEnHora();},TXT_RELOJ,line_num++))
	{
		delay(SECONDS_DELAY(2));
		screenManager.ShowMsgBox_P(PSTR("Reloj no responde, ajuste manual"),MsgOkButton,30);
		error_code=ERROR_NO_HORA;
	}



	//Esto hay que trasladarlo a otro estado. :)
	//Si no hemos conseguido ponerlo en hora , la solicitaremos manualmente
	if (gtKeeper.EstaEnHora())
		screenManager.SetPantallaActual((ScreenBase*)&menuScreen);
	else
		screenManager.SetPantallaActual((ScreenBase*)&fechahoraScreen);

	bSetupCompleted=true;
}




 //SALE
 void GTKeeper::OnLeaveInit()
 {
	 
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("OnLeaveInit");
	 

 }


