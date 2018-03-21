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

	const char  boundary[] PROGMEM= "---------------------------8278697928671";
	const char  guion_post[] PROGMEM= "--";

	memset(bufferapp,0,MAIN_BUFFER_SIZE);
	strcpy_P(bufferapp,boundary);

	if (GSMModem.SendCommandCheck( F("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=%s\""),(const  __FlashStringHelper*) ATSerial::AT_OK,bufferapp)==RX_CHECK_OK)
	{
		//Cambios de configuracion¿?
		//Cambios de programacion ¿?
		//Cambios en salidas activas
		//Logs..¿?
		uint8_t numsalidas= Riego.GetSalidasActivas();
		uint32_t logsize=SDCard.SizeLogs();
 
		//191 es el constante, boundarys etc etc
 		int16_t totalLen=191 + 
		LEN_CONFIG_STRING_CR_LF +LEN_SEPARATOR_WEB_ITEM +//CONFIGURACION
		(LEN_PROGRAMA_STRING_CR_LF * MAX_PROGRAMAS)+LEN_SEPARATOR_WEB_ITEM + //PROGRAMAS
		(numsalidas==0?0:(LEN_SALIDA_STRING_CR_LF * numsalidas)+LEN_SEPARATOR_WEB_ITEM)+ //SALIDAS
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

		if (GSMModem.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"), totalLen)==RX_CHECK_OK)
		{

			delay(500);

			uint8_t t=0;
			//t+=SendRawData("Accept-Encoding: deflate");
			//Vomitamos lo que hayamos registrado
			//Boundary
			t=GSMModem.SendRawData(guion_post);
			t+=GSMModem.SendRawData(boundary);
			t+=GSMModem.SendRawData(CRLF);

			//Cabecera
			t+=GSMModem.SendRawData_P(PSTR("Content-Disposition: form-data; name=\"submitted\"; filename=\"abcd.txt\""));
			t+=GSMModem.SendRawData(CRLF);

			t+=GSMModem.SendRawData_P(PSTR("Content-Type: text/plain"));
			t+=GSMModem.SendRawData_P(CRLF);
			t+=GSMModem.SendRawData_P(CRLF);

			//Ahora enviamos todo lo que haya en estadisticas
			//t+=SendRawData("abcd\r\n");
			 
			 //Config
			 memset(bufferapp,0,MAIN_BUFFER_SIZE);
			 Config.ConfiguracionToString(bufferapp);
			 t+=GSMModem.SendRawData(bufferapp);
			 t+=GSMModem.SendRawData_P(CRLF);

			 //Programas
			 for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
			 {
				memset(bufferapp,0,MAIN_BUFFER_SIZE);
				Riego.ProgramaToString(i,bufferapp);
				t+=GSMModem.SendRawData(bufferapp);
				t+=GSMModem.SendRawData_P(CRLF);
			 }

			 //Salidas
			 for (uint8_t i=0;i<numsalidas;i++)
			 {
				 memset(bufferapp,0,MAIN_BUFFER_SIZE);
				 Riego.SalidaToString(i,bufferapp);
				 t+=GSMModem.SendRawData(bufferapp);
				 t+=GSMModem.SendRawData_P(CRLF);
			 }

			 //Log
			 //El archivo de log se lee de la SD... 'si esta disponible'

			//Boundary
			t+=GSMModem.SendRawData_P(CRLF);
			t+=GSMModem.SendRawData_P(guion_post);
			t+=GSMModem.SendRawData_P(boundary);
			t+=GSMModem.SendRawData_P(guion_post);
			t+=GSMModem.SendRawData_P(CRLF);


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
		if (GSMModem.SendCommandCheck( F("AT+HTTPREAD=%i,%i"),F("+HTTPREAD:"),contador,length)==RX_CHECK_OK)
		{
			//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
			//+HTTPREAD: 19
			//char *buffer=GetLastResponse();
			//buffer=buffer+10;
			//int serialLen= atoi(buffer);<--Serial len

			GSMModem.WaitResponse(500);
			//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
			readResult= GSMModem.ReadSerialLine();
			char *line=GSMModem.GetLastResponse();

			LOG_INFO_ARGS("Web=>%s",line);

			if (readResult==RX_OK_READ && strcmp_P(line,ATSerial::AT_OK)==0 ) //Mientras no haya errores de procesamiento
			{
				 //Todo OK!!;)
				 gtKeeper.UpdateWebSuccess();			 
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
	//Para enviar datos a web debemos chequear
	//1º Que no hayamos tenido un error de envio hace menos de XXX
	//2º Que haya datos pendientes de envio (De config, programas o  salidas ) ó se cumpla leadtime envios de logs, tb se conectara si el usuario ha forzado la conexion
	//3º Que no hay que ejecutar acciones programadas o riegos en XXX minutos

	if (ELAPSED_SECONDS(GetLastTimeWebError())> (WEB_ERROR_SEND_TIME * GetErroresWeb()))
	{

		//Cambios pdtes.
		if (configuracion->GetChangedConfig() || riegos->GetChangedProgramas() || riegos->GetChangedSalidas() || ELAPSED_SECONDS(GetLastWebSync())> WEB_LOG_SEND_TIME )
			//Comprobamos que estamos en el rango de seguridad
			//Es más importante el riego que el envio de datos
			return ( ELAPSED_SECONDS(riegos->GetNextAction()) > -WEB_NOACTIVITY_TIME);//Debe ser negativo ya que GEtNextAction() estara en el futuro o 0
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
	//Apagamos
	screenManager.Apagar();
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

 

