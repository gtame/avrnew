/*
 * ATSerial.cpp
 *
 *  Created on: 15 de ene. de 2016
 *      Author: gtame
 */

#include "ATSerial.h"




const char ATSerial::AT_OK[] PROGMEM = "OK";
const char ATSerial::AT_ERROR[] PROGMEM = "ERROR";
const char ATSerial::AT_ERROR_CME[] PROGMEM = "+CME ERROR:";
const char ATSerial::AT_ERROR_CMS[] PROGMEM = "+CMS ERROR:";
const char ATSerial::AT_ALL_ERRORS[] PROGMEM ="X";



ATSerial::ATSerial(Stream *stream) {
	mSerial=stream;
	ATSerial::setTimeout(AT_DEFAULT_TIMEOUT);
}
 

//SimComm
char * ATSerial::GetToken(char *respuesta,uint8_t tokenNum,const char* splitchar)
{
	uint8_t contador=0;

	strtok(NULL,splitchar); //ASegura integridad de strtok
	char *token=strtok(respuesta,splitchar);
	LOG_DEBUG_ARGS("Token %i-> %s",contador,token);
	while (token !=NULL && contador<tokenNum)
	{
			contador++;
			token=strtok(NULL,splitchar);
			LOG_DEBUG_ARGS("Token %i-> %s",contador,token);

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



//Devuelve el numero de caracteres transmitidos en el puerto serie
uint8_t  ATSerial::SendCommandAsync(const __FlashStringHelper * comando,...)
{

	//Checks previos params
	if (strlen_P((char *)comando)>AT_BUFFER_SIZE_COMMAND)
	{
		//CRITICAL("ATS-3");
		return RX_ERROR_PARAMS;
	}

	memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);


	va_list ap;
	va_start(ap,comando);
	vsnprintf_P(buff_comando,AT_BUFFER_SIZE_COMMAND,(char *)comando, ap);

	va_end(ap);


	return ATSerial::_SendCommandCheck(buff_comando,NULL);
}


uint8_t ATSerial::SendCommandCheck(const __FlashStringHelper * __fmt,const __FlashStringHelper *response, ...)
{

		//Checks previos params
		if (strlen_P((char *)__fmt)>AT_BUFFER_SIZE_COMMAND)
		{
			//CRITICAL("ATS-1");
			return RX_ERROR_PARAMS;
		}

		if (strlen_P((char *)response)>AT_BUFFER_SIZE_EXPECTED)
		{
			//CRITICAL("ATS-2");
			return RX_ERROR_PARAMS;
		}




		memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);
		va_list ap;
		va_start(ap,__fmt);
		vsnprintf_P(buff_comando,AT_BUFFER_SIZE_COMMAND,(char *)__fmt, ap);

		va_end(ap);


		memset(buff_expected,0,AT_BUFFER_SIZE_EXPECTED);
		strcpy_P(buff_expected,(char *)response);

		return ATSerial::_SendCommandCheck(buff_comando,buff_expected);

}


//Devuelve el numero de caracteres transmitidos en el puerto serie
uint8_t  ATSerial::SendCommandAsync(const char * comando,...)
{

	if (strlen(comando)>AT_BUFFER_SIZE_COMMAND)
	{
		//CRITICAL("ATS-4");
		return RX_ERROR_PARAMS;
	}

	memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);
	va_list ap;
	va_start(ap,comando);
	vsnprintf(buff_comando,AT_BUFFER_SIZE_COMMAND,comando, ap);
	va_end(ap);

	return ATSerial::_SendCommandCheck(buff_comando,NULL);
}


uint8_t ATSerial::SendCommandCheck( const char *__fmt,const char *expectedResult, ...)
{
	if (strlen(__fmt)>AT_BUFFER_SIZE_COMMAND)
	{
		//CRITICAL("ATS-5");
		return RX_ERROR_PARAMS;
	}

	memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);
	va_list ap;
	va_start(ap,__fmt);
	vsnprintf(buff_comando,AT_BUFFER_SIZE_COMMAND,__fmt, ap);
	va_end(ap);

	return _SendCommandCheck(buff_comando,expectedResult);

}



uint8_t ATSerial::SendCommandCheckError( const __FlashStringHelper * __fmt,const __FlashStringHelper *response,const __FlashStringHelper *error,  ...)
{
	if (strlen_P((char *)__fmt)>AT_BUFFER_SIZE_COMMAND)
	{
		//CRITICAL("ATS-5");
		return RX_ERROR_PARAMS;
	}

	memset(buff_comando,0,AT_BUFFER_SIZE_COMMAND);
	va_list ap;
	va_start(ap,__fmt);
	vsnprintf_P(buff_comando,AT_BUFFER_SIZE_COMMAND,(char*)__fmt, ap);
	va_end(ap);


	memset(buff_expected,0,AT_BUFFER_SIZE_EXPECTED);
	strcpy_P(buff_expected,(char *)response);


	//Ponemos apuntando al final del buffer -len del error
	char *buff_error=buff_comando+( AT_BUFFER_SIZE_COMMAND -(strlen_P((char *) error)+1));
	strcpy_P(buff_error,(char *)error);


	return ATSerial::_SendCommandCheck(buff_comando,buff_expected,buff_error);


}

size_t ATSerial::SendRawData(char  data)
{
	return mSerial->print(data);
}

size_t ATSerial::SendRawData(const char * data)
{
	return mSerial->print(data);
}

size_t ATSerial::SendRawData_P(PGM_P data)
{
	return mSerial->print((__FlashStringHelper *)data);
}

uint8_t ATSerial::_SendCommandCheck(const char * command,const char *expectedResult)
{
	return _SendCommandCheck(command,expectedResult,NULL);
}

uint8_t ATSerial::_SendCommandCheck(const char * command,const char *expectedResult,const char *errorResult)
{

	uint8_t result=RX_NO_DATA;
 	ProcessResults();



 	LOG_DEBUG_ARGS("TX->%s",command);

	//Ejecuta el comando
	result=mSerial->println(command);
	/*
	result=mSerial->write('\r');
	result=mSerial->write('\n');*/

	//result=SendCommand(buff_comando,buff_response);


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

	if (ProcessResultPtr!=NULL)
		return ProcessResultPtr(respuesta);
	else
		return true;

}

uint8_t ATSerial::ExpectedResult (const char *resultado,const char * expectedResult)
{

	 //Leemos linea
	if (resultado!=NULL && expectedResult!=NULL )
	{
		if (strncmp(resultado,expectedResult,strlen(expectedResult))==0)
			return RX_CHECK_OK;
		else
			return RX_CHECK_KO;
	}
	else
		return RX_CHECK_KO;
}

uint8_t ATSerial::WaitResponseResult(char * resultado,const char *expectedResult, const char *errorResult,uint16_t wait)
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
				 if (strcmp_P(errorResult,ATSerial::AT_ALL_ERRORS)==0)
				 {
					 //Chequeamos todos los errores posibles
					 if
					 (
					 strncmp_P(resultado, ATSerial::AT_ERROR,strlen_P(ATSerial::AT_ERROR))==0 ||
					 strncmp_P(resultado, ATSerial::AT_ERROR_CME,strlen_P(ATSerial::AT_ERROR_CME))==0 ||
					 strncmp_P(resultado, ATSerial::AT_ERROR_CMS,strlen_P(ATSerial::AT_ERROR_CMS))==0
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

   	 //INFO4("Expected result %s -> %s = %i",resultado,expectedResult,result);

	 return result;


}

uint8_t ATSerial::WaitResponseResult(char * resultado,const char *expectedResult, uint16_t wait)
{
		return WaitResponseResult(resultado,expectedResult,NULL,wait);
}

void ATSerial::WaitResponse( uint16_t wait)
{
	unsigned long t_inicial = millis();

	while ((millis() - t_inicial) < wait && mSerial->available() == 0) {	delay(1);	}
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

	mSerial->flush();

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
			LOG_INFO_ARGS("RX %i->%s ",result,respuesta);

	}


	return result;
}

