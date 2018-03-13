/*
 * hilo.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */



#include "SIM900.h"




SIM900::SIM900(Stream * stream):ATSerial(stream) {	// TODO Auto-generated constructor stub
	init();
}


SIM900::~SIM900() {	// TODO Auto-generated destructor stub
}



void SIM900::init()
{
	_smslength=0;
	_status=Sim900Unknown;
	memset( _numeroSMS,0,NUM_SMS_LENGTH);


	pinMode(PIN_GSM_ON, OUTPUT);

}


//buff = (char *) pgm_read_word (&messages [index]);

void SIM900::SwitchModule() {


	LOG_INFO_B( "Arrancando modulo");

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

    SendCommandAsync(F("ATE0"));
    delay(100);
    SetMode(SleepDisable);

}




void SIM900::WakeUp()
{
	SendCommandAsync("AT");
}

bool SIM900::SetMode(Sim900SleepMode mode)
{
	WakeUp();
	uint8_t result= SendCommandCheck(F("AT+CSCLK=%i"),(__FlashStringHelper*) AT_OK,mode);
	if (result==RX_CHECK_OK)
	{
		CurrentSleepMode=mode;
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


bool SIM900::ExisteContactoSIM(uint8_t entrySim) {
	WakeUp();
	uint8_t result= SendCommandCheck ( F("AT+CPBR=%i"),F("+CPBR"),entrySim);
	return (result==RX_CHECK_OK);
}


bool SIM900::SIMEstaLista() {
	WakeUp();
	return (SendCommandCheck ( F("AT+CPIN?"), F("+CPIN: READY"))==RX_CHECK_OK);
}



/*
 * Posibles devoluciones
 -1 Error...
 0...1   0-Nada
 1...7:  1-Mala
 7...13: 2-Medio Bajo
 14..21: 3-Medio
 22..25: 4-Medio Alto
 26..31: 5-Buena
 99  99-Desconocido
 */
//
//CSQ value is the return value from AT+CSQ command:
//
//AT+CSQ
//+CSQ: 7,99
//+CSQ: RSSI ( Received Signal Strength Indicator in dBm),BER (BITS ERROR RATE)
//
//The first value denoted Signal Quality Measure (SQM, which is treated by gnokii as RFLevel). The second value is Bit Error Rate (BER). Both measurements are used to ascertain the relative quality of the received cellular signal.
//
//The values of SQM should be interpreted in the following way (conversion to RSSI, Received Signal Strength Indicator in dBm, which would be an equivalent to GN_RF_dBm):
//
//    0: -113 or less
//    1: -111
//    2-30: -109 to -53
//    31: -51 or greater
//    99: not present or not measurable
//
//[approximate RSSI (dBm) = (-113) + (2 * CSQ)]
//
//The values of BER should be interpreted in the following way:
//
//    0: less than 0.2%
//    1: 0.2-0.4%
//    2: 0.4-0.8%
//    3: 0.8-1.6%
//    4: 1.6-3.2%
//    5: 3.2-6.4%
//    6: 6.4-12.8%
//    7: more than 12.8%
//    99: not known or not detectable


uint8_t SIM900::GetCobertura() {
	uint8_t rango = 0;
	WakeUp();
	if (SendCommandCheck( F("AT+CSQ"),F("+CSQ:"))==RX_CHECK_OK)
	{
		//La posicion 9 nos dira
		char * respuesta=GetLastResponse();
		char * t1 = strtok(respuesta + 5, ",");
		/*if (t1[0]==' ')
		 t1[0]='0';*/
		/*
		 *
		 0...10 : Mala
		 11..22: Normal
		 23..31: Muy buena
		 *
		 *
		 * */

		int signal = atoi(t1);
		if (signal <= 0)
			rango = 0;
		else if (signal >= 1 && signal <= 7)
			rango = 1;
		else if (signal >= 8 && signal <= 12)
			rango = 2;
		else if (signal >= 13 && signal <= 21)
			rango = 3;
		else if (signal >= 22 && signal <= 29)
			rango = 4;
		else if (signal >= 30 && signal < 99)
			rango = 5;
		else if (signal >= 99)
			rango = 99;

		return rango;

	} else
		return -1;
}



//Devuelve si el Modulo esta activo
bool SIM900::EstaArrancado() {

	WakeUp();
	//Aseguramos que no queda nada de hez
	uint8_t result=SendCommandCheck(F("AT"),(__FlashStringHelper*) AT_OK);

	if (result==RX_CHECK_OK)
	{
		_status=Sim900Ready;
		return true;
	}
	else
	{
		_status=Sim900Unknown;
		return false;
	}

	return (result==RX_CHECK_OK);
}

//Comprueba si esta registrado
//AT+CREG?
//+CREG: 1,5
bool SIM900::EstaRegistrado() {

	WakeUp();
	if (SendCommandCheck( F("AT+CREG?"),F("+CREG"))==RX_CHECK_OK)
	{
		//La posicion 9 nos dira
		char * resultado=GetLastResponse();
		return ( ( resultado[9] == '5' ||  resultado[9] == '1'));
	}
	else
		return false;
}


bool SIM900::ConfigAPN(const __FlashStringHelper * variable,const char* valor)
{

	WakeUp();
	memset(buff_expected,0,AT_BUFFER_SIZE_EXPECTED);
	strcpy_P(buff_expected,(char *)variable);
	return (SendCommandCheck( F("AT+SAPBR=3,1,\"%s\",\"%s\""),(__FlashStringHelper*) AT_OK,buff_expected,valor)==RX_CHECK_OK);
}


bool SIM900::BorrarContactoSIM(uint8_t index)
{
	WakeUp();
	return (SendCommandCheck( F("AT+CPBW=%i"),(__FlashStringHelper*) AT_OK,index)==RX_CHECK_OK);
}

bool SIM900::GuardarEnSIM(uint8_t index,char *phone_number,char *contact_name)
{
	WakeUp();
	return (SendCommandCheck( F("AT+CPBW=%i,\"%s\",129,\"%s\""),(__FlashStringHelper*) AT_OK,index,phone_number,contact_name)==RX_CHECK_OK);
}


void SIM900::SetDefaultSMSNum (char* numero)
{
	if (strlen(numero)==0 || strlen(numero)>NUM_SMS_LENGTH)
		LOG_CRITICAL_ARGS("Numero Def SMS %s",numero);

	strncpy(_numeroSMS,numero,NUM_SMS_LENGTH);

}

bool SIM900::getIMEI(char * imei)
{
	bool result=false;
	WakeUp();
	SendCommandAsync( F("AT+CGSN"));
	delay(200);

	if (ReadSerialLine()==RX_OK_READ)
	{
		char * response=GetLastResponse();

		//LOG_DEBUG_ARGS("Respuesta IMEI: %s %i",response,strlen(response));
		if(strlen(response)==14 || strlen(response)==15)
		{

			strcpy(imei,response);
			bool result=true;
		}
	}

	return result;
}



Sim900Status SIM900::GetStatus()
{
	return _status;
}





uint8_t SIM900::Sms_P(const char* numero,PGM_P message) {

	uint8_t result=SmsOpen(numero);

	if (result==RX_CHECK_OK)
	{
		SmsMessage_P(message);
		return SmsSend();

	}
	 return result;
}


uint8_t SIM900::Sms(const char* numero,const   char* message) {

	uint8_t result=SmsOpen(numero);

	if (result==RX_CHECK_OK)
	{
		SmsMessage(message);
		return SmsSend();

	}
	 return result;
}



uint8_t SIM900::SmsOpen(const  char* numero)
{
	//Solo devuelve ->'> ' sin CRLF y el resultado es RX_NOD_DATA
	//Por lo que utilizaremos el modo comando directamente

	if (_status!=Sim900SMS)
	{
		WakeUp();
		//Reseteamos contador
		_smslength=0;

		//Copiamos el numero a enviar el sms, por si lo tenemos que trozear el msj ..ver SmsMessage Funtion
		strcpy(_numeroSMS,numero);

		SendCommandAsync( F("AT+CMGS=\"%s\""),_numeroSMS);


		WaitResponse(getTimeout());

		delay(100);
		ProcessResults();

		char * response= GetLastResponse();
		if (strcmp_P(response,PSTR("> "))==0)
		{
			_status=Sim900SMS;
			return RX_CHECK_OK;
		}
		else
			return RX_NO_DATA;
	}
	else
		return SMS_OPEN_ALREADY;

}


uint8_t SIM900::SmsMessage_P(PGM_P msg)
{

	if (_status!=Sim900SMS && strlen(_numeroSMS)>0)
	{
		SmsOpen(_numeroSMS);
	}



	if (_status==Sim900SMS)
	{

		//Si con el nuevo SMS se pasa de la longitud max. para el sms entonces enviamos el contenido
		//actual del mensaje y abrimos otro ;)
		if (_smslength+strlen_P(msg)>=MAX_CHARACTERS_SMS)
		{
			if (_smslength>0)
			{
				SmsSend();
				SmsOpen(_numeroSMS);
			}
		}

		//Añadimos al actual msg el texto..
		_smslength+=strlen_P(msg);
		uint8_t result= SendRawData_P(msg);




		//Cada vez que se envia una linea, el SIM900 envia por puerto serie estos chars '> '
		ProcessResults(200);
		return result;
	}
	else
		return SMS_NO_OPENED;
	// SendCommandCheck(F("AT+CMGDA=\"DEL ALL\""),F("OK"));

}


uint8_t SIM900::SmsMessage(const  char* msg)
{

	if (_status!=Sim900SMS && strlen(_numeroSMS)>0)
	{
		SmsOpen(_numeroSMS);
	}



	if (_status==Sim900SMS)
	{

		//Si con el nuevo SMS se pasa de la longitud max. para el sms entonces enviamos el contenido
		//actual del mensaje y abrimos otro ;)
		if (_smslength+strlen(msg)>=MAX_CHARACTERS_SMS)
		{
			if (_smslength>0)
			{
				SmsSend();
				SmsOpen(_numeroSMS);
			}
		}

		//Añadimos al actual msg el texto..
		_smslength+=strlen(msg);
		uint8_t result= SendRawData(msg);




		//Cada vez que se envia una linea, el SIM900 envia por puerto serie estos chars '> '
		ProcessResults(200);
		return result;
	}
	else
		return SMS_NO_OPENED;
	// SendCommandCheck(F("AT+CMGDA=\"DEL ALL\""),F("OK"));

}


uint8_t  SIM900::SmsSend()
{

	const char ctrlz=0x1A;//Control+Z caracter equivalente a CTRl+z
	const char escape=0x1B;//Escape  caracter equivalente a Escape , para cancelar envio

	uint8_t result=RX_NO_DATA;

	if (_status==Sim900SMS)
	{

		if (_smslength>0)
		{

				// end of message command 1A (hex)*/
			   mSerial->write(ctrlz); // sends ctrl+z end of message

			   //Cada vez que se envia una linea, el SIM900 envia por puerto serie estos chars '> '
			   result= WaitResponseResult(buff_response,"+CMGS",15000);


				//Limpia el ok despues de +CMGS:index<CR><LF>OK
				ProcessResults();

			   //Borramos todos SMS..
			   SendCommandCheck(F("AT+CMGDA=\"DEL ALL\""),(__FlashStringHelper*) AT_OK);



		}
		else
		{
			   mSerial->write(escape); // sends Esc para cancelar msg
			   ProcessResults();
		}

		//Una vez enviado
	   //Reseteamos el contador y status
	   _smslength=0;
		_status=Sim900Ready;
	}
	else
		result=SMS_NO_OPENED;

	   return result;

	   ///return SendCommandCheck("%x","OK",ctrlz);
}




bool SIM900::URLRequest(char *url,bool isGet,bool (*HttpParametersCallback)(),void (*HttpResultCallback)(const char*,int))
{
	bool result=false;
	bool httpinit=false;
	//Modificamos el timeout para darle más tiempo
	int currentimeout=getTimeout();
	setTimeout(10000);
	Sim900SleepMode currentmode=CurrentSleepMode;
	WakeUp();
	SetMode(SleepDisable);


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
		SetMode(currentmode);

		return result;
}








//12/12/12 10:30:23
time_t SIM900::ParseTime(TimeElements &elements,char *charptr)
{
	 if(strlen(charptr)>17)
		 charptr[17]=0;

	 charptr[2]=0;
	 elements.Year=y2kYearToTm(atoi(charptr));

	 charptr=charptr+3;
	 charptr[2]=0;
	 elements.Month=atoi(charptr);

	 charptr=charptr+3;
	 charptr[2]=0;
	 elements.Day=atoi(charptr);

	 charptr=charptr+3;
	 charptr[2]=0;
	 elements.Hour=atoi(charptr);

	 charptr=charptr+3;
	 charptr[2]=0;
	 elements.Minute=atoi(charptr);



	 charptr=charptr+3;
	 elements.Second=atoi(charptr);

	 return makeTime(elements);
}

time_t SIM900::GetTime(TimeElements &elements)
{
	//AT+CCLK?
	//+CCLK: "16/01/11,18:52:57+04"
	WakeUp();

	if (SendCommandCheck ( F("AT+CCLK?"),F("+CCLK"))==RX_CHECK_OK)
	{
			char *respuesta=GetLastResponse();
			  // response in case valid phone number stored:
			  //AT+CCLK?
			  //+CCLK: "16/01/11,18:52:57+04"
			  // <CR><LF>+CPBR: <index>,<number>,<type>,<text><CR><LF>
			  // <CR><LF>OK<CR><LF>

			  // response in case there is not phone number:
			  // <CR><LF>OK<CR><LF>
			 char * charptr=respuesta+8;
			 return ParseTime(elements,charptr);


	}
	else
		return 0;
}


bool  SIM900::GetSIMContact(uint8_t position, char *phone_number,char *contact_name)
{

	char *contactptr;
	char *phoneptr;
	bool result=false;
	WakeUp();
	ProcessResults();


	// response in case valid phone number stored:
	//+CPBR: 1,"653316799",129,"Casa"
	// <CR><LF>+CPBR: <index>,<number>,<type>,<text><CR><LF>
	// <CR><LF>OK<CR><LF>

	// response in case there is not phone number:
	// <CR><LF>OK<CR><LF>


	SendCommandAsync (F("AT+CPBR=%i"), position);

	WaitResponse(AT_DEFAULT_TIMEOUT);
	if (ReadSerialLine()==RX_OK_READ)
	{

		char * line= GetLastResponse();

		//Serial.println(line);
		if (strcmp_P(line, AT_OK)==0)
				result=false;
		else if (strncmp_P(line,PSTR("+CPBR"),5)==0)
		{



			uint8_t contador=0;
			const char *splitchar=",";
			char *token=strtok(line,splitchar);
			//LOG_DEBUG_ARGS("Token %i-> %s",contador,token);
			while (token !=NULL)
			{
					contador++;
					token=strtok(NULL,splitchar);

					if (phone_number != NULL && contador==1)
					{

						strncpy(phone_number,token+1,strlen(token)-2);
						LOG_DEBUG_ARGS("PHONE:%s",phone_number);
					}
					if (contador==3)
					{
						result=true;

						if (contact_name != NULL)
						{

							//Quitamos comillas
							 strncpy(contact_name,token+1,strlen(token)-2);
							 LOG_DEBUG_ARGS("NAME:%s",contact_name);
						}
					}

			 }



		}
	}

	/*

	if (SendCommandCheck ( F("AT+CPBR=%i"),F("+CPBR"), position)==RX_CHECK_OK)
	{


		// response in case valid phone number stored:
							  //+CPBR: 1,"653316799",129,"Casa"
							  // <CR><LF>+CPBR: <index>,<number>,<type>,<text><CR><LF>
							  // <CR><LF>OK<CR><LF>

							  // response in case there is not phone number:
							  // <CR><LF>OK<CR><LF>
							result=true;
							char *respuesta=GetLastResponse();

							if (contact_name != NULL)
							{

								contactptr = strstr((char *)(respuesta),",");

								if (contactptr!=NULL)
								{
									//Obtenemos la posicion
									p_char=respuesta+6;
									if (p_char[0]==' ')
										p_char++;
									position= atoi(p_char);

									contactptr++;
									contactptr= strstr((char *)(contactptr),",");
								}


								if (contactptr!=NULL)
								{
									contactptr++;
									contactptr = strstr((char *)(contactptr),",");


									contactptr++;
									contactptr++;

									p_char1 = strchr((char *)(contactptr),'"');
									if (p_char1 != NULL)
									{
											*p_char1 = 0; // end of string
											strcpy(contact_name,contactptr);
									}
								}

								 result= (strlen(contact_name)!=0);

							}

								if (phone_number != NULL)
								{
									p_char = strstr((char *)(respuesta),",");
									if (p_char != NULL)
									{
										p_char++;
										p_char = strstr((char *)(respuesta),",\"");

										if (p_char != NULL) {
											   p_char++;
											   p_char++;       // we are on the first phone number character
											   // find out '"' as finish character of phone number string
											   p_char1 = strchr((char *)(p_char),'"');
											   if (p_char1 != NULL) {
													*p_char1 = 0; // end of string
											   }
											   // extract phone number string
											   phone_number=strcpy(phone_number, (char *)(p_char));
											   // output value = we have found out phone number string
										  }
									}

									result= (strlen(phone_number)!=0);

				}
	}
	*/
	return result;

}







//-2 Si se produjo un error con el pin
//-1 Si se produjo un error al arrancar el modulo
// 0 Si el modulo quedo activo y con el pin insertado
bool SIM900::ActivaModulo() {
	uint8_t result =false;


	LOG_INFO("Activa modulo");
	WakeUp();

	if (!EstaArrancado())
		SwitchModule();

	if (EstaArrancado())
	{
		//Sin eco
		SendCommandAsync( F("ATE0"));
		//Control hardware
		//SendCommandAsync("AT&K3");
		// set SMS mode to text
		SendCommandAsync( F("AT+CMGF=1"));
		//This command will alert our GSM shield and now whenever it will receive message
		SendCommandAsync( F("AT+CNMI=1,2,0,0,0"));

		result=true;
	}
	else
		result=false;

	return result;
}







/*
 *


bool SIM900::TieneLlamadas()
{

	if (SendCommandCheck("AT+CPAS", "+CPAS:")==RX_CHECK_OK)
	{
		char * resultado=GetLastResponse();
		return (resultado[7]==3 || resultado[7]==4);
	}

}

bool SIM900::Llamar(callType_t tipo,const char *telefono)
{
	bool result = true;
			char buffer[25];

				if (EstaRegistrado()) {
					//--LLamaada
					if (tipo==SIM)
						//Desde SIM
						sprintf(buffer, "ATD>\"SM\"%s;", telefono);
					else
						//Desde tfno
						sprintf(buffer, "ATD\"%s\";", telefono);

					SendCommandAsync(buffer);

					//--Fin llamada
					delay(5000);

					SendCommandAsync("ATH");


				} else {
					result = false;
				}




			return result;
}

bool SIM900::LlamarDesdeSim(uint8_t entrySim)
{
	char entry[4];
	memset(entry, 0, sizeof(entry));
	sprintf(entry, "%i", entrySim);
	return Llamar(SIM,entry);
}

//-2 No se pudo registrar
//-1 Modulo GSM no se pudo activar
//0 -si todo fue ok
bool SIM900::LlamarTelefono(const char *telefono) {
	return Llamar(Phone,telefono);
}*/
