#include "gtkeeper.h"


/*
 * gtkeeper.c
 *
 *  Created on: 12 de ene. de 2016
 *      Author: gtame
 */


GTKeeper gtKeeper;//Clase de logica BBL

//OJO el puerto 9 debe ser reservado para arrancar el modulo GSM
const uint8_t GTKeeper::ports[PORTS_NUM]= {PORT_SECTOR1_PIN };//,PORT_SECTOR2_PIN,PORT_SECTOR3_PIN};//{224,25,26,27,28,29,30,31,32,33,34,35,36,37,38 } ;
const uint8_t GTKeeper::ports_abono[PORTS_ABONO]= {PORT_ABONO1_PIN,PORT_ABONO2_PIN } ;


GTKeeper::GTKeeper()
{
	//hora_actual=0;
	t_last_web=0; //Tiempo para controlar los tiempos de la web.
	last_RiegosCheck =0;
	stop_abono=0;
	error_web=0;//Numero de errores que se producen al intentar acceder al dispositivo

	salidas_activas=0;//Al arrancar no hay salidas activas ;)

	bSetupCompleted=false; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
	bWebInProcess=false;//Flag para indicar que estamos actualmente refrescando desde la web
	bpendingWeb=false;//Flag para indicar que es necesario el update de web
	bRebootSIM=false;//Flag para indicar que hemos reiniciado el modulo GSM, y necesitamos reconfigurarlo
	isHourSet=false;//Flag para saber si se ha fijado la hora


	salidas_web=0; //Numero de salidas pendientes de ser enviadas x WEB

	memset(buffer,0,MAIN_BUFFER_SIZE);
	memset(buff_parse,0,MAIN_BUFFER_PARSE);

	ResetConfig();


}

GTKeeper::~GTKeeper()
{

}

char * GTKeeper::PBB (const __FlashStringHelper * p1,...)
{

	va_list ap;
	va_start(ap,p1);
	memset(buffer,0,MAIN_BUFFER_SIZE);
	vsnprintf_P(buffer, MAIN_BUFFER_SIZE,(char*)p1, ap);
	va_end(ap);
	return buffer;
}

void GTKeeper::OnChangeStatus(uint8_t status)
{
	if(ChangeStatus!=NULL)
		ChangeStatus(status);
}


void GTKeeper::EndInitializate()
{
	if (this->IsGSMEnable())
		Sim900.getIMEI(config.Imei);


	Sim900.SetMode(SleepEnableAuto);

	//Comprobamos bloqueo del sistema�? ... de momento no



	bSetupCompleted=true;


}

void GTKeeper::ShowConfigInfo()
{
	LOG_INFO_ARGS("Imei %s",config.Imei);
	LOG_INFO_ARGS("Movil %s",config.MovilAviso);
	LOG_INFO_ARGS("PwdSMS %s",config.PasswordSMS);
	LOG_INFO_ARGS("APN %s",config.APN);
	LOG_INFO_ARGS("PwdAPN %s",config.userAPN);
	LOG_INFO_ARGS("UserAPN %s",config.pwdAPN);




	LOG_INFO_ARGS("AvisoSMS %i",config.AvisosSMS);
	LOG_INFO_ARGS("\tReset %s",(config.AvisosSMS & SMSReset?"si":"no"));
	LOG_INFO_ARGS("\tInicio %s",(config.AvisosSMS & SMSInicioSector?"si":"no"));
	LOG_INFO_ARGS("\tFin %s",(config.AvisosSMS & SMSFinSector?"si":"no"));
	LOG_INFO_ARGS("\tProgramacion %s",(config.AvisosSMS & SMSProgramacionWeb?"si":"no"));



	LOG_INFO_ARGS("MotorDiesel %i",config.motor_diesel);
	LOG_INFO_ARGS("NumAbono %i",config.numabono);
	LOG_INFO_ARGS("NumPuertos %i",config.numpuertos);
	LOG_INFO_ARGS("GSM %s",config.GSMAvailable?"si":"no");


}

void GTKeeper::Initializate()
{


	bSetupCompleted=false;

	//No se podia pasar el puntero a funcion dentro de una clase, requiere redefinir el puntero
	//Sim900.Sim900HTTPGet=GetURL;
	//Sim900.ProcessResultPtr = ProcessATMensajes;





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

	//Puerto pu
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


	Sim900.WakeUp();
	Sim900.SetMode(SleepDisable);
}


//03101010120001200000
//03->Sector
//0101010-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Ejecucion * Hora que se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
bool GTKeeper::CargaProgramaDesdeString(char *programa,Programa* tprograma)
{
	char tmp [4];


	LOG_DEBUG_ARGS("CArga desde string %s",programa);

	//01127125000010000
	if (strlen(programa)==LEN_PROGRAMA_STRING)
	{
		  memset(tmp,0,4);
    	  strncpy (tmp,programa,2);
		  tprograma->Sector=atoi(tmp);


		  //Ejecucion de programa -- Hora de inicio o sector
		  memset(tmp,0,4);
		  strncpy(tmp,programa+2,3);
		  tprograma->Dias= atoi(tmp);

		  //Ejecucion de programa -- Hora de inicio o sector
		  memset(tmp,0,4);
		  strncpy(tmp,programa+5,2);
		  tprograma->HoraInicio= atoi(tmp);

		  memset(tmp,0,4);
		  strncpy(tmp,programa+7,2);
		  tprograma->MinutoInicio=atoi(tmp);


		  //Tiempo riego
		  memset(tmp,0,4);
		  strncpy(tmp,programa+9,2);
		  tprograma->TiempoRiego= atoi(tmp) * SECS_PER_HOUR;

		  memset(tmp,0,4);
		  strncpy(tmp,programa+11,2);
		  tprograma->TiempoRiego+=atoi(tmp) * SECS_PER_MIN;


		  //Tiempo abono
		   memset(tmp,0,4);
		   strncpy(tmp,programa+13,2);
		   tprograma->TiempoAbono=atoi(tmp) * SECS_PER_HOUR;; //Tiempo de abono

		   memset(tmp,0,4);
		   strncpy(tmp,programa+15,2);
		   tprograma->TiempoAbono+=atoi(tmp) * SECS_PER_MIN; //Tiempo de abono



		  return true;
	}
	else
		return false;

}




bool GTKeeper::GrabarProgramaAEEPROM(uint8_t posicion,Programa* programa)
{
		memset(buffer,0, MAIN_BUFFER_SIZE);

		//memset(buff_parse,0,MAIN_BUFFER_PARSE);

		//strcpy_P(buff_parse,(char *)F("%02i%03i%02i%02i%02i%02i%02i%02i"));

		snprintf_P(buffer,
				MAIN_BUFFER_SIZE,
				PSTR("%02i%03i%02i%02i%02i%02i%02i%02i"),
				programa->Sector,
				programa->Dias,
				programa->HoraInicio,
				programa->MinutoInicio,
				hour(programa->TiempoRiego),
				minute(programa->TiempoRiego),
				hour(programa->TiempoAbono),
				minute(programa->TiempoAbono));


		if (strlen(buffer)!=LEN_PROGRAMA_STRING)
		{
			LOG_CRITICAL("GTK-1");
			return false;
		}
		else
		{


			uint16_t ee_addr=GET_ADDRES_PROGRAM(posicion);

		  	char *buffer_reader=buffer+(LEN_PROGRAMA_STRING+1);
			//Ahora lo que vamos a hacer es comparar con lo que hay guardado en la eeprom
			eeprom_read_block((void *)buffer_reader,(void *)ee_addr,LEN_PROGRAMA_STRING);

			//Solo guardo cuando sean iguales
			if (strcmp(buffer,buffer_reader)!=0)
			{
				LOG_DEBUG_ARGS("Guardando.. %s",buffer);
				 while (!eeprom_is_ready());

				eeprom_write_block((void *)buffer,(void *)ee_addr,LEN_PROGRAMA_STRING);
			}

			return true;
		}
}


bool GTKeeper::CargarProgramaDesdeEEPROM(uint8_t posicion,Programa* programa)
{
	memset(buffer,0,MAIN_BUFFER_SIZE);
	uint16_t ee_addr=GET_ADDRES_PROGRAM(posicion);
	eeprom_read_block((void *)buffer,(void *)ee_addr,LEN_PROGRAMA_STRING);

	if (strlen(buffer)==0)
	{
		ResetPrograma(programa);
		return true;
	}
	else
	{
		LOG_DEBUG_ARGS("Cargando.. %s",buffer);
		return  CargaProgramaDesdeString(buffer,programa);
	}

}


bool GTKeeper::EEPROMCargaProgramas()
{
	bool bresult=true;
		for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
		{
			if (CargarProgramaDesdeEEPROM(program,&programas[program]))
			{

				memset(buffer,0,MAIN_BUFFER_SIZE);
				ProgramaToString(buffer,&programas[program]);
			}
			else
				bresult=false;//LOG_ERROR_ARGS("Cargando programa %i",program);
		}
}


void GTKeeper::ResetProgramas()
{
		for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
		 {
			ResetPrograma(&gtKeeper.programas[program]);
			GrabarProgramaAEEPROM(program,&programas[program]);
		 }
}


void GTKeeper::ResetPrograma(Programa* programa)
{
					programa->Dias=0;
					programa->Sector=0;
					programa->HoraInicio=0;
					programa->MinutoInicio=0;
					programa->TiempoAbono=0;
					programa->TiempoRiego=0;
}


void GTKeeper::ShowInfoProgramas()
{
	for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
		 {
			memset(buffer,0,MAIN_BUFFER_SIZE);
			ProgramaToString(buffer,&programas[program]);
			LOG_DEBUG_ARGS("%i->%s",program,buffer);
		 }
}


void GTKeeper::ProgramaToString(char *text,Programa *programa) {

	if (programa->HoraInicio == 88) {
		sprintf_P(text, PSTR("S%02i SA:%02i R%02lu:%02i A%02lu:%02i"), programa->Sector,programa->MinutoInicio,NUMERO_HORAS(programa->TiempoRiego),minute(programa->TiempoRiego),NUMERO_HORAS(programa->TiempoAbono),minute(programa->TiempoAbono));

	} else {

		DiasSemana dias = (DiasSemana)programa->Dias;

		//Tenemos que ver para que dias de la semana esta configurado que salte. ;>
		char dias_select[8];
		dias_select[0] = ((dias & L) ? 'L' : '_');
		dias_select[1] = ((dias & M) ? 'M' : '_');
		dias_select[2] = ((dias & X) ? 'X' : '_');
		dias_select[3] = ((dias & J) ? 'J' : '_');
		dias_select[4] = ((dias & V) ? 'V' : '_');
		dias_select[5] = ((dias & S) ? 'S' : '_');
		dias_select[6] = ((dias & D) ? 'D' : '_');
		dias_select[7]=0;

		sprintf_P(text, PSTR("S%02i  %s H%02i:%02i R%02lu:%02i A%02lu:%02i"),
				programa->Sector, dias_select,programa->HoraInicio,programa->MinutoInicio,NUMERO_HORAS(programa->TiempoRiego),minute(programa->TiempoRiego),NUMERO_HORAS(programa->TiempoAbono),minute(programa->TiempoAbono));
	}


}


//Si la SIM esta lista, y es la primera vez
//verificamos que haya un contacto al que llamar
bool GTKeeper::CheckSIM()
{
	//Ya no comprobamos si existe un contacto
	return Sim900.SIMEstaLista();
	/*
	static bool firstCheck=true;
	bool result=Sim900.SIMEstaLista();
	if (result && firstCheck)
	{
		if (Sim900.ExisteContactoSIM(1))
			firstCheck=false;
		else
			result=false;
	}
	return result;
	*/
}


void GTKeeper::CheckRiegos(bool sendWeb)
{
	//Comprobamos si hay que lanzar-parar algun riego (Lo hacemos cuando cambia cada minuto)
	time_t current_minute=now();
	if (last_RiegosCheck==0 ||
		(
		minute(last_RiegosCheck) != minute(current_minute) ||
		hour (last_RiegosCheck) != hour(current_minute)
		)
	)
	{


		current_minute=GetTimeWithoutSeconds(current_minute);



		//Si cambia el minuto registramos estadisticas
		RegistrarEstadisticas();

		//actualizamos la hora de arduino
		//time_t hora_actual=Sim900.GetTime(timeEl);
		//setTime(hora_actual);
		breakTime( current_minute, timeEl);
		//LOG_INFO7
		LOG_INFO_ARGS("Chk->%02i/%s/%02i %02i:%02i:%02i",timeEl.Day, monthStr(timeEl.Month),timeEl.Year, timeEl.Hour,timeEl.Minute,timeEl.Second);

		LOG_INFO("Chequeando riegos");
			if (last_RiegosCheck == 0) {

				LOG_INFO("Check 0");
				ChequearRiegos(current_minute);

			} else {
				while (last_RiegosCheck < current_minute) {
					last_RiegosCheck += SECS_PER_MIN;

					if (last_RiegosCheck > current_minute)
						last_RiegosCheck = current_minute;



					ChequearRiegos(last_RiegosCheck);
				}
			}



			last_RiegosCheck = current_minute; // convert time elements into time_t


			if (sendWeb)
			{
				LOG_DEBUG_B("Send salidas web");
				//Si procede enviamos a web ;)
				if (!RegistrarSalidaEnWeb())
						error_web_salidas++;
				else
					error_web_salidas=0;


				LOG_DEBUG_ARGS_B("Salidas web %i",error_web_salidas);
			}
	}

}


time_t GTKeeper::GetTimeWithoutSeconds(time_t tiempo)
{
	breakTime(tiempo, timeEl); // break time_t into elements
	timeEl.Second = 0; //Quitamos los segundos para que sea hora exacta..
	return makeTime(timeEl); // convert time elements into time_
}

void GTKeeper::LanzaRiego(uint8_t contador,bool sendsms=false) {

	//Si esta entre los sectores activos :)
	if (programas[contador].Sector>0 && programas[contador].Sector<=PORTS_NUM )
	{


		//Log.Debug("Parada programada ->  %04i/%02i/%02i/ %02i:%02i", year(programas[contador].Hasta), month(programas[contador].Hasta), day(programas[contador].Hasta), hour(programas[contador].Hasta), minute(programas[contador].Hasta));

		//ACtivamos salida ;>
		EnciendePrograma(contador);

		LOG_INFO_ARGS
		(("Lanza P%02i S%02i R%02d:%02d A%02d:%02d")
			,contador+1
			,programas[contador].Sector
			,hour(programas[contador].TiempoRiego)
			,minute(programas[contador].TiempoRiego)
			,hour(programas[contador].TiempoAbono)
			,minute(programas[contador].TiempoAbono)
		);

		//Actualizamos la fecha Hasta
		uint8_t pos=GetPosicion(contador,actPrograma);
		if (pos!=-1)
			salidas[pos].Hasta= GetTimeWithoutSeconds(now()) + programas[contador].TiempoRiego;



		//Miramos los tiempos de abonos :)
		if (programas[contador].TiempoAbono>0)
		{
			EnciendeAbono(1);
			uint8_t pos=GetPosicion(1,actAbono);
			if (pos!=-1)
			{
				//Actualizamos la fecha de abono si es mas posterior
				if (salidas[pos].Hasta<(now() + programas[contador].TiempoAbono))
					salidas[pos].Hasta= GetTimeWithoutSeconds(now()) + programas[contador].TiempoAbono;
			}

		}

	#ifdef SMS
		if (IsGSMEnable() &&  sendsms || (config.AvisosSMS & SMSInicioSector))
		{

			Sim900.SmsMessage(PBB(F("Lanza S%i R%02d:%02d A%02d:%02d\n"),programas[contador].Sector
					,hour(programas[contador].TiempoRiego),minute(programas[contador].TiempoRiego)
					,hour(programas[contador].TiempoAbono),minute(programas[contador].TiempoAbono)));
		}
	#endif
		//Log.Debug("Rega
		//Log.Debug("Regando-> Programa:%i Sector:%i Tiempo:%02i:%02i",contador, programas[contador].Sector, hour(programas[contador].TiempoRiego), minute(programas[contador].TiempoRiego));
		//Log.Debug("Hasta - %04i/%02i/%02i %02i:%02i", year(programas[contador].Hasta), month(programas[contador].Hasta), day(programas[contador].Hasta), hour(programas[contador].Hasta), minute(programas[contador].Hasta));

	}
}

void GTKeeper::PararRiego(uint8_t contador) {

	//Desactiva la salida
 	 ApagaPrograma(contador);


	//Log.Debug("Parando riego-> Programa:%i Sector:%i", contador, programas[contador].Sector);
	//Miramos si hay algun programa que debemos lanzar. ;>
	for (uint8_t i = 0; i < MAX_PROGRAMAS; i++)
	{
		if (i != contador)
		{
				//Chequeamos que no haya otro programa que este en ejecucion para el mismo sector y acabe mas tarde
				if (programas[i].HoraInicio == 88)
				{
					uint8_t programa = programas[i].MinutoInicio;
					//El contador empieza en 0
					if ((programa - 1) == contador && programas[i].TiempoRiego>0)
					{
						LanzaRiego(i);
					}
				}
		}
	}





#ifdef SMS
		if (IsGSMEnable() && (config.AvisosSMS & SMSFinSector))
		{
			Sim900.SmsMessage(PBB(F("Paro P%i S%i\n"),contador+1, programas[contador].Sector));

		}
#endif



}


void GTKeeper::ChequearRiegos(time_t tiempo) {

	//Recorremos todos los programas miramos si debemos lanzar alguno
	//O parar alguno que este en ejecucion...


	uint8_t hora_actual = hour(tiempo);
	uint8_t minuto_actual = minute(tiempo);

	//Sim900.SmsOpen(config.MovilAviso);

	//Log.Info("Chequeando riegos de esta hora %02i:%02i", hora_actual, minuto_actual);

	//Chequeamos si debemos parar algun riego � abono
	uint8_t contador = 0;
	//Deberemos chequear con un while, ya que con un for no nos vale pq al parar algun programa reordena el listado
	//y nos la lia
	while(contador<GetSalidasActivas())
	{
		switch (salidas[contador].Tipo)
		{
			case actPrograma:
			{
				if (salidas[contador].Hasta <= now())
				{
					PararRiego(salidas[contador].Ident);
					contador=0;
				}
				else
					contador++;
			}
			break;
			case actAbono:
			{
					if (salidas[contador].Hasta <= now())
					{
						LOG_DEBUG("Parando abono");
						ApagaAbono(salidas[contador].Ident);
						contador=0;
#ifdef SMS
						if (IsGSMEnable() && (config.AvisosSMS & SMSFinSector))
						{
							Sim900.SmsMessage_P(PSTR("Parada bomba abono\n"));

						}
#endif
					}
					else
						contador++;
			}
			break;
			default:
				contador++;
		}
	}



	for (uint8_t contador = 0; contador < MAX_PROGRAMAS; contador++) {

		//Si todavia no esta arrancado
		if (!SalidaRegistrada(contador,actPrograma) )
		{
			//Log.Debug("Programa. %i - hora %i minuto %i", contador, programas[contador].HoraInicio,	programas[contador].MinutoInicio);

			if (hora_actual == programas[contador].HoraInicio && minuto_actual ==programas[contador].MinutoInicio) {
				bool lanzar = false;
				//Log.Debug("Esta en hora de lanzar riego. %i", contador);

				//Tenemos que ver para que dias de la semana esta configurado que salte. ;>
				if ((programas[contador].Dias & L) && weekday(tiempo) == dowMonday)
					lanzar = true;

				if ((programas[contador].Dias & M) && weekday(tiempo) == dowTuesday)
					lanzar = true;

				if ((programas[contador].Dias & X) && weekday(tiempo) == dowWednesday)
					lanzar = true;

				if ((programas[contador].Dias & J) && weekday(tiempo) == dowThursday)
					lanzar = true;

				if ((programas[contador].Dias & V) && weekday(tiempo) == dowFriday)
					lanzar = true;

				if ((programas[contador].Dias & S) && weekday(tiempo) == dowSaturday)
					lanzar = true;

				if ((programas[contador].Dias & D) && weekday(tiempo) == dowSunday)
					lanzar = true;

				//Si la tenemos que lanzar
				if (lanzar)
					LanzaRiego(contador);
			}

		}

	}


#ifdef SMS
	//Enviara el SMS - o si no tiene ningun msj lo cancela
	Sim900.SmsSend();
#endif
}


//PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele
//Con una polaridad abre la valvula, con la otra la cierra
//De este modo la valula no tiene que estar alimentada continuamente.(ahorro)
void GTKeeper::AbrirValvulaLatch(uint8_t sector)
{
	//El comun si no se activa esta a -(negativo)
	//Activamos el puente para que le llege positivo a la valvula
	digitalWrite(PORT_PUENTEH1_PIN,HIGH);
	digitalWrite(PORT_PUENTEH2_PIN,LOW);
	delay(500);

	//Enviamos pulso
	//Activamos el rele comun
	ENCIENDE_RELE(PORT_COMUN_PIN);
	delay(250);
	//Activamos el rele de la valvula 
	ENCIENDE_RELE(ports[(sector-1)]);
	delay(25);
	
	//Tiramos abajo el rele
	APAGA_RELE(PORT_COMUN_PIN);
	APAGA_RELE(ports[(sector-1)]);

	digitalWrite(PORT_PUENTEH1_PIN,LOW);
	digitalWrite(PORT_PUENTEH2_PIN,LOW);


}

//PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele

//PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele
//Con una polaridad abre la valvula, con la otra la cierra
//De este modo la valula no tiene que estar alimentada continuamente.(ahorro)
void GTKeeper::CerrarValvulaLatch(uint8_t sector)
{
		///El comun si no se activa esta a -(negativo)
		//Activamos el puente para que le llege positivo a la valvula
		digitalWrite(PORT_PUENTEH1_PIN,LOW);
		digitalWrite(PORT_PUENTEH2_PIN,HIGH);
		delay(500);

		//Enviamos pulso
		//Activamos el rele comun
		ENCIENDE_RELE(PORT_COMUN_PIN);
		delay(250);
		//Activamos el rele de la valvula
		ENCIENDE_RELE(ports[(sector-1)]);
		delay(25);
		
		//Tiramos abajo el rele
		APAGA_RELE(PORT_COMUN_PIN);
		APAGA_RELE(ports[(sector-1)]);

		digitalWrite(PORT_PUENTEH1_PIN,LOW);
		digitalWrite(PORT_PUENTEH2_PIN,LOW);

}

 

bool GTKeeper::EnciendeSector(uint8_t sector)
{

	if (!SalidaRegistrada(sector,actSector))
	{
		//Lo ponemos en marcha si aun no lo estaba
		if (RiegosActivosEnSector(sector)==0)
			 this->AbrirValvulaLatch(sector);

		RegistrarSalida(sector,actSector);
		if (config.motor_diesel)
			EnciendeMotor();

		return true;
	}
	else
		return false;

}

bool GTKeeper::ApagaSector (uint8_t sector)
{

	LOG_DEBUG_ARGS_B("Apagando sector %i",sector);

	if (SalidaRegistrada(sector,actSector))
	{
		//Antes de apagar el rele, tendremos que comprobar de los programas activos no sean para ese sector.
		if (RiegosActivosEnSector(sector)==1)
		{
			LOG_DEBUG_ARGS_B("Apaga sector %i",sector);
			this->CerrarValvulaLatch(sector);
		}

		EliminarSalida(sector,actSector);
		if (GetSalidasActivas()==1)
		{
			if (SalidaRegistrada(1,actMotor))
				//Si solo queda la salida activa del motor se apaga
				ApagaMotor();
		}

		return true;
	}
	else
		return false;
}

bool GTKeeper::EnciendeAbono(uint8_t unidAbono)
{
	if (!SalidaRegistrada(unidAbono,actAbono))
	{
		ENCIENDE_RELE(ports_abono[unidAbono-1]);
		RegistrarSalida(unidAbono,actAbono);
		return true;
	}
	else
		return false;
}

bool GTKeeper::ApagaAbono (uint8_t unidAbono)
{
	if (SalidaRegistrada(unidAbono,actAbono))
	{
		APAGA_RELE(ports_abono[unidAbono-1]);
		EliminarSalida(unidAbono,actAbono);
		stop_abono=0;
		return true;
	}
	else
		return false;
}

bool GTKeeper::EnciendePrograma(uint8_t program)
{
	if (!SalidaRegistrada(program,actPrograma))
	{
		//Lo ponemos en marcha si aun no lo estaba
		if (RiegosActivosEnSector(programas[program].Sector)==0)
			this->AbrirValvulaLatch(programas[program].Sector);

		RegistrarSalida(program,actPrograma);

		if (config.motor_diesel)
		{
			EnciendeMotor();
		}
		return true;
	}
	else
		return false;

}

bool GTKeeper::ApagaPrograma (uint8_t program)
{
	if (SalidaRegistrada(program,actPrograma))
	{

		if (RiegosActivosEnSector(programas[program].Sector)==1)
			this->CerrarValvulaLatch(programas[program].Sector);

		EliminarSalida(program,actPrograma);
		if (GetSalidasActivas()==1)
		{
			if (SalidaRegistrada(1,actMotor))
				//Si solo queda la salida activa del motor se apaga
				ApagaMotor();
		}
		return true;
	}
	else
		return false;
}

bool GTKeeper::ApagaMotor()
{
	if (SalidaRegistrada(1,actMotor))
	{
		APAGA_RELE(PORT_MOTOR);
		EliminarSalida(1,actMotor);
		return true;
	}
	else
		return false;
}

bool GTKeeper::EnciendeMotor ()
{
	if (!SalidaRegistrada(1,actMotor))
	{
		ENCIENDE_RELE(PORT_MOTOR);
		RegistrarSalida(1,actMotor);
		return true;
	}
	else
		return false;
}

void GTKeeper::EnciendeSectorSMS(uint8_t sector)
{
	//Apagamos todos sectores , excepto el que estamos arrancando
	ApagarRiegos();

	//encendemos el seleccionado
	if (sector>0 && sector<=PORTS_NUM)
	{
		EnciendeSector(sector);


		LOG_INFO_ARGS("Encendiendo Sector %i, puerto %i",sector,ports[sector-1]);

#ifdef SMS
		if (config.GSMAvailable &&  (config.AvisosSMS & SMSInicioSector))
		{

			Sim900.Sms(config.MovilAviso,PBB(F("Arrancado Sector %i desde SMS"),sector));
		}
#endif
	}


}

void GTKeeper::ApagarRiegos()
{


	//Si hacemos un for, como lo reordena, nos hace la pirula y se quedan activos
	while (GetSalidasActivas()>0)
	{
		switch (salidas[0].Tipo)
		{
			case actPrograma:
			{
				ApagaPrograma(salidas[0].Ident);
			}
			break;
			case actSector:
			{
				ApagaSector(salidas[0].Ident);
			}
			break;
			case actAbono:
			{
				ApagaAbono(salidas[0].Ident);
			}
			break;
			case actMotor:
			{
				ApagaMotor();
			}
			break;
			case actLimpieza:
			{
				//---No implementado
			}
			break;
			default:
			{
				LOG_DEBUG_B("Apagar!!--None");
			}
			break;
		}

	}

}


void GTKeeper::CheckWeb()
{
	//Comprobamos si hay que comprobar los programas de riego via peticion web
	//Log.Info("Quedan: %i para checkweb",TIME_CHECK_WEB_SECONDS-(now() - t_inicial));

	if (((now() - t_last_web) > TIME_CHECK_WEB_SECONDS) && !bWebInProcess )
	{
			bWebInProcess=true;

			if (GetURL(SETTING_URL_PROGRAMACION))
			//Comprobamos si hay nueva programacion via peticion HTTP web.
			{
				 bpendingWeb=false;
				 //reiniciamos contador�?
				 t_last_web = now();
				 error_web=0;

			}
			else
				error_web++;

#ifdef SMS
			//Si se producen m�s errores de la cuenta
			if (error_web>MAX_ERROR_WEB)
			{
				Sim900.SmsOpen(config.MovilAviso);
				Sim900.SmsMessage_P(PSTR("Tras varios intentos no se puede acceder a la programacion web :(\nIntente sincronizar mas tarde."));
				SendSmsHora();
				SendSmsProgramacion();
				Sim900.SmsSend();


				CargaConfigWeb();

				bpendingWeb=false;
				error_web=0;
			}
#endif

			 bWebInProcess=false;
	}


}




void GTKeeper::SendSmsHora()
{
#ifdef SMS
	time_t current_minute=now();
	breakTime( current_minute, timeEl);
	Sim900.SmsMessage(PBB(F("Hora: %02i/%s/%02i %02i:%02i:%02i\n"),timeEl.Day, monthStr(timeEl.Month),tmYearToY2k(timeEl.Year), timeEl.Hour,timeEl.Minute,timeEl.Second));
#endif
}

//Envia info con los sectores en ejecucion
void GTKeeper::SendSmsSectoresEjecucion()
{
//#ifdef SMS
	LOG_INFO("Enviando SMS Sectores en ejecucion!!");


	if (config.MovilAviso!=NULL && strlen(config.MovilAviso)>0)
	{
			Sim900.SmsOpen(config.MovilAviso);


			if (GetSalidasActivas()>0)
			{
				Sim900.SmsMessage_P(PSTR("Sectores con riegos activos:\n"));

				//Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
				for(uint8_t i=0;i< MAX_PROGRAMAS;i++)
				{
					switch (salidas[i].Tipo)
					{
						case actPrograma:
						{
							Sim900.SmsMessage(PBB(F("Programa %i Sector %i\n"),salidas[i].Ident,salidas[i].Sector));
						}
						break;
						case actSector:
						{
							Sim900.SmsMessage(PBB(F("Sector %i\n"),salidas[i].Ident));
						}
						break;
						case actAbono:
						{
							Sim900.SmsMessage_P(PSTR("Bomba abono activada\n"));

						}
						break;
						case actMotor:
						{
							Sim900.SmsMessage_P(PSTR("Motor activado\n"));
						}
						break;
						case actLimpieza:
						{
							//---No implementado
						}
						break;
					}
				}
			}
			else
			//Si no hay en ejecucion . lo indicamos
 				Sim900.SmsMessage(PBB(F("No hay ningun riego activado")));

			Sim900.SmsSend();
	}
//#endif
}


//Envia alerta cuando se completo el reinicio
void GTKeeper::SendSmsFinReinicio()
{
#ifdef SMS


	if (IsGSMEnable() && (config.AvisosSMS & SMSReset))
	{
		Sim900.SmsOpen(config.MovilAviso);

		Sim900.SmsMessage_P(PSTR("Reiniciado GTKeeper con exito\n"));
		SendSmsHora();
		Sim900.SmsMessage(PBB(F("Password SMS: %s\n"),config.PasswordSMS));
		Sim900.SmsMessage(PBB(F("Aviso SMS: %i\n"),config.AvisosSMS));
		Sim900.SmsMessage(PBB(F("Imei: %s\n"),config.Imei));
		SendSmsProgramacion();
		Sim900.SmsSend();

	}
#endif

}

//Envia por SMS alerta de reinicio
void GTKeeper::SendSmsIniReinicio()
{

#ifdef SMS

//Esperamos unos segundos..
if (IsGSMEnable() && (config.AvisosSMS & SMSReset))
{
	delay(1000);
	Sim900.Sms_P(config.MovilAviso,PSTR("Reiniciando GTKeeper.."));
}

#endif
}

//Envia por SMS la programacion, logicamente ha de estar  GSM enable
void GTKeeper::SendSmsProgramacion()
{
#ifdef SMS
	if (IsGSMEnable())
	{
					LOG_INFO("Enviando SMS!!");

					bool tiene_programas=false;
					//Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
					for(uint8_t program=0;program< MAX_PROGRAMAS;program++)
					{

						if (programas[program].TiempoRiego>0)
						{
								LOG_DEBUG_ARGS_B("Enviando programa %i",program);

								tiene_programas=true;
								memset(buffer,0,MAIN_BUFFER_SIZE);
								ProgramaToString(buffer,&programas[program]);
								buffer[strlen(buffer)]='\n'; //Salto linea por programa
								Sim900.SmsMessage(buffer);

						}
					}



					//Si hay algun pdte de enviar..
					if (!tiene_programas)
					{
						Sim900.SmsMessage_P(PSTR("No hay programas cargados"));
					}
	}
#endif

}



void GTKeeper::ResetConfig()
{


	memset(&config,0,sizeof(Configuracion));
	config.motor_diesel=false;




	strcpy_P(config.PasswordSMS,PSTR("1111"));
	config.numabono=0;
	config.numpuertos=PORTS_NUM;
	config.GSMAvailable=true;
	//Mandar sms por defecto
	config.AvisosSMS=(uint8_t)( SMSInicioSector | SMSFinSector |   SMSProgramacionWeb |  SMSReset);




}

bool GTKeeper::CargaConfigDesdeSim()
{
	if (Sim900.ExisteContactoSIM(1) && 	Sim900.ExisteContactoSIM(2) && 	Sim900.ExisteContactoSIM(3))
	{

		Sim900.GetSIMContact(1,config.MovilAviso,NULL);
		Sim900.GetSIMContact(2,config.PasswordSMS,NULL);
		Sim900.GetSIMContact(3,NULL,config.APN);
		Sim900.GetSIMContact(4,NULL,config.userAPN);
		Sim900.GetSIMContact(5,NULL,config.pwdAPN);
		return true;
	}
	else
		return false;
}


void GTKeeper::CargaConfigDesdeString(char* configstr)
{
	char tmp[5];


	//12665331679911111215

	//3 para AvisosSMS
	memset(tmp,0,5);
	strncpy(tmp,configstr,3);
	config.AvisosSMS=atoi(tmp);

	//6 siguientes para movil
	 strncpy(config.MovilAviso,configstr+3,9);

	//NUM 4 password sms
	 strncpy(config.PasswordSMS,configstr+12,4);

	 //BOOL 1 o 0 para motor diesel
	 memset(tmp,0,4);
	 strncpy(tmp,configstr+16,1);
	 config.motor_diesel=atoi(tmp);
	//
	//NUM 1 para
	 memset(tmp,0,4);
	 strncpy(tmp,configstr+17,1);
	 config.numabono=atoi(tmp);

	//NUM 2 para puertos
	 memset(tmp,0,4);
	 strncpy(tmp,configstr+18,2);
	 config.numpuertos=atoi(tmp);

}


//Carga los parametros de configuraci�n web para que el modulo pueda conectar a internet
void GTKeeper::CargaConfigWeb()
{

		Sim900.ConfigAPN(F("CONTYPE"),"GPRS");


		Sim900.SendCommandCheckError(F("AT+CSTT=\"%s\",\"%s\",\"%s\""),(__FlashStringHelper*)SIM900::AT_OK,(__FlashStringHelper*)SIM900::AT_ALL_ERRORS,config.APN,config.userAPN,config.pwdAPN);

		if (strlen(config.APN)>0)
			Sim900.ConfigAPN(F("APN"),config.APN);

		//--Configuracion APN
		if (strlen(config.userAPN)>0)
			Sim900.ConfigAPN(F("USER"),config.userAPN);

		//--UserAPN
			if (strlen(config.pwdAPN)>0)
			Sim900.ConfigAPN(F("PWD"),config.pwdAPN);


}


void GTKeeper::CheckWebConfig()
{
	//Comprobamos si es necesario refrescar la configuraci�n

	//Realizamos la peticion para que nos responda con la configuracion actual
	bool result=GetConfig4Web(SETTING_URL_CONFIG);
}

bool GTKeeper::GetConfig4Web(char *url)
{
	return Sim900.URLRequest(url,true,NULL,ConfigHttpResultCallbackStatic);
}

void GTKeeper::ConfigHttpResultCallbackStatic(const char* url,int len)
{
	gtKeeper.ConfigHttpResultCallback(url,len);
}

void GTKeeper::ConfigHttpResultCallback(const char* url,int len)
{
	int contador=0;
	uint8_t readResult=RX_OK_READ;
	uint8_t result=(len>0?LOAD_WEB_OK:LOAD_WEB_ERR_NO_RESPONSE);

	//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
	if (Sim900.SendCommandCheck( "AT+HTTPREAD=%i,%i","+HTTPREAD:",contador,LEN_CONFIG_STRING)==RX_CHECK_OK)
	{
		//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
		//+HTTPREAD: 19
		//char *buffer=Sim900.GetLastResponse();
		//buffer=buffer+10;
		//int serialLen= atoi(buffer);<--Serial len
		Sim900.WaitResponse(500);
		//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
		readResult= Sim900.ReadSerialLine();
		char *line=Sim900.GetLastResponse();
		if(strlen(line)==LEN_CONFIG_STRING)
		{
			//Cargamos la config desde string si todo sale OK punto pelota
			gtKeeper.CargaConfigDesdeString(line);
			gtKeeper.EEPROMGuardaConfig();
		}

	}
}



bool GTKeeper::GetURL(char *url)
{
	return Sim900.URLRequest(url,true,NULL,DefaultHttpResultCallbackStatic);
}


void GTKeeper::DefaultHttpResultCallbackStatic(const char* url,int len)
{
	gtKeeper.GetHttpResultCallback(url,len);
}


//LLamada a metodo
void GTKeeper::GetHttpResultCallback(const char* url,int len)
{
	//url solo sera la de consulta de programas
	//por lo tanto no la comprobaremos

	int contador=0;
	uint8_t program=0;
	uint8_t readResult=RX_OK_READ;
	uint8_t result=(len>0?LOAD_WEB_OK:LOAD_WEB_ERR_NO_RESPONSE);


	//Si es la web de comprobacion de programas procedemos a actualizar la programacion
	if (strncmp(url,SETTING_URL_PROGRAMACION,strlen(SETTING_URL_PROGRAMACION))==0)
	{

			bool delete_all=false;
			while (!delete_all && contador<len && readResult==RX_OK_READ && result==LOAD_WEB_OK)
			{
				//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
				if (Sim900.SendCommandCheck( "AT+HTTPREAD=%i,%i","+HTTPREAD:",contador,LEN_PROGRAMA_STRING_CR_LF)==RX_CHECK_OK)
				{
					//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
					//+HTTPREAD: 19
					//char *buffer=Sim900.GetLastResponse();
					//buffer=buffer+10;
					//int serialLen= atoi(buffer);<--Serial len
					Sim900.WaitResponse(500);
					//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
					readResult= Sim900.ReadSerialLine();
					char *line=Sim900.GetLastResponse();

					LOG_INFO_ARGS("Web=>%s",line);

					if (result==LOAD_WEB_OK ) //Mientras no haya errores de procesamiento
					{
						if (strcmp_P(line,PSTR("DELETE_ALL"))==0)
						{
							delete_all=true;
							//Si es delete all hay que salir del bucle

							LOG_DEBUG("BORRANDO TODOS PROGRAMAS");

							//Reseteamos los programas
							for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
							{
								//Los otros programas los dejamos a cero
								ResetPrograma(&programas[i]);

								LOG_INFO_ARGS("Programa %i",i);
								//Guardamos en SIM los programas cargados

								if (!GrabarProgramaAEEPROM(i,&programas[i]))
									LOG_ERROR("Error grabando a sim");
							}

							//Borra todos programas ;)
							/*while (contador<=SIM_MAX_ENTRIES)
							{
								Sim900.BorrarContactoSIM(contador);
								contador++;
							}*/
							result=LOAD_WEB_OK ;

						}
						else if (strlen(line)==LEN_PROGRAMA_STRING)
						{
							if (program<MAX_PROGRAMAS)
							{
								//Tiene que venir una linea de programaci�n de sector :)
								if (!CargaProgramaDesdeString(line,&programas[program]))
								{
									//OJO No podemos guardar en SIM porque sino procesamos los results metodo ProcessResults y no leemos las
									//lineas desde aqui :(
									//ademas se ira recibiendo datos en el puerto serie y se puede saturar
									//Por lo que se ha movido fuera de este while

									LOG_ERROR_ARGS( "Prog->%s",line );

									result=LOAD_WEB_ERR_LOAD_PROG;
								}

								//Incrementar contador de programas
								program++;
							}


						}
						else
						{

							LOG_ERROR_ARGS("NoLen %i %s",strlen(line),line);
							result=LOAD_WEB_ERR_UNKNOWN_STRING;
						}
					}


					contador+=strlen(line) + 2; //La longitud de la linea + los correspondientes caracteres de fin de linea [CR] + [LF]

				}//fin AT+HTTPREAD
			}//fin while


			//Apagado de sectores �?
			//ApagarRiegos();



			if (result==LOAD_WEB_OK)
			{
				LOG_INFO_ARGS("Programas leidos %i",program+1);
				for (uint8_t i=0;i<program;i++)
				{
					//Los otros programas los dejamos a cero

					LOG_INFO_ARGS("Programa %i",i);
					//Guardamos en SIM los programas cargados
					if (!GrabarProgramaAEEPROM(i,&programas[i]))
					{
						LOG_ERROR_ARGS("Error Grabado programa %i",program);
						result=LOAD_WEB_ERR_SAVE_SIM;
					}
					else
						LOG_WARN_ARGS("Grabado programa %i",i);
				}
			}



			if (result==LOAD_WEB_OK)
			{


				//Borra todos programas desde donde esta el contador en adelante  ;)
				while ((program)<MAX_PROGRAMAS)
				{
					//ResetPrograma(&programas[program+SIM_ENTRY_PROGRAM]);

					//Los otros programas los dejamos a cero
					ResetPrograma(&programas[program]);
					if (!GrabarProgramaAEEPROM(program,&programas[program]))
						result=LOAD_WEB_ERR_DELETE_PROGRAM;

					programas[program].TiempoAbono=0;
					programas[program].TiempoRiego=0;

					//Lo quitamos del programa
					program++;
				}


		#ifdef SMS
				if (result==LOAD_WEB_OK)
				{

					//Avisamos de que se ha cargado nuevo programa
					if (config.AvisosSMS & SMSProgramacionWeb)
					{
						Sim900.SmsOpen(config.MovilAviso);
						Sim900.SmsMessage_P(PSTR("Cargada programacion\n"));
						SendSmsHora();
						SendSmsProgramacion();
						Sim900.SmsSend();
					}

					//Registramos en la web que la carga del programa esta OK!!
					//Sim900.GetURL()
				}
		#endif
			}

			if (result!=LOAD_WEB_OK)
			{
				//Reseteamos los programas
				for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
				{
					//Los otros programas los dejamos a cero
					ResetPrograma(&programas[i]);

					LOG_INFO_ARGS("Programa %i",i);
					//Guardamos en SIM los programas cargados

					if (!GrabarProgramaAEEPROM(i,&programas[i]))


						LOG_ERROR("Error grabando a sim");
				}




		#ifdef SMS
				//Algo ha ido mal :(((
				Sim900.Sms(config.MovilAviso,PBB(F("Error cargando programas - %i"),result));
		#endif
			}


			LOG_DEBUG("FIN gETWEB");
	}

}


//Procesa los mensajes AT Recibidos para callbacks y demas
bool GTKeeper::ProcessATMensajes(char * msg)
{

	if (msg!=NULL && bSetupCompleted)
	{


		//LLamadas
//		if (strcmp_P(msg,(char *)F("RING"))==0)
//		{
//			//Log.Info("LLamada entrante");
//
//		}
//		//Messages
//		else if (strcmp_P(msg,(char *)F("NO CARRIER"))==0)
//		{
//			//Log.Info("fin de llamada");
//
//		}
		//SMS
		if (strncmp_P(msg,(char *)F("+CMT"),4)==0)
		{

			//uint8_t contador=0;
			memset(buffer,0,MAIN_BUFFER_SIZE);

			//Esperamos medio seg, para que reciba las siguientes lineas
			delay(400);
			if (Sim900.ReadSerialLine(buffer,MAIN_BUFFER_SIZE)==RX_OK_READ)
			{
				//La password debera ir entre corchetes #1111#
				memset(buff_parse,0,MAIN_BUFFER_PARSE);
				sprintf(buff_parse,"#%s#",config.PasswordSMS);
				//Comprobamos que contiene la pwd correcta
				if (strncmp(buffer,buff_parse,strlen(buff_parse))==0)
				{
					//Comprobamos el comando
					char command= buffer[strlen(buff_parse)];

					LOG_DEBUG_ARGS ("COMANDO %s",command);

					//lanzar Sector!!!
					char * sectorptr=buffer+(strlen(buff_parse));



					//Si es comando de arrancar sector-- Mays or Min
					if (command=='P' || command=='p')
					{
						 sectorptr=sectorptr+1;
						int programa=atoi(sectorptr);
						LOG_DEBUG_ARGS ("PROGRAMA %i %s",programa,sectorptr);
						if (programa>0 && programa<=MAX_PROGRAMAS)
						{
#ifdef SMS
							Sim900.SmsOpen(config.MovilAviso);
#endif
							LanzaRiego(programa-1,true);
#ifdef SMS
							SendSmsHora();
							Sim900.SmsSend();
#endif

						}
					}
					else if (strncmp(sectorptr,"APN:",4)==0)
					{
						sectorptr=sectorptr+4;
						if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
						{
							strcpy(config.APN,sectorptr);
							EEPROMGuardaConfig();
							CargaConfigWeb();
							Sim900.Sms(config.MovilAviso,PBB(F("APN Configurado %s"),config.APN));
						}
						else
							Sim900.Sms(config.MovilAviso,PBB(F("Incorrecto APN: %s"),sectorptr));


					}
					else if (strncmp(sectorptr,"USERAPN:",8)==0)
					{
						sectorptr=sectorptr+8;
						if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
						{
							strcpy(config.userAPN,sectorptr);
							EEPROMGuardaConfig();
							CargaConfigWeb();
							Sim900.Sms(config.MovilAviso,PBB(F("User APN Configurado %s"),sectorptr));
						}
						else
							Sim900.Sms(config.MovilAviso,PBB(F("Incorrecto User APN: %s"),sectorptr));


					}
					else if (strncmp(sectorptr,"PWDAPN:",7)==0)
					{
						sectorptr=sectorptr+7;
						if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
						{
							strcpy(config.pwdAPN,sectorptr);
							EEPROMGuardaConfig();
							CargaConfigWeb();
							Sim900.Sms(config.MovilAviso,PBB(F("Pwd APN Configurado %s"),sectorptr));
						}
						else
							Sim900.Sms(config.MovilAviso,PBB(F("Incorrecto PWd APN: %s"),sectorptr));


					}
					else
					{

						//Comandos numericos
								int sector=atoi(sectorptr);

								//El comando  77 -- Quiere muestra info! :)
								if (sector==77)
								{

			#ifdef SMS

									Sim900.SmsOpen(config.MovilAviso);
									Sim900.SmsMessage_P(PSTR("Estoy preparado!\n"));
									SendSmsHora();
									SendSmsProgramacion();

									Sim900.SmsSend();
			#endif


								}
								//Si el comando es 88 -- Quiere decir que queremos forzar el chequeo web
								//Comprobamos tb que no estamos en pleno proceso de actualizacion web
								if (sector==88)
								{

									bpendingWeb=true; //Ponemos pendiente
									delay(300);
									//Reseteamos la fecha de ultimo refresco para forzarlo, en caso que estemos actualizando cada x tiempo;
									t_last_web=0;

									CheckWeb();
								}

								else if (sector==89)
								{
									this->CheckWebConfig();

								}
								//Si el comando es 99 quiere decir que apague todos sectores
								//Y si esta ejecutandose la programacion que la cancele
								else if ( sector==99)
								{
									ApagarRiegos();
			#ifdef SMS
									if (config.AvisosSMS & SMSFinSector)
									{

										Sim900.Sms_P(config.MovilAviso,PSTR("Parados todos programas"));
									}
			#endif
								}
								//Comando 101 este comando quiere decir que enciende la bomba
								else if (sector==101)
								{

									//Dependiendo si esta encendido o no
									//OJO!!!NOTAR QUE ENCENDIDO ES ESTADO LOW, SI ES HIGH LA BOMBA NO FUNCIONA

									if (GetPosicion(1,actAbono)!=-1)
									{
										ApagaAbono(1);
			#ifdef SMS
										if (config.AvisosSMS & SMSFinSector)
										Sim900.Sms_P(config.MovilAviso,PSTR("Apagada bomba de abono por SMS"));
			#endif
									}
									else
									{
										EnciendeAbono(1);
			#ifdef SMS
										if (config.AvisosSMS & SMSInicioSector)
										Sim900.Sms_P(config.MovilAviso,PSTR("Arrancada bomba de abono por SMS"));
			#endif
									}

								}
								//Comando para mostrar la informaci�n de estado del reiego
								else if (sector==100)
								{
									SendSmsSectoresEjecucion();

								}
								//Si el comando esta dentro del rango de sectores lo arrancamos
								else if (sector>0 && sector<=PORTS_NUM )
								{

									LOG_INFO_ARGS("Arrancar sector %i",sector);
									EnciendeSectorSMS(sector);

								}
					}
				}
			}
		}

		LOG_INFO_ARGS("MSG => %s",msg);

	}
}


//Loop para procesar
bool GTKeeper::Loop()
{
	bool hashError=false;
	if (IsGSMEnable())
	{
			//Comprobamos si hay que lanzar/parar sectores
			//Chequeamos si esta arrancado
			if (Sim900.EstaArrancado() && error_web_salidas<2)
			{
				//Si la sim esta ok
				if (CheckSIM())
				{
					//Chequeamos si esta registrado en la red movil
					if (Sim900.EstaRegistrado())
					{


						//Si se ha sido re-inicializado el modulo GSM
						//Volvemos a cargar la configuraci�n web
						if (bRebootSIM)
						{
							CargaConfigWeb();
							bRebootSIM=false;
						}

						//Encendemos los leds de trabajando
						OnChangeStatus(LED_WORKING_PIN);

						//Si ha pasado el tiempo desde la ultima actualizaci�n comprobamos si hay
		#ifndef NO_CHECK_WEB //Preprocesador para chequeo web
						CheckWeb(); //Se har� por comando SMS #PWDSMS#,88
		#else
						//En caso que tengamos pendiente refresco web...
						if (IsPendingWeb())
								CheckWeb();
		#endif

					}
					else
					{
						hashError=true;

						LOG_ERROR_ARGS("GT-CREG-%i",ERROR_NO_REGISTER);

						OnChangeStatus(LED_ERROR_NETWORK_PIN);
					}
				}
				else
				{
					hashError=true;
					//Registrando en la red
					LOG_ERROR_ARGS("GT-SIM-%i",ERROR_SIM);
					OnChangeStatus(LED_ERROR_SIM_PIN);

				}
			}
			else
			{
					hashError=true;
					LOG_CRITICAL_ARGS("GT-AT-%i",ERROR_NO_RESPOND);
					//Encendemos los led error en modo tranquilo
					OnChangeStatus(LED_ERROR_MODULE_PIN);
					if (error_web_salidas>2)
						Sim900.SwitchModule();

					error_web_salidas=0;
					bRebootSIM=true;
					Sim900.ActivaModulo();
			}
	}

	return hashError;
}


void GTKeeper::EEPROMGuardaConfig()
{
	config.flag_check='X';
	//Cargamos la configuracion
	 while (!eeprom_is_ready());
	 eeprom_write_block((void*)&config, ( void*) 0, sizeof(Configuracion));

}


bool GTKeeper::EEPROMCargaConfig()
{

		config.flag_check='\0';//Lo ponemos a false para comprobar que carga bien la config

		//Cargamos la configuracion
		eeprom_read_block((void*)&config, (const void*) 0, sizeof(Configuracion));

		return (config.flag_check=='X');

}

//Trata de comprobar si esta registrado en la red GSM
bool GTKeeper::EstaRegistradoGSM()
{
	//Comprobamos si esta registrado a la red Movil
	 return Sim900.EstaRegistrado();

}

//Trata de fijar la hora utilizando la red GSM
bool GTKeeper::FijarHoraGSM()
{
	//Obtenemos hora del telefono
	 time_t hora_actual=Sim900.GetTime(timeEl);

	 LOG_INFO_ARGS_B("Set hora->%02i/%s/%02i %02i:%02i:%02i",timeEl.Day, monthStr(timeEl.Month),timeEl.Year, timeEl.Hour,timeEl.Minute,timeEl.Second);

	if (hora_actual!=0 && timeEl.Year!=01)
	{
		this->SetHora(hora_actual);
		return true;
	}
	else
		return false;


}

//Indica si la hora ha sido fijada
bool GTKeeper::EstaEnHora()
{
	return isHourSet;
}

//Indica si Gtkeeper esta configurado con modulo gsm
bool GTKeeper::IsGSMEnable()
{
	return config.GSMAvailable;
}

//Fija la hora :)
void GTKeeper::SetHora(time_t time)
{
	if (time!=0)
		isHourSet=true;
	//Fijamos la hora en arduino
	t_last_web=time;
	setTime(time);
}





void GTKeeper::RegistrarEstadisticas()
{
	//Chequeamos si tiene abono
	bool bAbono1= SalidaRegistrada(1,actAbono);



	for (uint8_t salida=0;salida<GetSalidasActivas();salida++)
	{
		if (salidas[salida].Tipo==actSector || salidas[salida].Tipo==actPrograma)
		{



			//salidas[salida].sector
			//Check en caso que ese sector este en ejecucion x mas de un programa
			bool bImputado=false;
			for (uint8_t i=0;i<salida;i++)
			{
				if (salidas[i].Sector==salidas[salida].Sector)
				{
					bImputado=true;
					break;
				}
			}

			//Si no esta imputado el tiempo para ese Sector... lo registramos
			if (!bImputado)
			{



				EEPROMLeerEstadistica(salidas[salida].Sector,&estadistica);


				//Registramos la estadistica para el sector
				estadistica.sector=salidas[salida].Sector;
				estadistica.tiempo_riego+=60;//60seg

				//Si tiene abono
				if (bAbono1)
					estadistica.tiempo_abono+=60;//60seg


				LOG_DEBUG_ARGS_B ("Registrando S%i A%i",salidas[salida].Sector,bAbono1);

				EEPROMGuardarEstadistica(&estadistica);



			}
		}
	}

}

void GTKeeper::ResetearEstadisticas()
{
	for (uint8_t contador=1;contador<=PORTS_NUM;contador++)
	{
		estadistica.sector=contador;
		estadistica.tiempo_abono=0;
		estadistica.tiempo_riego=0;
		EEPROMGuardarEstadistica(&estadistica);
	}
}

void GTKeeper::EEPROMLeerEstadistica(uint8_t sector,Estadistica * stat)
{
	uint16_t ee_addr= GET_ADDRES_ESTADISTICA(sector);
	eeprom_read_block((void *)stat,(void *)ee_addr,sizeof(Estadistica));

}

void GTKeeper::EEPROMGuardarEstadistica(Estadistica * stat)
{

	uint16_t ee_addr= GET_ADDRES_ESTADISTICA(stat->sector);
	//Ahora lo que vamos a hacer es comparar con lo que hay guardado en la eeprom
	 while (!eeprom_is_ready());

	eeprom_write_block((void *)stat,(void *)ee_addr,sizeof(Estadistica));

	//Solo guardo cuando sean iguales
//	if (strcmp(buffer,buffer_reader)!=0)
//	{
//		LOG_DEBUG_ARGS("Guardando.. %s",buffer);
//		eeprom_write_block((void *)buffer,(void *)ee_addr,LEN_PROGRAMA_STRING);
//	}
}



void GTKeeper::ShowInfoSalidas()
{

	memset(buffer,0,MAIN_BUFFER_SIZE);
	 for(int8_t i=0;i<GetSalidasActivas();i++)
	 {
		 switch (salidas[i].Tipo)
		 {
			 case actPrograma:
			 {
				strcpy_P(buffer,PSTR("PROGRAMA"));
			 }
			 break;
			 case actSector:
			 {
				 strcpy_P(buffer,PSTR("SECTOR"));
			 }
			 break;
			 case actAbono:
			 {
				 strcpy_P(buffer,PSTR("ABONO"));
			 }
			 break;
			 case actMotor:
			 {
				 strcpy_P(buffer,PSTR("MOTOR"));
			 }
			 break;
			 case actLimpieza:
			 {
				 strcpy_P(buffer,PSTR("LIMPIEZA"));
			 }
			 break;

		 }

		 LOG_DEBUG_ARGS("%s %i %02d:%02d hasta %02d:%02d",buffer,salidas[i].Ident, hour(salidas[i].Desde),minute(salidas[i].Desde),hour(salidas[i].Hasta),minute(salidas[i].Hasta));
	 }
}

//Obtiene la posici�n en la que la salida ha sido registrada
int8_t GTKeeper::GetPosicion(uint8_t salida , TipoSalidaActiva tipo)
{
	 for(int8_t i=0;i<GetSalidasActivas();i++)
	 {
		 if (salidas[i].Tipo== tipo && salidas[i].Ident==salida )
		 {
			 return i;
		 }
	 }
	 return -1;
}

//Indica si esa salida esta registrada
bool GTKeeper::SalidaRegistrada(uint8_t salida , TipoSalidaActiva tipo)
{
	return (GetPosicion(salida,tipo)!=-1);
}

uint8_t GTKeeper::RiegosActivosEnSector(uint8_t sector)
{
	 int num_riegos=0;
	 for(int8_t i=0;i<GetSalidasActivas();i++)
	 {
		 if ((salidas[i].Tipo== actPrograma || salidas[i].Tipo==actSector) && salidas[i].Sector==sector )
			 num_riegos++;

	 }

	 return num_riegos;
}


//Registra la salida
void GTKeeper::RegistrarSalida(uint8_t salida , TipoSalidaActiva tipo)
{
	int8_t pos=GetPosicion(salida,tipo);
	if (pos==-1)
	{

			uint8_t i=GetSalidasActivas();

			LOG_DEBUG_ARGS_B("SAl %i No registrada, registrar en %i, ",salida,i);
 			//Pal saco - Lo asocio al 1� que esta libre
			salidas[i].Tipo= tipo;
			salidas[i].Ident= salida;
			salidas[i].Desde=now();//Registra la hora desde la que esta activa!!


			if (tipo==actSector)
				salidas[i].Sector=salida;
			else if (tipo==actPrograma)
				salidas[i].Sector=programas[salida].Sector;
			else
				salidas[i].Sector=0;

			RegistrarSalidaEnEEPROM(&salidas[i],false);

			salidas_activas++;
			qsort (salidas, salidas_activas, sizeof(SalidasActivas),sortmethod);
	}
}

//Elimina la salida
void GTKeeper::EliminarSalida(uint8_t salida , TipoSalidaActiva tipo)
{
	int8_t pos=GetPosicion(salida,tipo);
	if (pos!=-1)
	{
		salidas[pos].Hasta=now();

		//Lo registramos en la g�eb :)
		RegistrarSalidaEnEEPROM(&salidas[pos],true);

		//if (salidas[pos].Tipo==)
		//if (RiegosActivosEnSector(salidas[pos].Sector))
		//salidas[pos].Desde=now

		//Registrar estadistica
		//salidas[pos].Hasta=0;

		salidas[pos].Tipo=actNone;
		qsort (salidas, salidas_activas, sizeof(SalidasActivas),sortmethod);
		salidas_activas--;//Importante para que reordene correctamente hacer la resta despues del qsort


	}
}

void  GTKeeper::RegistrarSalidaEnEEPROM(SalidasActivas * stat,bool finaliza)
{


	//Su es 0 inicializamos
	if (salidas_web==0)
	{

		salidas_web=EEADDR_SW;
	}

	memset(buffer,0,MAIN_BUFFER_SIZE);
	if (finaliza)
	{
		sprintf_P(buffer,PSTR("%i;%i;%lu;%lu;%i;1\n"),stat->Tipo,stat->Ident,stat->Desde,stat->Hasta,stat->Sector);

	}
	else
	{
		sprintf_P(buffer,PSTR("%i;%i;%lu;%lu;%i;0\n"),stat->Tipo,stat->Ident,stat->Desde,stat->Hasta,stat->Sector);

	}



	//Lo guardamos en la EEPROM  e incrementamos contador
	if ((salidas_web + strlen(buffer))>=EEPROM_LENGTH)
	{
		LOG_DEBUG_B("Eeprom llena");
	}
	else
	{
		//Guardamos la salida e incrementamos el contador de memoria :)
		 while (!eeprom_is_ready());

		 eeprom_write_block((void*)buffer, ( void*)salidas_web, strlen(buffer));
		 salidas_web+=strlen(buffer);
	}
}


bool GTKeeper::PostHttpParametersCallback()
{

	//Set the Content as multipart/form-data type of HTTP POST and also set the boundary value
	//AT+HTTPPARA="CONTENT","multipart/form-data; boundary=----WebKitFormBoundaryvZ0ZHShNAcBABWFy"
	//OK
	//====================================
	//AT+HTTPDATA=192,10000
	//OK
	//DOWNLOAD
	//Una vez recibimos el download , enviamos toda peticion el 192, es de todo Cabecera + Contenido .
	//------WebKitFormBoundaryvZ0ZHShNAcBABWFy
	//Content-Disposition: form-data; name="fileToUpload"; filename="data.txt"
	//Content-Type: text/plain
	//Hello Ravi
	//------WebKitFormBoundaryvZ0ZHShNAcBABWFy
	//OK

	//http://www.raviyp.com/embedded/194

	const char  boundary[] = "---------------------------8278697928671";


	if (Sim900.SendCommandCheck( F("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=%s\""),(__FlashStringHelper*) ATSerial::AT_OK,boundary)==RX_CHECK_OK)
	{

		//gtKeeper.salidas_web-GET_ADDRES_SALIDAS_WEB
	//	if (Sim900.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"),195)==RX_CHECK_OK)



		int16_t totalLen=191 + (gtKeeper.salidas_web-EEADDR_SW); //191 es el constante, boundarys etc etc

		LOG_DEBUG_ARGS_B("totalLen %i",totalLen);

		if (Sim900.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"), totalLen)==RX_CHECK_OK)
		{

			delay(500);

				uint8_t t=0;
				//t+=Sim900.SendRawData("Accept-Encoding: deflate");
				//Vomitamos lo que hayamos registrado
				//Boundary
				t=Sim900.SendRawData("--");
				t+=Sim900.SendRawData(boundary);
				t+=Sim900.SendRawData("\r\n");

				//Cabecera
				t+=Sim900.SendRawData("Content-Disposition: form-data; name=\"submitted\"; filename=\"abcd.txt\"\r\n");
				t+=Sim900.SendRawData("Content-Type: text/plain\r\n\r\n");

				//Ahora enviamos todo lo que haya en estadisticas
				//t+=Sim900.SendRawData("abcd\r\n");

				uint16_t ee_addr=EEADDR_SW;

				 while ( ee_addr != gtKeeper.salidas_web) {
				    //ch = EEPROM.read(ee_addr);
					 memset (gtKeeper.buff_parse,0,MAIN_BUFFER_PARSE);

					 if (MAIN_BUFFER_PARSE<(gtKeeper.salidas_web-ee_addr))
					 {
						 eeprom_read_block((void *)gtKeeper.buff_parse,(void *)ee_addr,MAIN_BUFFER_PARSE);
						 ee_addr+=MAIN_BUFFER_PARSE;
					 }
					 else
					 {
						 eeprom_read_block((void *)gtKeeper.buff_parse,(void *)ee_addr,(gtKeeper.salidas_web-ee_addr));
						 ee_addr+=(gtKeeper.salidas_web-ee_addr);
					 }


				    t+=Sim900.SendRawData(gtKeeper.buff_parse);
				  }


				//Boundary
				t+=Sim900.SendRawData("\r\n--");
				t+=Sim900.SendRawData(boundary);
				t+=Sim900.SendRawData("--\r\n");

				LOG_DEBUG_ARGS_B("Enviado x post--> %i",t);
				delay(500);
				return true;
		}
		else
			return false;
	}
	else
		return false;
}

void GTKeeper::PostHttpResultCallback(const char* url,int length)
{

	uint16_t contador=0;
	uint8_t readResult=0;
	uint8_t result =-1;

	//Solo esperamos 2 posibles valores OK � ERR
	if (length==2)
	{
		//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
		if (Sim900.SendCommandCheck( F("AT+HTTPREAD=%i,%i"),F("+HTTPREAD:"),contador,length)==RX_CHECK_OK)
		{
			//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
			//+HTTPREAD: 19
			//char *buffer=Sim900.GetLastResponse();
			//buffer=buffer+10;
			//int serialLen= atoi(buffer);<--Serial len

			Sim900.WaitResponse(500);
			//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
			readResult= Sim900.ReadSerialLine();
			char *line=Sim900.GetLastResponse();

			LOG_INFO_ARGS("Web=>%s",line);

			if (readResult==RX_OK_READ && strcmp_P(line,PSTR("OK"))==0 ) //Mientras no haya errores de procesamiento
			{
				result =LOAD_WEB_OK;
				//Si es ok.. inicializamos contador .. :)
				gtKeeper.salidas_web=EEADDR_SW;
			}



		}
	}

}

bool GTKeeper::RegistrarSalidaEnWeb()
{
	bool result=true;
	//Comprobamos si hay algo pdte de enviar a la web
	if (salidas_web!=0)
	{

		if (salidas_web!=EEADDR_SW)
		{

			//A enviar ..
			memset(buffer,0,MAIN_BUFFER_SIZE);
			sprintf(buffer,URL_SEND_SALIDAS,config.Imei);
			if (!Sim900.URLRequest(buffer,false,PostHttpParametersCallback,PostHttpResultCallback))
				result=false;
				//Reiniciamos modulo GSM
		}
	}

	return result;
}




//Negativo A>B
//Cero =
//Positivo B>A
int GTKeeper::sortmethod(const void* a, const void* b)
{
	SalidasActivas *salidaA = (SalidasActivas *)a;
	SalidasActivas *salidaB = (SalidasActivas *)b;

	//Lo que mas pesara sera el tipo :>
	int result= (salidaA->Tipo-salidaB->Tipo);

	//Si son del mismo tipo, fecha desde
	if (result==0)
			result=(salidaA->Desde-salidaB->Desde);

	return result;
}
