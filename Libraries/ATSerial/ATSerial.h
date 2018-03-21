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
	#define AT_DEFAULT_TIMEOUT 2000
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

#define AT_BUFFER_SIZE_RX 150//Default  internal buffer size
#define AT_BUFFER_SIZE_COMMAND 150//Default  internal buffer size
#define AT_BUFFER_SIZE_EXPECTED 25//Resultado esperado para un comando




class ATSerial {
public:

	ATSerial(Stream *stream);
 

	//Ultimo comando AT lanzado
	char * GetLastCommand();
	//Ultima recepcion
	char * GetLastResponse();


	//Devuelve el numero de caracteres transmitidos en el puerto serie
	uint8_t  SendCommandAsync(const char * comando,...);

	uint8_t SendCommandCheck( const char * __fmt,const char *response, ...);
	uint8_t SendCommandCheckError( const __FlashStringHelper * __fmt,const __FlashStringHelper *response,const __FlashStringHelper *error,  ...);
	uint8_t SendCommandCheck(const __FlashStringHelper * __fmt,const __FlashStringHelper *response, ...);

	//Devuelve el numero de caracteres transmitidos en el puerto serie
	uint8_t  SendCommandAsync(const __FlashStringHelper * comando,...);
	size_t SendRawData(const char * data);
	size_t SendRawData(char  data);
	size_t SendRawData_P(PGM_P data);

	uint8_t ReadSerialLine(char *respuesta, uint8_t size=AT_BUFFER_SIZE_RX);

	uint8_t ReadSerialLine();

	void WaitResponse( uint16_t wait=0);
	uint8_t WaitResponseResult(char * resultado,const char *expectedResult, uint16_t wait );
	uint8_t WaitResponseResult(char * resultado,const char *expectedResult, const char *errorResult,uint16_t wait);

	//Procesamos resultados que halla en Buffer de serial
	void ProcessResults(uint16_t wait=250);

	virtual bool OnProcessResult( char * );




	char * GetToken(char *respuesta,uint8_t tokenNum,const char* splitchar);



	bool (*ProcessResultPtr)(char *);


	//Pone el nivel
    inline int  getTimeout(void)      { return _timeout;  }
    inline void setTimeout(int  timeout) {  _timeout = timeout; }



    const static char AT_OK[] PROGMEM;
    const static char AT_ERROR[] PROGMEM;
    const static char AT_ERROR_CME[] PROGMEM;
    const static char AT_ERROR_CMS[] PROGMEM;
    const static char AT_ALL_ERRORS[] PROGMEM;



protected:
	uint8_t ExpectedResult (const char *resultado,const char * expectedResult);


	uint8_t _SendCommandCheck(const char *__fmt,const char *expectedResult);
	uint8_t _SendCommandCheck(const char *__fmt,const char *expectedResult,const char *expectedError);

	char buff_expected[AT_BUFFER_SIZE_EXPECTED];
	char buff_comando[AT_BUFFER_SIZE_COMMAND]; //Comando enviado
	char buff_response[AT_BUFFER_SIZE_RX]; //Respuesta


	Stream *mSerial;//ACtual puerto de comunicacion bien sea HArdwareSerial o SoftwareSerial

private:

	int _timeout;


};




#endif /* ATSERIAL_H_ */
