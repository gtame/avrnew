
/*
 * SIM900.h
 *
 *
 *  Defines utilizados
 *  SIM900_SOFTSERIAL -> Para indicar que queremos utilizar un SoftwareSerial ó HardwareSerial (Ver ATSerial.cpp es dnd esta implementado)
 *  PIN_GSM_ON -> PIN Utilizado para encender por software el modulo
 */


#ifndef HILO_H_
#define HILO_H_

#include <ATModem.h>
#include <Time.h>
#include <string.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

//typedef unsigned int uint32;


// pins definition
#ifndef PIN_GSM_ON
	#define PIN_GSM_ON         9 // connect GSM Module turn ON to pin 77
#endif

#ifndef GSM_SLEEP_PIN
	#define GSM_SLEEP_PIN 13
#endif


 typedef enum
 {
	 SleepDisable=0, //Nunca se pone en modo ahorro
	 SleepDTRControl=1, //Cuando DTR es alto(1) el dispositivo puede entrar en Sleep Mode, cuando DTR es bajo(0)(-) el modulo puede quitar el sleep mode
	 SleepEnableAuto=2,//Si pasan 5s el solo se pone en modo sleep mode

 } Sim900SleepMode;

typedef uint8_t (*HttpResultCallback)(const char*,uint16_t) ;
typedef bool (*HttpParametersCallback)();

class SIM900:public ATModem {
public:
	//Constructor de logger
	SIM900(Stream *serial);

	bool SetSleepMode(Sim900SleepMode mode);
	bool URLRequest(char *url,bool isGet,HttpParametersCallback paramCallback,HttpResultCallback resultCallback);
	bool GetHttpBuffer(uint8_t desde,uint8_t length);
	bool ConfigAPN(const __FlashStringHelper * variable,const char* valor);
	bool GPIOWrite(uint8_t pin,uint8_t mode);//GPIO

	//DTMF
	bool GenerarPulsoDTMF(char caracter,uint16_t duration);//DTMF pulso
	bool PasarDTMFMode();
	

	//Punteros a funcion
	void (*Sim900Message)(char*);
	void (*Sim900Calling)();

 

	void WakeUp() override;
	void Sleep() override;
	void PowerOff() override;
	void PowerOn() override;
	
	
private:
 

	Sim900SleepMode _sleepmode ;

};

 
#endif /* HILO_H_ */
