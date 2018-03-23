/*
 * hilo.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */



#include "SIM900.h"




SIM900::SIM900(Stream * stream):ATModem(stream) {	// TODO Auto-generated constructor stub
	_sleepmode=  SleepDisable;
	pinMode(PIN_GSM_ON, OUTPUT);
}


/////// METHODS POWER

void SIM900::WakeUp()
{

	//para leventar hay que poner el pin DTR a low
	digitalWrite(GSM_SLEEP_PIN, LOW);
	delay(100);
	 
}

void SIM900::Sleep()
{

	//para leventar hay que poner el pin DTR a low
	digitalWrite(GSM_SLEEP_PIN, LOW);
	delay(100);
	
}

void SIM900::PowerOff()
{
 	LOG_INFO( "Parando modulo");

 	#if !defined(PROTEUS) //&& !defined(DEBUG)
 	LOG_INFO_ARGS( "Activando pulso pin %i",PIN_GSM_ON);
 	pinMode(PIN_GSM_ON, OUTPUT);
 	digitalWrite(PIN_GSM_ON,LOW);
 	delay(1000);
 	digitalWrite(PIN_GSM_ON,HIGH);
 	delay(1000);
 	digitalWrite(PIN_GSM_ON,LOW);
 	delay(3000);
 	#endif
}

void SIM900::PowerOn()
{
	
	//Levantamos
	WakeUp();
	
	//Vemos si responde
	
	if (!EstaArrancado())
	{
		LOG_INFO( "Arrancando modulo");

		#if !defined(PROTEUS) //&& !defined(DEBUG)
		LOG_INFO_ARGS( "Activando pulso pin %i",PIN_GSM_ON);
		pinMode(PIN_GSM_ON, OUTPUT);
		digitalWrite(PIN_GSM_ON,LOW);
		delay(1000);
		digitalWrite(PIN_GSM_ON,HIGH);
		delay(1000);
		digitalWrite(PIN_GSM_ON,LOW);
		delay(3000);
		#endif
	}
	
	SendCommandAsync(F("ATE0"));
	delay(100);
	SetSleepMode(_sleepmode);
	//Sin eco
	SendCommandAsync( F("ATE0"));
	//Control hardware
	//SendCommandAsync("AT&K3");
	// set SMS mode to text
	SendCommandAsync( F("AT+CMGF=1"));
	//This command will alert our GSM shield and now whenever it will receive message
	SendCommandAsync( F("AT+CNMI=1,2,0,0,0"));
	
}

/////// ENDMETHODS

 bool SIM900::GetHttpBuffer(uint8_t desde,uint8_t length)
 {
	return (SendCommandCheck( F("AT+HTTPREAD=%i,%i"),F("+HTTPREAD:"),desde,length)==RX_CHECK_OK);
 }
 

bool SIM900::SetSleepMode(Sim900SleepMode mode)
{
	WakeUp();
	uint8_t result= SendCommandCheck(F("AT+CSCLK=%i"),(__FlashStringHelper*) AT_OK,mode);
	if (result==RX_CHECK_OK)
	{
		_sleepmode=mode;
		return true;
	}
	else
		return false;
}

bool SIM900::GPIOWrite(uint8_t pin,uint8_t mode)
{
	/*
	AT+SGPIO=<operation>,<GPIO>,<function>,<level> */
	WakeUp();
	uint8_t result= SendCommandCheck(F("AT+SGPIO=0,%i,1,%i"),(__FlashStringHelper*) AT_OK,pin,mode);
	return (result==RX_CHECK_OK);
}


bool SIM900::ConfigAPN(const __FlashStringHelper * variable,const char* valor)
{

	WakeUp();
	memset(buff_expected,0,AT_BUFFER_SIZE_EXPECTED);
	strcpy_P(buff_expected,(char *)variable);
	return (SendCommandCheck( F("AT+SAPBR=3,1,\"%s\",\"%s\""),(__FlashStringHelper*) AT_OK,buff_expected,valor)==RX_CHECK_OK);
}


bool SIM900::URLRequest(char *url,bool isGet,bool (*HttpParametersCallback)(),void (*HttpResultCallback)(const char*,int))
{
	bool result=false;
	bool httpinit=false;
	//Modificamos el timeout para darle más tiempo
	int currentimeout=getTimeout();
	setTimeout(10000);
	WakeUp();
 
	//Abrimos el ID de conexion 1
		if (SendCommandCheckError( F("AT+SAPBR=1,1"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)ATSerial::AT_ALL_ERRORS)==RX_CHECK_OK)
		{
			//Consultamos si ha asignado IP
			if (SendCommandCheckError( F("AT+SAPBR=2,1"),F("+SAPBR"),(__FlashStringHelper*)ATSerial::AT_ALL_ERRORS)==RX_CHECK_OK)
			{

			 //Iniciamos peticion Http
			if(SendCommandCheckError( F("AT+HTTPINIT"),(__FlashStringHelper*)AT_OK,(__FlashStringHelper*)AT_ERROR_CME)==RX_CHECK_OK)
			{
				httpinit=true;

				 //Indicamos que vamos a utilizarla conexion1 para realizar la peticion web
				if (SendCommandCheckError( F("AT+HTTPPARA=\"CID\",1"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME)==RX_CHECK_OK)
				{
					 //Fijamos URl de conexion
					if (SendCommandCheckError( F("AT+HTTPPARA=\"URL\",\"%s\""),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME,url)==RX_CHECK_OK)
					{


						//Para get no haria falta ningun header adicional
						//Para post -> seria algo tal que si.. (Ejemplo enviar fichero)
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
						//
						//Content-Type: text/plain
						//
						//Hello Ravi
						//
						//------WebKitFormBoundaryvZ0ZHShNAcBABWFy--
						//OK
						if (HttpParametersCallback!=NULL)
							HttpParametersCallback();

						//AT+HTTPACTION=%i -> Action 0 = GET- 1 = POST
						if (SendCommandCheckError( F("AT+HTTPACTION=%i"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ALL_ERRORS,(isGet?0:1))==RX_CHECK_OK)
						{
							//Alguna vez ocurre que no responde con +HTTPACTION
							if (WaitResponseResult(buff_response,"+HTTPACTION",10000)==RX_CHECK_OK)
							{
								//+HTTPACTION:0,200,<length>
								//delay(4000);
								char *buffer=GetLastResponse();
								char *httplen=GetToken(buffer,2,",");
								int serialLen= atoi(httplen);

								//Para bien tendria que devover un resultado
								//para saber si la respuesta es ok o no..
								if (HttpResultCallback!=NULL)
									HttpResultCallback(url,serialLen);

								result=true;

							}
						}
						else
							LOG_ERROR_ARGS("%s",GetLastCommand());
					}
					else
						LOG_ERROR_ARGS("%s",GetLastCommand());
				}
			else
				LOG_ERROR_ARGS("%s",GetLastCommand());
			}
			else
				LOG_ERROR_ARGS("%s",GetLastCommand());
			}
			else
				LOG_ERROR_ARGS("%s",GetLastCommand());
		}
		else
			LOG_ERROR_ARGS("%s",GetLastCommand());



		//setTimeout(1000);
		 //Finalizamos peticion Http
		if (httpinit)
			SendCommandCheckError( F("AT+HTTPTERM"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME);


		delay(100);
		//Cerramos el ID de conexion 1¿?
		SendCommandCheckError( F("AT+SAPBR=0,1"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME);

		//setTimeout(AT_DEFAULT_TIMEOUT);
		setTimeout(currentimeout);
 
		return result;
}


bool  SIM900::GenerarPulsoDTMF(char caracter,uint16_t durationseg=1)
{


	return (SendCommandCheckError(F("AT+CLDTMF=%i,\"%c\",%i,2"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*) AT_ERROR,durationseg,caracter,durationseg*100)==RX_CHECK_OK);
}

bool SIM900::PasarDTMFMode()
{
	
	return (SendCommandCheck(F("AT+DDET=1,1000,0,0"),(__FlashStringHelper*) AT_OK)==RX_CHECK_OK);
}