/*
 * Web.cpp
 *
 * Created: 14/03/2018 8:46:53
 *  Author: gtame
 */ 

#include <gtkeeper.h>


int32_t PostHttpParametersCallback()
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

	static const char  boundary[] PROGMEM="---------------------------8278697928671" ;
	static const char  guion_post[] PROGMEM= "--";
	static const uint8_t POST_HEADERS_LEN=191;



	//Cambios de configuracion¿?
	//Cambios de programacion ¿?
	//Cambios en salidas activas
	//Logs..¿?
	uint8_t numsalidas= Riego.GetSalidasActivas();
	uint32_t logsize=SDCard.SizeLogs();
	
	//191 es el constante, boundarys etc etc
	uint32_t totalLen=POST_HEADERS_LEN +
	//CONFIGURACION
	(Config.GetChangedConfig()?LEN_CONFIG_STRING_CR_LF + LEN_SEPARATOR_WEB_ITEM :0) +
	//PROGRAMAS
	(Riego.GetChangedProgramas()?(LEN_PROGRAMA_STRING_CR_LF * MAX_PROGRAMAS) + LEN_SEPARATOR_WEB_ITEM :0) +
	//SALIDAS
	(Riego.GetChangedSalidas()?(LEN_SALIDA_STRING_CR_LF * numsalidas)+LEN_SEPARATOR_WEB_ITEM:0)+
	//LOG
	(logsize==0?0:logsize+LEN_SEPARATOR_WEB_ITEM);

	//El formato del envio del fichero es con este formato
	//+C:CONFIG<CR><LF>
	//+P:PROGRAM1<CR><LF>
	//PROGRAM2<CR><LF>
	//+S:SALIDA1<CR><LF>
	//SALIDA2<CR><LF>
	//+L:LOG1<CR><LF>
	//LOG2<CR><LF>

	

	LOG_DEBUG_ARGS_B("totalLen %i",totalLen);
	//Check si no hay que enviar nada ...
	if (totalLen==POST_HEADERS_LEN)
	return 0;


	memset(bufferapp,0,MAIN_BUFFER_SIZE);
	strcpy_P(bufferapp,boundary);
	


	if (GSMModem.SendCommandCheck( F("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=%s\""),(const  __FlashStringHelper*) ATSerial::AT_OK,bufferapp)==RX_CHECK_OK)
	{
		

		if (GSMModem.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"), totalLen)==RX_CHECK_OK)
		{

			//delay(500);

			uint32_t t=0;
			//t+=SendRawData("Accept-Encoding: deflate");
			//Vomitamos lo que hayamos registrado
			//Boundary
			t=GSMModem.SendRawData_P((const char *)guion_post);
			t+=GSMModem.SendRawData_P((const char *)boundary);
			t+=GSMModem.SendRawData_P((const char *)CRLF);
	 
			//Cabecera
			t+=GSMModem.SendRawData_P(PSTR("Content-Disposition: form-data; name=\"submitted\"; filename=\"abcd.txt\""));
			t+=GSMModem.SendRawData_P(CRLF);

			t+=GSMModem.SendRawData_P(PSTR("Content-Type: text/plain"));
			t+=GSMModem.SendRawData_P(CRLF);
			t+=GSMModem.SendRawData_P(CRLF);

			//Ahora enviamos todo lo que haya en estadisticas
			//t+=SendRawData("abcd\r\n");
			 
			 //Config
			 if (Config.GetChangedConfig())
			 {
				memset(bufferapp,0,MAIN_BUFFER_SIZE);
				strcpy(bufferapp,"C:+");
				t+=GSMModem.SendRawData(bufferapp);
				 memset(bufferapp,0,MAIN_BUFFER_SIZE);
				 Config.ConfiguracionToString(bufferapp);
				 t+=GSMModem.SendRawData(bufferapp);
				 t+=GSMModem.SendRawData_P(CRLF);
			 }

			 //Programas
			 if (Riego.GetChangedProgramas())
			 {

			 	

				 for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
				 {
					 memset(bufferapp,0,MAIN_BUFFER_SIZE);
					 Riego.ProgramaToString(i,bufferapp);
					 t+=GSMModem.SendRawData(bufferapp);
					 t+=GSMModem.SendRawData_P(CRLF);
				 }
			 }


			 //Salidas
			 if (Riego.GetChangedRiegos())
			 {

			 
			 for (uint8_t i=0;i<numsalidas;i++)
			 {
				 memset(bufferapp,0,MAIN_BUFFER_SIZE);
				 Riego.SalidaToString(i,bufferapp);
				 t+=GSMModem.SendRawData(bufferapp);
				 t+=GSMModem.SendRawData_P(CRLF);
			 }
			 }

			 //Log
			 //El archivo de log se lee de la SD... 'si esta disponible'
			 if (logsize>0)
			 {
				SDCard.WriteLogToStream(GSMModem.GetStream());//Escribe el contenido de los logs al stream
			 }

			//Boundary
			t+=GSMModem.SendRawData_P(CRLF);
			t+=GSMModem.SendRawData_P(guion_post);
			t+=GSMModem.SendRawData_P(boundary);
			t+=GSMModem.SendRawData_P(guion_post);
			t+=GSMModem.SendRawData_P(CRLF);


			LOG_DEBUG_ARGS_B("Enviado x post--> %i",t);
			delay(500);
			return t;
		}
		else
		return -1;
	}
	else
	return -1;
}

uint8_t PostHttpResultCallback(const char* url,uint16_t length)
{

	uint16_t contador=0;
	uint8_t readResult=0;
	uint8_t result =LOAD_WEB_OK;
	uint8_t currentprogram=0;

	//Solo esperamos 2 posibles valores OK ó ERR
	//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
	LOG_DEBUG("INI WHILE");

	while (	contador!=length && result==LOAD_WEB_OK ) //Mientras no haya errores de procesamiento
	{
	
		 LOG_DEBUG("LEIDO??");
		//Obtenemos los 3 primeros caracteres, son los que nos marcaran el contenido que vamos a leer
		if (GSMModem.GetHttpBuffer(contador,3,bufferapp))	
		{
			LOG_DEBUG("LEIDOS +3");
			//Incrementamos el contador
			contador+=3;


		 
			if  (strncmp_P(bufferapp,PSTR("R+:"),3)==0) //Respuesta
			{
				if (GSMModem.GetHttpBuffer(contador,1,bufferapp))
				{
					contador+=3;//1 caracter + CRLF caracteres
					//La respuesta es 'O' -> OK
					//La respuesta es 'E' -> ERROR No proceso bien el fichero .. :(
	 
					//Si no es OK
					if (!strncmp_P(bufferapp,PSTR("O"),1)==0)
					{
						if (strncmp_P(bufferapp,PSTR("E"),1)==0)
						result=LOAD_WEB_ERR_SERVER_RESPONSE;
						else
						result=LOAD_WEB_ERR_UNKNOWN_RESPONSE;
					}

				}
				else
				result=LOAD_WEB_ERR_MALFORMED_FILE;
			}
			else if  (strncmp_P(bufferapp,PSTR("C+:"),3)==0) //Config
			{
				
				LOG_DEBUG("CONFIG!");
				//Limpio todo ya que tendre que pasarle el buffer + LEN_CONFIG_STRING_CR_LF
				memset(bufferapp,0,MAIN_BUFFER_SIZE);
				//Leemos configuracion
				if (GSMModem.GetHttpBuffer(contador,LEN_CONFIG_STRING_CR_LF,bufferapp ))
				{
					contador+=LEN_CONFIG_STRING_CR_LF;//Movemos contador
					bufferapp[LEN_CONFIG_STRING]=0;//Para que el CR-LF no sea tenido en cuenta ;)
					LOG_DEBUG_ARGS("LEIDO! %i %s",strlen(bufferapp),bufferapp);
					
					//Si no es OK
					if (strlen(bufferapp)==LEN_CONFIG_STRING)
					{
						if (!Config.CargaConfigDesdeString(bufferapp))
						result=LOAD_WEB_ERR_CANT_LOAD_CONFIG;
					}
					else
						result=LOAD_WEB_ERR_MALFORMED_CONFIG;
				}
				else
				result=LOAD_WEB_ERR_MALFORMED_FILE;

	
			}
			else if  (strncmp_P(bufferapp,PSTR("D+:"),3)==0) //Eliminar programacion
			{

				if (GSMModem.GetHttpBuffer(contador,1,bufferapp))
				{
					contador+=3;//1 caracter + CRLF caracteres
					//La respuesta es 'O' -> OK - Borramos programa
					if (strncmp_P(bufferapp,PSTR("O"),1)==0) 
					{
						LOG_DEBUG_ARGS("Resetea programa %i",currentprogram);
						Riego.ResetPrograma(currentprogram);
						currentprogram++;
					}
					else
						result=LOAD_WEB_ERR_UNKNOWN_DELETE_RESPONSE;
				}
				else
					result=LOAD_WEB_ERR_MALFORMED_FILE;

			}
			else if  (strncmp_P(bufferapp,PSTR("P+:"),3)==0)//Programa
			{
 						
				//Leemos programa
				if (GSMModem.GetHttpBuffer(contador,LEN_PROGRAMA_STRING_CR_LF,bufferapp))
				{
					contador+=LEN_PROGRAMA_STRING_CR_LF;//17 caracter + CRLF caracteres
					bufferapp[LEN_PROGRAMA_STRING]=0;//Para que el CR-LF no sea tenido en cuenta ;)
					LOG_DEBUG_ARGS("LEIDO! %i %s",strlen(bufferapp),bufferapp);
		 
					//Si no es OK
					if (strlen(bufferapp)==LEN_PROGRAMA_STRING)
					{
						if (Riego.CargaProgramaDesdeString(currentprogram,bufferapp))
						currentprogram++;
						else
						result=LOAD_WEB_ERR_CANT_LOAD_PROGRAM;
					}
					else
					result=LOAD_WEB_ERR_MALFORMED_PROGRAM;
				}
				else
				result=LOAD_WEB_ERR_MALFORMED_FILE;
			}
			else
			result=LOAD_WEB_ERR_UNKNOWN_STRING; //Respuesta desconocida

		} 
		else
			result=LOAD_WEB_ERR_UNKNOWN_RESPONSE;
		// Fin de if

		LOG_DEBUG_ARGS("Result %i",result);

	}// fin de while

	LOG_DEBUG("FIN WHILE");

	//Si todo cargo OK -> Guardamos lo que se ha actualizado
	if (result==LOAD_WEB_OK)
	{
		LOG_DEBUG("FICHERO WEB LEIDO CORRECTAMENTE");
		//Guardamos programas
		if (Riego.GetChangedProgramas())
			Riego.GuardarProgramasEEPROM();

		//Guardamos config
		if (Config.GetChangedConfig())
			Config.EEPROMGuardaConfig();


		gtKeeper.UpdateWebSuccess();

		//Avisamos por SMS?
	}
	else
	{
		LOG_DEBUG("FALLO CARGANDO FICHERO WEB");
		//Avisamos por SMS¿?
	}

}

void wakeUpUserWeb()
{
	int_input_user=true;
}

//CHECK
bool GTKeeper::CheckWeb()
{
	//Para enviar datos a web debemos chequear
	//1º Que no hayamos tenido un error de envio hace menos de XXX
	//2º Que haya datos pendientes de envio (De config, programas o  salidas ) ó se cumpla leadtime envios de logs, tb se conectara si el usuario ha forzado la conexion
	//3º Que no hay que ejecutar acciones programadas o riegos en XXX minutos

	if (ELAPSED_SECONDS(GetLastTimeWebError())>= (WEB_ERROR_SEND_TIME * GetErroresWeb()))
	{
		LOG_DEBUG("ENTRA EN Chagned");

		//Cambios pdtes.
		if (configuracion->GetChangedConfig() || riegos->GetChangedProgramas() || riegos->GetChangedSalidas() || ELAPSED_SECONDS(GetLastWebSync())> WEB_LOG_SEND_TIME )
		{
			LOG_DEBUG_ARGS("EINx? %lu %lu",now(),riegos->GetNextAction());
			
			LogTime(riegos->GetNextAction());
			LogTime(now());

			if (riegos->GetNextAction()<now())
				//La ultima accion ya fue ejecutada..
				return true;
			else	
				//La accion esta por ejecutarse
				//Comprobamos que estamos en el rango de seguridad
				//Es más importante el riego que el envio de datos
				return (  (riegos->GetNextAction()-now()) >= WEB_NOACTIVITY_TIME);//Debe ser negativo ya que GEtNextAction() estara en el futuro o 0
		}
	}
	 
	return false;
}


//ACCION
void GTKeeper::OnWeb()
{
 
	// Allow wake up pin to trigger interrupt on low.
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT), wakeUpUserWeb, HIGH);
		 
	//URL con imei como parameter
  	memset(buffer,0,MAIN_BUFFER_SIZE);
	sprintf(buffer,SETTING_URL_PROGRAMACION,config->Imei);

	//Enviamos POST reques
	if (!gsm->URLRequest(buffer,false,PostHttpParametersCallback,PostHttpResultCallback))
		AddErrorWeb();//Logeamos el error web

	#ifdef SMS
	//Si se producen más errores de la cuenta
	if (error_web>MAX_ERROR_WEB)
	{
		gsm->SmsOpen(config.MovilAviso);
		gsm->SmsMessage_P(PSTR("Tras varios intentos no se puede acceder a la programacion web :(\nIntente sincronizar mas tarde."));
		gsm->SendSmsHora();
		//SendSmsProgramacion();
		gsm->SmsSend();
				 
		gsm->CargaConfigWeb();
 
		error_web=0;
	}
	#endif

	// Disable external pin interrupt on wake up pin.
	detachInterrupt(digitalPinToInterrupt(INTERRUPT_USER_INPUT));

}

//SALE
void GTKeeper::OnLeaveWeb()
{
#ifdef PANTALLA_TECLADO
	//Apagamos
	screenManager.Apagar();
#endif

	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}

//Metodo para la actualizacion OK
void GTKeeper::UpdateWebSuccess()
{
	//Reset Contadores, Flags  y Logs
	//Contadores
	SetLastWebSync();
	ClearErrorWebs();
	
	//Flags
	riegos->UpdateWebSuccess();

	//Logs
	logsd->ClearLogs();
}

 

