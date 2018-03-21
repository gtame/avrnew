/*
 * web.cpp
 *
 * Created: 21/03/2018 20:03:03
 *  Author: Gabi
 */ 
 
//Hay que revisar estos metodos

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
	if (gsm->SendCommandCheck( "AT+HTTPREAD=%i,%i","+HTTPREAD:",contador,LEN_CONFIG_STRING)==RX_CHECK_OK)
	{
		//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
		//+HTTPREAD: 19
		//char *buffer=GetLastResponse();
		//buffer=buffer+10;
		//int serialLen= atoi(buffer);<--Serial len
		gsm->WaitResponse(500);
		//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
		readResult= gsm->ReadSerialLine();
		char *line=gsm->GetLastResponse();
		if(strlen(line)==LEN_CONFIG_STRING)
		{
			//Cargamos la config desde string si todo sale OK punto pelota
			configuracion->CargaConfigDesdeString(line);
			configuracion->EEPROMGuardaConfig();
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
								if (!riegos->CargaProgramaDesdeString(program,line))
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

