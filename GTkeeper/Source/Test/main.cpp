 
#include "main.h"
 
GTKeeper gtKeeper;  
Stream *sserial=&Serial;

test(correct)
{
	int x=1;
	assertEqual(x,1);
}

test(incorrect)
{
	int x=1;
	assertNotEqual(x,1);
}

test(otro)
{
	int x=1;
	assertNotEqual(x,1);
}

test(executeCommand)
{
	strcpy(gtKeeper.config.PasswordSMS,"1111");
	char str[]="#1111#01";
	
	assertTrue(gtKeeper.ExecuteCommand(str));
}





void CargaProgramas()
{
	Programa prog= gtKeeper.programas[0];

			prog.Dias=L; //Dias en los que se debe ejecutar
			prog.HoraInicio=8;//Cuando debe ejecutarse , tiene dos variantes
			prog.MinutoInicio=50;//99:XX --> Indicara que se lanze cuando termine el programa XX
			  	  	  	  	  	   //HH:MM --> Indicara la hora a la que se lanzara si el dia esta marcado
			prog.TiempoRiego=1200; //Tiempo de riego , en segundos
			prog.TiempoAbono=60; //Tiempo de abono,  en segundos

			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);

			LOG_DEBUG_ARGS("PROGrama a guardar %i->%s",0,buffer_test);

			prog.Sector=40;
			gtKeeper.GrabarProgramaAEEPROM(0,&prog);
			prog.Sector=1;
			gtKeeper.GrabarProgramaAEEPROM(1,&prog);
			prog.Sector=2;
			gtKeeper.GrabarProgramaAEEPROM(2,&prog);
			prog.Sector=3;
			gtKeeper.GrabarProgramaAEEPROM(3,&prog);

			gtKeeper.CargarProgramaDesdeEEPROM(0,&prog);
			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);
			LOG_DEBUG_ARGS("Programa leido %i->%s",0,buffer_test);


			gtKeeper.CargarProgramaDesdeEEPROM(1,&prog);
			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);
			LOG_DEBUG_ARGS("Programa leido %i->%s",0,buffer_test);

			gtKeeper.CargarProgramaDesdeEEPROM(2,&prog);
			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);
			LOG_DEBUG_ARGS("Programa leido %i->%s",0,buffer_test);

			gtKeeper.CargarProgramaDesdeEEPROM(3,&prog);
			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);
			LOG_DEBUG_ARGS("Programa leido %i->%s",0,buffer_test);


			gtKeeper.CargarProgramaDesdeEEPROM(10,&prog);
			memset(buffer_test,0,MAIN_BUFFER_SIZE);
			gtKeeper.ProgramaToString(buffer_test,&prog);
			LOG_DEBUG_ARGS("Programa leido %i->%s",0,buffer_test);
}

void CheckWeb()
{
	gtKeeper.CheckWeb();
}


void _print2digits(int number) {
	if (number >= 0 && number < 10) {
		Serial.write('0');
	}
	Serial.print(number);
}

void RtcCheck()
{
 
 tmElements_t tm;

 
  if (RTC.read(tm)) {
		   
	  Serial.print("Ok, Time = ");
	  _print2digits(tm.Hour);
	  Serial.write(':');
	  _print2digits(tm.Minute);
	  Serial.write(':');
	  _print2digits(tm.Second);
	  Serial.print(", Date (D/M/Y) = ");
	  Serial.print(tm.Day);
	  Serial.write('/');
	  Serial.print(tm.Month);
	  Serial.write('/');
	  Serial.print(tmYearToCalendar(tm.Year));
	  Serial.println();
	  } else {
	  if (RTC.chipPresent()) {
		  LOG_DEBUG("The DS1307 is stopped.  Please run the SetTime");
		  LOG_DEBUG("example to initialize the time and begin running.");
		   // get the date and time the compiler was run
		  
		   
		  } else {
		  LOG_DEBUG("DS1307 read error!  Please check the circuitry.");
	  }
	  delay(9000);

	  ///Si queremos poner fecha ..
	  return;
	  tm.Year=CalendarYrToTm(2017);
	  tm.Month=12;
	  tm.Day=12;
	  tm.Hour=12;
	  tm.Minute=12;
	  tm.Second=12;
	  // and configure the RTC with this info
	  if (RTC.write(tm)) {
		  LOG_DEBUG("Configurado rtc!!");
	  }
  }

}



void ListarSIM()
{
	 char buffname[25];
	 char buffnum[25];
	 uint8_t contacto=1;

	 memset(buffname,0,25);
	 memset(buffnum,0,25);
/*
	 while (Sim900.GetSIMContact(contacto,buffname,buffnum))
	 {


		 LOG_DEBUG_ARGS("%i -> %s  Num: %s",contacto,buffname,buffnum);

		 memset(buffname,0,25);
		 memset(buffnum,0,25);


		 contacto++;
	 }

*/
}


void CargaDesdeSIM()
{
	gtKeeper.CargaConfigDesdeSim();
	gtKeeper.ShowConfigInfo();
}


void ShowConfigInfo()
{
	gtKeeper.ShowConfigInfo();
}

void Config4String()
{
	gtKeeper.CargaConfigDesdeString("12665331679912341215");
	ShowConfigInfo();
	gtKeeper.EEPROMGuardaConfig();
}

void Config2EEPROM()
{
	gtKeeper.EEPROMGuardaConfig();
}

void Config4EEPROM()
{
	gtKeeper.EEPROMCargaConfig();
}

void Config4Web()
{
	gtKeeper.CheckWebConfig();
	ShowConfigInfo();
}

void ShowInfoProgramas()
{
	gtKeeper.ShowInfoProgramas();
}

void CheckReles()
{
	//Rele
	ENCIENDE_RELE(PORT_COMUN_PIN);
	delay(2000);
	APAGA_RELE(PORT_COMUN_PIN);


	//Puente
	ENCIENDE_RELE(PORT_MOTOR_PIN);
	delay(2000);
	APAGA_RELE(PORT_MOTOR_PIN);


	//Sectores

	//for	(uint8_t i=0;i<PORTS_NUM;i++)
	for	(uint8_t i=0;i<gtKeeper.config.numpuertos;i++)
	{
		ENCIENDE_RELE(gtKeeper.ports[i]);
		delay(2000);
		APAGA_RELE(gtKeeper.ports[i]);
	}

	//Abonos
	//for	(uint8_t i=0;i<PORTS_ABONO;i++)
	for	(uint8_t i=0;i<gtKeeper.config.numabono;i++)
	{
		ENCIENDE_RELE(gtKeeper.ports_abono[i]);
		delay(2000);
		APAGA_RELE(gtKeeper.ports_abono[i]);
	}
}

void AbreCierraValvula()
{
	gtKeeper.AbrirValvulaLatch(1);
	delay(5000);
	gtKeeper.CerrarValvulaLatch(1);
}

void LanzaRiego()
{
	//Si quisieramos hacerlo por reloj..
	/*
	TimeElements time;
	Sim900.GetTime(time);
	gtKeeper.programas[0].HoraInicio=time.Hour;
	gtKeeper.programas[0].MinutoInicio=time.Minute+1;
	*/
	gtKeeper.LanzaRiego(0,false);
}

void ShowInfoSalidas()
{
	gtKeeper.ShowInfoSalidas();
}
 



void AddTest(char * menu,void (*Test)())
{
	strncpy(tests[contador].text,menu,25);
	tests[contador].Test=Test;
	contador++;
}

void ExecuteTest(uint8_t test)
{
	if (test-1<contador)
		tests[test-1].Test();
	else
		LOG_DEBUG_ARGS("Comando no encontrado %s!!!!",buffercommand);

}

void PrintMenu()
{
LOG_DEBUG_B("===========TEST MENU ================");
for(uint8_t i=0;i<contador;i++)
	 LOG_DEBUG_ARGS_B("%i - %s",i+1,tests[i].text);
}




int main() {

	//Necesario para la inicializacion de arduino
	init();

	//Setup
	setup();


	 
	//Loop
	while (true)
	loop();

}

void setup()
{

	//Inicializamos puerto serie
	Serial.begin(9600);
	while(!Serial)

	Serial1.begin(9600);
	while(!Serial1)

	LOG_DEBUG("Inicializando");
	
	const uint8_t NUM_INTENTOS=25;
	bool blnOk =false;

	//Programas tests
	AddTest("Carga programas",CargaProgramas);
	AddTest("Carga Prog desde Web",CheckWeb);
	AddTest("Lanza Riego",LanzaRiego);
	AddTest("ShowSalidasActivas",ShowInfoSalidas);
	AddTest("ShowProgramas",ShowInfoProgramas);

	//SIM testss
	AddTest("Listar  SIM",ListarSIM);
	AddTest("Carga config desde SIM",CargaDesdeSIM);


	//Valvula TEsts..
	AddTest("Check Reles",CheckReles);
	AddTest("Valv AbreCierra",AbreCierraValvula);




	//Configuracion
	AddTest("ShowConfig",ShowConfigInfo);
	AddTest("EEPROMGuardarConfig",Config2EEPROM);
	AddTest("EEPROMCargarConfig",Config4EEPROM);
	AddTest("ConfigDesdeString",Config4String);
	AddTest("ConfigDesdeWeb",Config4Web);


	AddTest("SD Test",SDCheck);
	AddTest("RTC Test",RtcCheck);


	

	 

 
	//Entorno de TEST
	/*gtKeeper.Initializate();



	 //Arranca Modulo
	uint8_t num_veces=0;
	 while(!Sim900.ActivaModulo() &&  num_veces<NUM_INTENTOS)
	 {
		 gtKeeper.setLed(LED_ERROR_MODULE_PIN);

		 num_veces++;


	 }

	 //SIM disponible
	 num_veces=0;
	 while (!(blnOk=gtKeeper.CheckSIM())  &&  num_veces<NUM_INTENTOS)
	 {
		 gtKeeper.setLed(LED_ERROR_SIM_PIN);
		 delay(500);
		 num_veces++;
	 }


	 //Cargamos la configuracion desde la EEproom
	 if (!(gtKeeper.EEPROMCargaConfig()))
	 {
		 gtKeeper.ResetConfig();

		 //Cargamos la configuracion desde la SIM si estamos presionando el boton XXX
		 while (!gtKeeper.CargaConfigDesdeSim()  &&  num_veces<NUM_INTENTOS)
		 {
			 gtKeeper.setLed(LED_ERROR_SIM_PIN);
			 delay(500);
			 num_veces++;
		 }
		 gtKeeper.EEPROMGuardaConfig();
	 }


	 gtKeeper.ShowConfigInfo();

	//Network
	 num_veces=0;
	 while (!(blnOk=gtKeeper.EstaRegistradoGSM()) &&  num_veces<NUM_INTENTOS)
	 {
		 gtKeeper.setLed(LED_ERROR_NETWORK_PIN);
		 delay(500);
		 num_veces++;
	 }

	 //Configuramos el modem para 3G desde los datos que hemos cargado desde la SIM
	 gtKeeper.CargaConfigWeb();

	 //Cargamos los programas desde la eeprom
	 gtKeeper.EEPROMCargaProgramas();



	 LOG_DEBUG("Fijando la hora...");
	 //Fija Hora
	 //delay(10000);
	 num_veces=0;
	 while(!gtKeeper.FijarHoraGSM() &&  num_veces<NUM_INTENTOS)
	 {
		 gtKeeper.setLed(LED_ERROR_MODULE_PIN);
		 delay(500);
		 num_veces++;
	 }




	 
	 gtKeeper.EEPROMCargaProgramas();

	 strcpy(gtKeeper.config.MovilAviso ,"653316799");
	 strcpy(gtKeeper.config.APN ,"gprs-service.com");
	 gtKeeper.config.GSMAvailable=true;
	 gtKeeper.CargaConfigWeb();
	 



	if (gtKeeper.IsGSMEnable())
			 Sim900.ActivaModulo();

	gtKeeper.EndInitializate();*/
}

void loop() //----( LOOP: RUNS CONSTANTLY )----
{
	//Nos aseguramos que los test automaticos se ejecutan
	int i=0;
	while (i<10)
	{
		//Test automaticos
		Test::run();
		i++;
	}
	
	return;
	
	uint8_t index=0;
	bool blnContinue=true;

	memset(buffercommand,0,sizeof(buffercommand));
	PrintMenu();

	time_t lastgtLoop=0;
	while (blnContinue)
	{


		while(sserial->available()==0)
		{

			//Check Riegos
		 	if (gtKeeper.EstaEnHora())
				gtKeeper.CheckRiegos(!SCREEN_ACTIVE());




		 	//Chequeos internos de gtKeeper(Comprobaciones web , y otros)
			//Si no esta activa la consola no se testea web mas alla del process
			if ( ELAPSED_SECONDS(lastgtLoop)>60 )
			{

				//LOG_DEBUG("INI GTKEEPER LOOP");
					//Por si acaso mandamos un at-comand para sacarlo del sleep mode
					//Sim900.WakeUp();

					gtKeeper.Loop();
					lastgtLoop=now();
				//	LOG_DEBUG("FIN GTKEEPER LOOP");

					//LOG_DEBUG_ARGS("Cobertura %i",Sim900.GetCobertura());
			}

 
		}

		char c=(char) sserial->read();

		if (c!=-1)
		{
			//LOG_DEBUG_ARGS("Leido %i %c",c,c);

			if (c==13)
				blnContinue=false;
			else
				buffercommand[index]=c;
			index++;

			sserial->print(c);
		}

		//LOG_DEBUG_ARGS("Command %s",buffercommand);
	}


	LOG_DEBUG("\n===INICIO TEST==");

	ExecuteTest(atoi(buffercommand));


	LOG_DEBUG("\n\n==FIN TEST=====\n\n");

}
 


 