/*
 * hilo.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */



#include "Neoway590.h"

 

 //Neoway590::Neoway590(Stream * stream,uint8_t bootpin):ATSerial(stream) {	
Neoway590::Neoway590(Stream * stream,uint8_t bootpin):WebSockets(stream) {	// TODO Auto-generated constructor stub
	
	this->ringPing=-1;
	this->bootPin=bootpin;
 
	init();
}

 




void Neoway590::init()
{
	_smslength=0;
	
	_status=Neoway590Unknown;
	pinMode(this->bootPin, OUTPUT);
	digitalWrite(this->bootPin,HIGH);

}


void Neoway590::AttachRing(void (*ring)() , uint8_t pin)
{
	if(this->ringPing==-1)
	{
		this->ringPing=pin;

		//pinMode(pin,INPUT);
		delay(100);
		attachInterrupt(digitalPinToInterrupt(pin),ring,FALLING);
	}
}

void Neoway590::DetachRing()
{
	if(this->ringPing!=-1)
	{
		detachInterrupt(digitalPinToInterrupt(this->ringPing));
		this->ringPing=-1;
	}
}

int Neoway590::Apagar() 
{

	this->DetachRing();

	if (EstaArrancado())
	{
		uint8_t result= tNeoway590.SendCommandCheckOK(F("AT+CPWROFF\r"));
		digitalWrite(this->bootPin,LOW);
		delay(300);
		digitalWrite(this->bootPin,HIGH);
		delay(5000);
		if (! EstaArrancado())
		{
			_status= Neoway590Disconnected;
			return true;
		}
		else
			return AT_NO_APAGADO;
			
	}
	else
		return AT_NO_ARRANCADO;
	
}

//buff = (char *) pgm_read_word (&messages [index]);

void Neoway590::SwitchModule() {



	LOG_INFO_ARGS( "Arrancando, pulso %i",this->bootPin);
	digitalWrite(this->bootPin,LOW);
	delay(350);
	digitalWrite(this->bootPin,HIGH);
	ProcessResults();
    SendCommandCheckOK(F("ATE0"));
	


}



///Funcion que si esta en sleep mode salga de el
void Neoway590::WakeUp()
{
	SendCommandCheckOK(F("AT"));
}


int Neoway590::IsReady()
{
	if (EstaArrancado())
	{
		if  (GetStatus()==  Neoway590Ready)
			return true;
		else
			return AT_NO_READY;

	}
	else
		return AT_NO_ARRANCADO;


}


int  Neoway590::ExisteContactoSIM(uint8_t entrySim) 
{
	WakeUp();
	int simReady=SIMEstaLista();
	if (simReady)
	{
		uint8_t result= SendCommandCheckError( F("AT+CPBR=%i"),F("+CPBR"),(__FlashStringHelper *)AT_ERROR_CME_RESPONSE,entrySim);
		if (result==RX_CHECK_OK)
			return AT_RESULT_OK;
		else
			return AT_NO_FOUND_CONTACT;
 	}
	else
		return simReady;
		
}


int  Neoway590::SIMEstaLista() {
	WakeUp();
	int ready=IsReady();
	if (ready==true)
	{
		if (SendCommandCheckError ( F("AT+CPIN?"), F("+CPIN"),(__FlashStringHelper *)AT_ERROR_RESPONSE)==RX_OK_READ)
		{
				char *result=GetLastResponse();

				//+CPIN:READY
				result=result+6;
				if (ExpectedResult(result,F("READY"))==RX_OK_READ)
					return AT_RESULT_OK;
				//+CPIN:SIM PIN -o- //+CPIN:SIM PIN2
				else if (ExpectedResult(result,F("SIM PIN"))==RX_OK_READ)
					return AT_SIM_PIN;
				//+CPIN:SIM PUK -o- //+CPIN:SIM PUK2
				else if (ExpectedResult(result,F("SIM PUK"))==RX_OK_READ)
					return AT_SIM_PUK;
				else  
					return AT_ERROR_UNKNOW;
		}
		else
			return AT_NO_SIMREADY;
	}
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


int Neoway590::GetCobertura() {
	uint8_t rango = 0;
	WakeUp();
	if (SendCommandCheckError( F("AT+CSQ"),F("+CSQ:"),(__FlashStringHelper *)AT_ERROR_CME_RESPONSE)==RX_CHECK_OK)
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
		return AT_ERROR_CME;
}




 

//Devuelve si el Modulo esta activo
bool Neoway590::EstaArrancado() {

	WakeUp();
	int timeout=getTimeout();
	setTimeout(100);
	uint8_t result=SendCommandCheckOK(F("AT"));
	if (result!= RX_CHECK_OK)
	{	
		//Por si acaso , enviamos señal de fin de envio de datos
		SendRawData(EXIT_INPUTMODE);

		//Procesamos resultados 
		ProcessResults();

		//Volvemos a enviar comando AT
		result=SendCommandCheckOK(F("AT"));
		return (result==RX_CHECK_OK);
	
	}
	
	setTimeout(timeout);

	return (result==RX_CHECK_OK);
	
}

//Comprueba si esta registrado
//AT+CREG?
//+CREG: 1,5
int  Neoway590::EstaRegistrado() {

	WakeUp();
	if (EstaArrancado())
	{
		if (SendCommandCheck( F("AT+CREG?"),F("+CREG"))==RX_CHECK_OK)
		{
			//La posicion 9 nos dira
			char * resultado=GetLastResponse();
			if ( ( resultado[9] == '5' ||  resultado[9] == '1'))	
				return true;
			else
				return AT_NO_REGISTRADO;
		}
		else
			return AT_NO_REGISTRADO;
	}
	else
		return AT_NO_ARRANCADO;

}



 

int Neoway590::BorrarContactoSIM(uint8_t index)
{
	WakeUp();
	if (SendCommandCheckError( F("AT+CPBW=%i"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME_RESPONSE,index)==RX_CHECK_OK)
		return AT_RESULT_OK;
	else
		return AT_ERROR_CME;
}

int Neoway590::GuardarEnSIM(uint8_t index,char *phone_number,char *contact_name)
{
	WakeUp();
	if (SendCommandCheck( F("AT+CPBW=%i,\"%s\",129,\"%s\""),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*)AT_ERROR_CME_RESPONSE,index,phone_number,contact_name)==RX_CHECK_OK)
		return AT_RESULT_OK;
	else
		return AT_ERROR_CME;
		
	
}


 

int Neoway590::getIMEI(char * imei)
{
	bool result=false;
	WakeUp();

	SendCommandAsync( F("AT+CGSN"));

	delay(200);

	if (ReadSerialLine()==RX_OK_READ)
	{
		char * response=GetLastResponse();
		if (ExpectedResult(response,(__FlashStringHelper*)AT_ERROR_CME_RESPONSE)==RX_CHECK_OK)
		{
			return AT_ERROR_CME;
		}
		else
		{
			//LOG_DEBUG_ARGS("Respuesta IMEI: %s %i",response,strlen(response));
			if(strlen(response)==14 || strlen(response)==15)
			{

				strcpy(imei,response);
				return AT_RESULT_OK;
			}
		}
	}

	return AT_ERROR_UNKNOW;
}

 


  




//12/12/12 10:30:23
time_t Neoway590::ParseTime(TimeElements &elements,char *charptr)
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

time_t Neoway590::GetTime(TimeElements &elements)
{
	//AT+CCLK?
	//+CCLK: "16/01/11,18:52:57+04"
	WakeUp();
	uint8_t result=SendCommandCheckError ( F("AT+CCLK?"),F("+CCLK"),(__FlashStringHelper *)AT_ERROR_CME_RESPONSE);
	if (result==RX_CHECK_OK)
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
	else if (result==RX_ERROR)
		return AT_ERROR_CME;
	else 
		return AT_ERROR_UNKNOW;
}


Neoway590Status Neoway590::GetStatus()
{
	return _status;
}


bool Neoway590::OnProcessResult( char * respuesta)
{
	//LOG_DEBUG	("Proc-Neoway590");
	bool result=WebSockets::OnProcessResult(respuesta);

	uint8_t resultProc= ATSerial::ExpectedResult(respuesta,F("MODEM:STARTUP"));
	if(resultProc==RX_CHECK_OK)
	{
		_status=Neoway590Starting;
		this->OnStart();
		
	}
	else 
	{
		uint8_t isPcbReady= ATSerial::ExpectedResult(respuesta,F("+PBREADY"));
		if (isPcbReady==RX_CHECK_OK)
		{
			_status=Neoway590Ready;
			this->OnReady();
			
		}
	}


	return result;
}



//-2 Si se produjo un error con el pin
//-1 Si se produjo un error al arrancar el modulo
// 0 Si el modulo quedo activo y con el pin insertado
bool Neoway590::ActivaModulo() {
	uint8_t result =false;


	LOG_INFO("Activa modulo");
	WakeUp();

	if (!EstaArrancado())
		SwitchModule();

	if (EstaArrancado())
	{
		//Sin eco
		SendCommandCheckOK( F("ATE0"));
		//Control hardware
		//SendCommandAsync("AT&K3");
		// set SMS mode to text
		SendCommandCheckOK( F("AT+CMGF=1"));
		
		SendCommandCheckOK( F("AT+CSCS=\"GSM\""));
		
		//This command will alert our GSM shield and now whenever it will receive message
		SendCommandCheckOK( F("AT+CNMI=1,1,0,0,0"));
		SendCommandCheckOK( F("AT+CNMI?"));

		result=true;
	}
	else
		result=false;

	return result;
}


void Neoway590::OnStart()
{
	LOG_DEBUG("Started!");

}

void Neoway590::OnReady()
{
	LOG_DEBUG("OnReady");


	
	//Sin eco
	SendCommandCheckOK( F("ATE0"));
	//Control hardware
	//SendCommandAsync("AT&K3");
	// set SMS mode to text
	SendCommandCheckOK( F("AT+CMGF=1"));
	
	SendCommandCheckOK( F("AT+CSCS=\"GSM\""));
	
	//This command will alert our GSM shield and now whenever it will receive message
	SendCommandCheckOK( F("AT+CNMI=1,1,0,0,0"));
	SendCommandCheckOK( F("AT+CNMI?"));


	//PAra los msjs tiene que estar READY!!!
	/*SendCommandCheckOK(F("AT+CMGD=1,4"));//Borrado de todos los msghs

	//for(int i=0;i<20;i++)
	//SendCommandCheckOK(F("AT+CMGD=%i"),i);


	SendCommandCheckOK( F("AT+CPMS=\"SM\""));
	SendCommandCheckOK( F("AT+CPMS?"));*/
	

}



/*
 *


bool Neoway590::TieneLlamadas()
{

	if (SendCommandCheck("AT+CPAS", "+CPAS:")==RX_CHECK_OK)
	{
		char * resultado=GetLastResponse();
		return (resultado[7]==3 || resultado[7]==4);
	}

}

bool Neoway590::Llamar(callType_t tipo,const char *telefono)
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

bool Neoway590::LlamarDesdeSim(uint8_t entrySim)
{
	char entry[4];
	memset(entry, 0, sizeof(entry));
	sprintf(entry, "%i", entrySim);
	return Llamar(SIM,entry);
}

//-2 No se pudo registrar
//-1 Modulo GSM no se pudo activar
//0 -si todo fue ok
bool Neoway590::LlamarTelefono(const char *telefono) {
	return Llamar(Phone,telefono);
}*/
