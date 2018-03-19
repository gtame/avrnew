/*
 * Web.cpp
 *
 * Created: 14/03/2018 8:46:53
 *  Author: gtame
 */ 
#include <gtkeeper.h>


bool PostHttpParametersCallback()
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


	if (gtKeeper.SendCommandCheck( F("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=%s\""),(const  __FlashStringHelper*) ATSerial::AT_OK,boundary)==RX_CHECK_OK)
	{
		//Cambios de configuracion¿?
		//Cambios de programacion ¿?
		//Cambios en salidas activas
		//Logs..¿?

		


		uint8_t numsalidas= gtKeeper.GetSalidasActivas();
		
 
 		int16_t totalLen=191 + 
		LEN_CONFIG_STRING +//CONFIGURACION
		(LEN_PROGRAMA_STRING * MAX_PROGRAMAS)+ //PROGRAMAS
		(LEN_SALIDA_STRING * numsalidas)+ //SALIDAS
		0 ;//Logfile


		 //191 es el constante, boundarys etc etc

		LOG_DEBUG_ARGS_B("totalLen %i",totalLen);

		if (gtKeeper.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"), totalLen)==RX_CHECK_OK)
		{

			delay(500);

			uint8_t t=0;
			//t+=SendRawData("Accept-Encoding: deflate");
			//Vomitamos lo que hayamos registrado
			//Boundary
			t=gtKeeper.SendRawData("--");
			t+=gtKeeper.SendRawData(boundary);
			t+=gtKeeper.SendRawData("\r\n");

			//Cabecera
			t+=gtKeeper.SendRawData("Content-Disposition: form-data; name=\"submitted\"; filename=\"abcd.txt\"\r\n");
			t+=gtKeeper.SendRawData("Content-Type: text/plain\r\n\r\n");

			//Ahora enviamos todo lo que haya en estadisticas
			//t+=SendRawData("abcd\r\n");
			 
			 //Config
			 memset(bufferapp,0,MAIN_BUFFER_SIZE);
			 gtKeeper.ConfiguracionToString(bufferapp);
			 t+=gtKeeper.SendRawData(bufferapp);

			 //Programas
			 for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
			 {
				memset(bufferapp,0,MAIN_BUFFER_SIZE);
				gtKeeper.ProgramaToString(i,bufferapp);
				t+=gtKeeper.SendRawData(bufferapp);
			 }

			 //Salidas
			 for (uint8_t i=0;i<numsalidas;i++)
			 {
				 memset(bufferapp,0,MAIN_BUFFER_SIZE);
				 gtKeeper.SalidaToString(i,bufferapp);
				 t+=gtKeeper.SendRawData(bufferapp);
			 }

			 //Log
			 //El archivo de log se lee de la SD... 'si esta disponible'

			//Boundary
			t+=gtKeeper.SendRawData("\r\n--");
			t+=gtKeeper.SendRawData(boundary);
			t+=gtKeeper.SendRawData("--\r\n");

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

void PostHttpResultCallback(const char* url,int length)
{

	uint16_t contador=0;
	uint8_t readResult=0;
	uint8_t result =-1;

	//Solo esperamos 2 posibles valores OK ó ERR
	if (length==2)
	{
		//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
		if (gtKeeper.SendCommandCheck( F("AT+HTTPREAD=%i,%i"),F("+HTTPREAD:"),contador,length)==RX_CHECK_OK)
		{
			//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
			//+HTTPREAD: 19
			//char *buffer=GetLastResponse();
			//buffer=buffer+10;
			//int serialLen= atoi(buffer);<--Serial len

			gtKeeper.WaitResponse(500);
			//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
			readResult= gtKeeper.ReadSerialLine();
			char *line=gtKeeper.GetLastResponse();

			LOG_INFO_ARGS("Web=>%s",line);

			if (readResult==RX_OK_READ && strcmp_P(line,PSTR("OK"))==0 ) //Mientras no haya errores de procesamiento
			{
				result =LOAD_WEB_OK;
				//Si es ok.. inicializamos contador .. :)
				 
			}



		}
	}

}

void wakeUpUserWeb()
{
	int_input_user=true;
}

//CHECK
bool GTKeeper::CheckWeb()
{
	bool result=false;
	time_t nextejecucion=0;
	//Para enviar datos a web debemos chequear
	//1º Que haya datos pendientes de envio
	//2º Que no hay que ejecutar acciones programadas o riegos en XXX minutos

	//Obtenemos las siguientes ejecuciones
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		if (programas[i].Dias != NONE)
		{
			time_t nextejecucion= GetNextEjecucion(i);
		}
	}
	
	//Comprobamos que estamos en el rango de seguridad
	//Es más importante el riego que el envio de datos
	if (nextejecucion==0 || nextejecucion>(now()+WEB_NOACTIVITY_SECONDS))
	{
		
	}

	return result;
}


//ACCION
void GTKeeper::OnWeb()
{
	//MArca de tiempo
	time_t time=now();

	// Allow wake up pin to trigger interrupt on low.
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT), wakeUpUserWeb, HIGH);
		 
	//A enviar ..
  	memset(buffer,0,MAIN_BUFFER_SIZE);
	sprintf(buffer,SETTING_URL_PROGRAMACION,config.Imei);
	//Si ok
	if (URLRequest(buffer,false,PostHttpParametersCallback,PostHttpResultCallback))
	{
			//reiniciamos contador
			t_last_web = now();
			error_web=0;
	}
	else
		error_web++;
 

	#ifdef SMS
	//Si se producen más errores de la cuenta
	if (error_web>MAX_ERROR_WEB)
	{
		SmsOpen(config.MovilAviso);
		SmsMessage_P(PSTR("Tras varios intentos no se puede acceder a la programacion web :(\nIntente sincronizar mas tarde."));
		SendSmsHora();
		//SendSmsProgramacion();
		SmsSend();
				 
		CargaConfigWeb();
 
		error_web=0;
	}
	#endif

	// Disable external pin interrupt on wake up pin.
	detachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT));
}

//SALE
void GTKeeper::OnLeaveWeb()
{
	//Apagamos
	screenManager.Apagar();
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}


//Carga los parametros de configuración web para que el modulo pueda conectar a internet
void GTKeeper::CargaConfigWeb()
{

		ConfigAPN(F("CONTYPE"),"GPRS");


		SendCommandCheckError(F("AT+CSTT=\"%s\",\"%s\",\"%s\""),(const __FlashStringHelper*)ATSerial::AT_OK,(const __FlashStringHelper*)ATSerial::AT_ALL_ERRORS,config.APN,config.userAPN,config.pwdAPN);

		if (strlen(config.APN)>0)
			ConfigAPN(F("APN"),config.APN);

		//--Configuracion APN
		if (strlen(config.userAPN)>0)
			ConfigAPN(F("USER"),config.userAPN);

		//--UserAPN
			if (strlen(config.pwdAPN)>0)
			ConfigAPN(F("PWD"),config.pwdAPN);


}

void GTKeeper::CheckWebConfig()
{
	//Comprobamos si es necesario refrescar la configuración
	//Realizamos la peticion para que nos responda con la configuracion actual
	bool result=GetConfig4Web(SETTING_URL_CONFIG);
}

bool GTKeeper::GetConfig4Web(char *url)
{
	return URLRequest(url,true,NULL,ConfigHttpResultCallbackStatic);
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
	if (gtKeeper.SendCommandCheck( "AT+HTTPREAD=%i,%i","+HTTPREAD:",contador,LEN_CONFIG_STRING)==RX_CHECK_OK)
	{
		//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
		//+HTTPREAD: 19
		//char *buffer=GetLastResponse();
		//buffer=buffer+10;
		//int serialLen= atoi(buffer);<--Serial len
		gtKeeper.WaitResponse(500);
		//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
		readResult= gtKeeper.ReadSerialLine();
		char *line=gtKeeper.GetLastResponse();
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
	return URLRequest(url,true,NULL,DefaultHttpResultCallbackStatic);
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
				if (SendCommandCheck( "AT+HTTPREAD=%i,%i","+HTTPREAD:",contador,LEN_PROGRAMA_STRING_CR_LF)==RX_CHECK_OK)
				{
					//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
					//+HTTPREAD: 19
					//char *buffer=GetLastResponse();
					//buffer=buffer+10;
					//int serialLen= atoi(buffer);<--Serial len
					WaitResponse(500);
					//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
					readResult= ReadSerialLine();
					char *line=GetLastResponse();

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
								ResetPrograma(i);

								LOG_INFO_ARGS("Programa %i",i);
								//Guardamos en SIM los programas cargados

								if (!GrabarProgramaAEEPROM(i))
									LOG_ERROR("Error grabando a sim");
							}

							//Borra todos programas ;)
							/*while (contador<=SIM_MAX_ENTRIES)
							{
								BorrarContactoSIM(contador);
								contador++;
							}*/
							result=LOAD_WEB_OK ;

						}
						else if (strlen(line)==LEN_PROGRAMA_STRING)
						{
							if (program<MAX_PROGRAMAS)
							{
								//Tiene que venir una linea de programación de sector :)
								if (!CargaProgramaDesdeString(program,line))
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


			//Apagado de sectores ¿?
			//ApagarRiegos();



			if (result==LOAD_WEB_OK)
			{
				LOG_INFO_ARGS("Programas leidos %i",program+1);
				for (uint8_t i=0;i<program;i++)
				{
					//Los otros programas los dejamos a cero

					LOG_INFO_ARGS("Programa %i",i);
					//Guardamos en SIM los programas cargados
					if (!GrabarProgramaAEEPROM(i))
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
					ResetPrograma( program);
					if (!GrabarProgramaAEEPROM(program))
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
						SmsOpen(config.MovilAviso);
						SmsMessage_P(PSTR("Cargada programacion\n"));
						SendSmsHora();
						SendSmsProgramacion();
						SmsSend();
					}

					//Registramos en la web que la carga del programa esta OK!!
					//GetURL()
				}
		#endif
			}

			if (result!=LOAD_WEB_OK)
			{
				//Reseteamos los programas
				for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
				{
					//Los otros programas los dejamos a cero
					ResetPrograma(i);

					LOG_INFO_ARGS("Programa %i",i);
					//Guardamos en SIM los programas cargados

					if (!GrabarProgramaAEEPROM(i ))


						LOG_ERROR("Error grabando a sim");
				}




		#ifdef SMS
				//Algo ha ido mal :(((
				Sms(config.MovilAviso,PBB(F("Error cargando programas - %i"),result));
		#endif
			}


			LOG_DEBUG("FIN gETWEB");
	}

}

