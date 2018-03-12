/*
 * ATSerial.h
 *
 *  Created on: 15 de ene. de 2016
 *      Author: gtame
 *
 *  Defines utilizados
 *  SIM900_SOFTSERIAL -> Para indicar que queremos utilizar un SoftwareSerial ó HardwareSerial (Ver ATSerial.cpp es dnd esta implementado)
 *  MAX_CHARACTERS_SMS -> Num. caracteres max x sms
 *  AT_BUFFER_SIZE_RX
 *  AT_BUFFER_SIZE_COMMAND
  */


#ifndef ATSERIAL_H_
#define ATSERIAL_H_


#include <string.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

#include <Logger.h>



//Max characters for each SMS
#ifndef MAX_CHARACTERS_SMS
	#define MAX_CHARACTERS_SMS 140
#endif

//Default Timeout para DEBUG
#ifndef AT_DEFAULT_TIMEOUT
	#define AT_DEFAULT_TIMEOUT 500
#endif




//Rx STATEs
#define RX_OK_READ 0
#define RX_NO_DATA 1
#define RX_OVERFLOW_BUFFER 2
#define RX_TIMEOUT 3
#define RX_CHECK_OK 4
#define RX_CHECK_KO 5

#define RX_ERROR_CMS 97
#define RX_ERROR_CME 98
#define RX_ERROR 99
#define RX_ERROR_PARAMS 100

#define AT_BUFFER_SIZE_RX 50//Default  internal buffer size
#define AT_BUFFER_SIZE_COMMAND 50//Default  internal buffer size
#define AT_BUFFER_SIZE_EXPECTED 25//Resultado esperado para un comando


typedef   int	(*vsnprintffunc)(char *, size_t , const char *, va_list ) ;
typedef size_t (*strlenfunc) (const char *);


class ATSerial {
public:

	ATSerial(Stream *stream);
	virtual ~ATSerial();


	//Ultimo comando AT lanzado
	char * GetLastCommand();
	//Ultima recepcion
	char * GetLastResponse();
	bool  CheckInputMode();


	uint8_t SendCommandCheckOK(const __FlashStringHelper * __fmt, ...);
	uint8_t SendCommandCheckOK(const char * __fmt, ...);


	uint8_t SendCommandCheck( const char * __fmt,const __FlashStringHelper * expectedResult, ...);
	uint8_t SendCommandCheck(const __FlashStringHelper * __fmt,const __FlashStringHelper *response, ...);

	uint8_t SendCommandCheckError( const __FlashStringHelper * __fmt,const __FlashStringHelper *response,const __FlashStringHelper *error,  ...);

	//Devuelve el numero de caracteres transmitidos en el puerto serie
	uint8_t  SendCommandAsync(const __FlashStringHelper * comando,...);
	uint8_t  SendCommandAsync(const char * comando,...);

	size_t SendRawData(const char * data, ...);
	size_t SendRawData(char  data);
	size_t SendRawData( const __FlashStringHelper * __fmt, ...);
	size_t SendEndLine();

	uint8_t ReadSerialLine(char *respuesta, uint8_t size=AT_BUFFER_SIZE_RX);

	uint8_t ReadSerialLine();


	uint8_t WaitResponseResult(const __FlashStringHelper * expectedResult,const __FlashStringHelper *error, uint16_t wait);
	int WaitResponse( uint16_t wait=0);
	bool CheckChar(const char chr,uint16_t wait=0);


	//Procesamos resultados que halla en Buffer de serial
	void ProcessResults(uint16_t wait=250);

	



	char * GetNextToken(uint8_t tokenNum, char* splitchar);
	char * GetToken(char *respuesta,uint8_t tokenNum,char* splitchar);



	bool (*ProcessResultPtr)(char *);


	//Pone el nivel
    inline int  getTimeout(void)      { return _timeout;  }
    inline void setTimeout(int  timeout) {  _timeout = timeout; }




	 static const  char AT_OK[] PROGMEM;
	 static const  char AT_ERROR_RESPONSE[] PROGMEM;
	 static const  char AT_ERROR_CME_RESPONSE[] PROGMEM;
	 static const  char AT_ERROR_CMS_RESPONSE[] PROGMEM;
	 static const  char AT_ALL_ERRORS_RESPONSE[] PROGMEM;
	 static const  char AT_ENDLINE[] PROGMEM;

	 

protected:
	uint8_t ExpectedResult (const char *resultado,const __FlashStringHelper * expectedResult);

	const char EXIT_INPUTMODE=0x0D;
	virtual bool OnProcessResult( char * );
	uint8_t _SendCommandCheck(const char *__fmt,const __FlashStringHelper * expectedResult);
	uint8_t _SendCommandCheck(const char *__fmt,const __FlashStringHelper * expectedResult,const __FlashStringHelper * expectedError);

	uint8_t WaitResponseResult(char  * resultado,const __FlashStringHelper  *expectedResult, uint16_t wait );
	uint8_t WaitResponseResult(char * resultado,const __FlashStringHelper *expectedResult, const __FlashStringHelper *errorResult,uint16_t wait);



	
	
	
	char buff_expected[AT_BUFFER_SIZE_EXPECTED];
	char buff_comando[AT_BUFFER_SIZE_COMMAND]; //Comando enviado
	char buff_response[AT_BUFFER_SIZE_RX]; //Respuesta


	Stream *mSerial;//ACtual puerto de comunicacion bien sea HArdwareSerial o SoftwareSerial



	int8_t ParseCommand_P(const __FlashStringHelper * comando,va_list ap);
	int8_t ParseCommand(const char* comando,va_list ap);
	int8_t _ParseCommand(const char* ,strlenfunc ,vsnprintffunc ,va_list );

private:

	int _timeout;
		
};



#endif /* ATSERIAL_H_ */
