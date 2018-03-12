/*
 * ATSerial.cpp
 *
 *  Created on: 15 de ene. de 2016
 *      Author: gtame
 */

#include "ATSerial.h"


// PROGMEMs
const char ATSerial::AT_OK[] PROGMEM = "OK";
const char ATSerial::AT_ERROR_RESPONSE[] PROGMEM = "ERROR";
const char ATSerial::AT_ERROR_CME_RESPONSE[] PROGMEM = "+CME ERROR:";
const char ATSerial::AT_ERROR_CMS_RESPONSE[] PROGMEM = "+CMS ERROR:";
const char ATSerial::AT_ALL_ERRORS_RESPONSE[] PROGMEM ="X";
const char ATSerial::AT_ENDLINE[] PROGMEM ="\r\n";

// size_t strlen_P(const char * s)
// int	vsnprintf_P(char *__s, size_t __n, const char *__fmt, va_list ap);




ATSerial::ATSerial(Stream *stream) 
{
	mSerial=stream;
	ProcessResultPtr=NULL;
	ATSerial::setTimeout(AT_DEFAULT_TIMEOUT);


}

ATSerial::~ATSerial() {
	// TODO Auto-generated destructor stub
}


char *   ATSerial::GetNextToken(uint8_t tokenNum, char* splitchar)
{
	return GetToken(NULL,tokenNum,splitchar);

}

//SimComm
char * ATSerial::GetToken(char *respuesta,uint8_t tokenNum, char* splitchar)
{
	uint8_t contador=0;
	char *token;

	if (respuesta)
		token=strtok(respuesta,splitchar);
	else
		token=strtok(NULL,splitchar);

	while (token !=NULL && contador<tokenNum)
	{
			contador++;
			token=strtok(NULL,splitchar);
	}

	return token;

}

char * ATSerial::GetLastResponse()
{
 return buff_response;
}

char * ATSerial::GetLastCommand()
{
	return buff_comando;
}




int8_t ATSerial::ParseCommand(const char * comando,va_list ap)
{
	return _ParseCommand(comando,strlen,vsnprintf,ap);
}


int8_t ATSerial::ParseCommand_P(const __FlashStringHelper * comando,va_list ap)
{
	return _ParseCommand((const char*)comando,strlen_P,vsnprintf_P,ap);
}


int8_t ATSerial::_ParseCommand(const char* comando,strlenfunc strlenPTR,vsnprintffunc vsnprintfPTR,va_list ap)
{

	//Checks previos params
	if (strlenPTR((char *)comando)<=AT_BUFFER_SIZE_COMMAND)
	{
		memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);
		vsnprintfPTR(buff_comando,AT_BUFFER_SIZE_COMMAND,(char *)comando, ap);
		
		va_end(ap);
		return 0;	
	}
	else
		return RX_ERROR_PARAMS;
}


//Devuelve el numero de caracteres transmitidos en el puerto serie
uint8_t  ATSerial::SendCommandAsync(const __FlashStringHelper * comando,...)
{
	va_list ap;
	va_start(ap,comando);
	if (ParseCommand_P(comando,ap)==0)
		return ATSerial::_SendCommandCheck(buff_comando,NULL);
	else
		return RX_ERROR_PARAMS;

}


uint8_t  ATSerial::SendCommandAsync(const char * comando,...)
{
	va_list ap;
	va_start(ap,comando);
	ParseCommand(comando,ap);
	if (ParseCommand(comando,ap)==0)
		return ATSerial::_SendCommandCheck(buff_comando,NULL);
	else
		return RX_ERROR_PARAMS;
}




bool ATSerial::CheckInputMode()
{
	if (WaitResponse(10000)>0)
	{
		ReadSerialLine();
		char *result=GetLastResponse();
		return (strncmp(result,">",1)==0);
	}
	else
		return false;
}

uint8_t ATSerial::SendCommandCheck(const __FlashStringHelper * __fmt,const __FlashStringHelper *response, ...)
{

		//Checks previos params
	 	if (strlen_PF(response)>AT_BUFFER_SIZE_EXPECTED)	return RX_ERROR_PARAMS;

		va_list ap;
		va_start(ap,__fmt);
		if (ParseCommand_P(__fmt,ap)==0)
			return ATSerial::_SendCommandCheck(buff_comando,response);
		else
			return RX_ERROR_PARAMS;


}


uint8_t ATSerial::SendCommandCheck( const char *__fmt,const __FlashStringHelper * expectedResult, ...)
{
	//Checks previos params
	if (strlen_PF(expectedResult)>AT_BUFFER_SIZE_EXPECTED)	return RX_ERROR_PARAMS;

	va_list ap;
	va_start(ap,__fmt);
	if (ParseCommand(__fmt,ap)==0)
		return ATSerial::_SendCommandCheck(buff_comando,expectedResult);
	else
		return RX_ERROR_PARAMS;

}






uint8_t ATSerial::SendCommandCheckOK(const __FlashStringHelper * __fmt, ...)
{

	va_list ap;
	va_start(ap,__fmt);
	if (ParseCommand_P(__fmt,ap)==0)
		return _SendCommandCheck(buff_comando,(__FlashStringHelper *) ATSerial::AT_OK,(__FlashStringHelper *) ATSerial::AT_ALL_ERRORS_RESPONSE);
	else
		return RX_ERROR_PARAMS;
}



uint8_t ATSerial::SendCommandCheckOK(const char *__fmt, ...)
{

	va_list ap;
	va_start(ap,__fmt);
	if (ParseCommand(__fmt,ap)==0)
	return _SendCommandCheck(buff_comando,(__FlashStringHelper *) ATSerial::AT_OK,(__FlashStringHelper *) ATSerial::AT_ALL_ERRORS_RESPONSE);
	else
	return RX_ERROR_PARAMS;

	return _SendCommandCheck(buff_comando,(__FlashStringHelper *) ATSerial::AT_OK,(__FlashStringHelper *) ATSerial::AT_ALL_ERRORS_RESPONSE);
}



uint8_t ATSerial::SendCommandCheckError( const __FlashStringHelper * __fmt,const __FlashStringHelper *response,const __FlashStringHelper *error,  ...)
{

	//Checks previos params
	if (strlen_PF(response)>AT_BUFFER_SIZE_EXPECTED)	return RX_ERROR_PARAMS;
	if (strlen_PF(error)>AT_BUFFER_SIZE_EXPECTED)	return RX_ERROR_PARAMS;

	va_list ap;
	va_start(ap,__fmt);
	if (ParseCommand_P(__fmt,ap)==0)
		return ATSerial::_SendCommandCheck(buff_comando,response,error);
	else
		return RX_ERROR_PARAMS;
}



size_t ATSerial::SendEndLine()
{
	
	LOG_DEBUG("<CR><LF>");
	return mSerial->println();
}

size_t ATSerial::SendRawData( const __FlashStringHelper * __fmt, ...)
{
	va_list ap;
	va_start(ap,__fmt);
	if (ParseCommand_P(__fmt,ap)==0)
	{
			LOG_DEBUG_ARGS("TX->%s",buff_comando);
			return mSerial->write(buff_comando,strlen(buff_comando));
	}
	else
		return -1;
}

size_t ATSerial::SendRawData(char  data)
{
	
	return mSerial->print(data);
}



size_t ATSerial::SendRawData(const char * data,...)
{

	va_list ap;
	va_start(ap,data);
	if (ParseCommand(data,ap)==0)
	{
		LOG_DEBUG_ARGS("TX->%s",data);
		return mSerial->write(data,strlen(data));
	}
	else
	return -1;


}





uint8_t ATSerial::_SendCommandCheck(const char * command,const __FlashStringHelper *expectedResult)
{
	return _SendCommandCheck(command,expectedResult,( __FlashStringHelper *)NULL);
}

uint8_t ATSerial::_SendCommandCheck(const char * command,const __FlashStringHelper *expectedResult,const __FlashStringHelper *errorResult)
{

	uint8_t result=RX_NO_DATA;
 	ProcessResults();

 	LOG_DEBUG_ARGS("TX->%s",command);
	//Ejecuta el comando
	result=mSerial->println(command);
 
	if (expectedResult!=NULL)
	{
		//Espera hasta que tengamos el expected result
		result=WaitResponseResult(buff_response,expectedResult,errorResult,getTimeout());
	}

	return result;
}



void ATSerial::ProcessResults(uint16_t wait)
{
 	uint8_t result=RX_NO_DATA;
 	do
 	{
		 //Esperamos respuesta
		 WaitResponse(wait);

		 //Si la lectura ha ido bien lo mandamos a procesar
		 if ((result=ReadSerialLine(buff_response))==RX_OK_READ)
			 OnProcessResult(buff_response);

	 }while (result!=RX_NO_DATA);

}





bool ATSerial::OnProcessResult( char * respuesta)
{
	//LOG_DEBUG	("Proc-ATSerial");
	if (ProcessResultPtr!=NULL)
		return ProcessResultPtr(respuesta);
	else
		return true;

}


uint8_t ATSerial::ExpectedResult (const char * resultado,const __FlashStringHelper * expectedResult)
{

	 //Leemos linea
	if (resultado!=NULL && expectedResult!=NULL )
	{
		if (strncmp_PF(resultado,expectedResult,strlen_PF(expectedResult))==0)
			return RX_CHECK_OK;
		else
			return RX_CHECK_KO;
	}
	else
		return RX_CHECK_KO;
}


uint8_t ATSerial::WaitResponseResult(char * resultado,const __FlashStringHelper   *expectedResult, const __FlashStringHelper *errorResult,uint16_t wait)
{

 	uint8_t result=RX_NO_DATA;

 	do
 	{
		 //Esperamos respuesta
		 WaitResponse(wait);

		 //Comprobamos lectura
		 if ((result=ReadSerialLine(resultado))==RX_OK_READ)
		 {
			
			 result=ExpectedResult (resultado,expectedResult);
			
			 if (result==RX_CHECK_KO && errorResult!=NULL && result!=NULL)
			 {

				 //Esto quiere decir que chequeemos cualquier tipo de error
				 if (errorResult==(__FlashStringHelper *) ATSerial::AT_ALL_ERRORS_RESPONSE)
				 {
					 //Chequeamos todos los errores posibles
					 if
					 (
					 strncmp_P(resultado, AT_ERROR_RESPONSE,strlen_P(ATSerial::AT_ERROR_RESPONSE))==0 ||
					 strncmp_P(resultado, AT_ERROR_CME_RESPONSE,strlen_P(AT_ERROR_CME_RESPONSE))==0 ||
					 strncmp_P(resultado, AT_ERROR_CMS_RESPONSE,strlen_P(AT_ERROR_CMS_RESPONSE))==0
				      )
						 result=RX_ERROR;
				 }
				 else
				 {
					
					 result=ExpectedResult (resultado,errorResult);
					 if (result==RX_CHECK_OK)
						 result=RX_ERROR;

				 }
			 }

		 }
		 
		 if (result==RX_CHECK_KO)
			 OnProcessResult(resultado);

	 }while ( result!=RX_NO_DATA && result!=RX_CHECK_OK && result!=RX_ERROR);

	 return result;
}


 

bool ATSerial::CheckChar(const char chr,uint16_t wait)
{
// &&  ((char))==chr
	
	if (WaitResponse(wait)==1 )
		return (chr==(char)mSerial->read());
	else
		return false;

}



uint8_t ATSerial::WaitResponseResult(const __FlashStringHelper * expectedResult,const __FlashStringHelper *error, uint16_t wait)
{

		memset(buff_response,0,AT_BUFFER_SIZE_RX);
		return WaitResponseResult(buff_response,expectedResult,error,wait);
}





int ATSerial::WaitResponse( uint16_t wait)
{
	unsigned long t_inicial = millis();

	while ((millis() - t_inicial) < wait && mSerial->available() == 0) {	delay(1);	}

	return mSerial->available();
}



uint8_t ATSerial::ReadSerialLine()
{
	return ATSerial::ReadSerialLine(buff_response,AT_BUFFER_SIZE_RX);
}
//Lee una linea del puerto serie
//Returns
//0 OK
//1 No data
//2 Size buffer overflow (La linea recibida es mas grande que el buffer)
uint8_t ATSerial::ReadSerialLine(char *respuesta, uint8_t size) {

	uint8_t result=RX_NO_DATA;
	uint8_t contador=0;
	char readcharAnt=0;

	//mSerial->flush();

	if (size>0)
	{
		memset(respuesta, 0, size);

		while (mSerial->available() &&  result==RX_NO_DATA) {

			char readchar = (char) mSerial->read();

			if (readchar != 10 && readchar != 13)
			{
				//Oh oh llegamos al overflow!!!
				if (contador >= size)
					result=RX_OVERFLOW_BUFFER;
				else
				{
					respuesta[contador] = readchar;
					contador++;
				}

			}
			//Si encontramos final de linea
			else if  (readchar == 10 && contador>0 && readcharAnt==13 )
				result=RX_OK_READ;


			if (result==RX_NO_DATA)
			{
				//Aqui en ocasiones se queda pillado
				//No se si es por memoria ..
				uint8_t contdog=0;
				while (mSerial->available()==0 && contdog<100){	delay(10);contdog++;}
			}

			readcharAnt=readchar;
		}


		if (respuesta!=NULL && strlen(respuesta)>0)
			{
			LOG_INFO_ARGS_B("RX %i->%s ",result,respuesta);
			}
			

	}


	return result;
}

