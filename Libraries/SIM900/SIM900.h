
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

#include <ATSerial.h>
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


#define NUM_SMS_LENGTH 15 //Numero de sms Ej.. 0034653316799


/*
typedef enum
{
	SIM,
	Phone
} callType_t;
*/


#define SMS_NO_OPENED -1
#define SMS_OPEN_ALREADY -1

typedef enum
{
	SleepDisable=0, //Nunca se pone en modo ahorro
	SleepDTRControl=1, //Cuando DTR es alto(1) el dispositivo puede entrar en Sleep Mode, cuando DTR es bajo(0)(-) el modulo puede quitar el sleep mode
	SleepEnableAuto=2,//Si pasan 5s el solo se pone en modo sleep mode

} Sim900SleepMode;



typedef enum {
	Sim900Unknown =0,//
	Sim900Ready = 1,//Listo
	Sim900SMS= 2,//Enviando SMS
	Sim900URL=3,//Peticion URL en curso
} Sim900Status;


class SIM900:public ATSerial {
public:
	//Constructor de logger


	SIM900(Stream *serial);


	virtual ~SIM900();

	Sim900SleepMode CurrentSleepMode ;


bool URLRequest(char *url,bool isGet,bool (*HttpParametersCallback)(),void (*HttpResultCallback)(const char*,int));

time_t GetTime(TimeElements &);

time_t ParseTime(TimeElements &elements,char *charptr);

bool ConfigAPN(const __FlashStringHelper * variable,const char* valor);
bool getIMEI(char * imei);


bool EstaArrancado();
bool ActivaModulo();
bool EstaRegistrado();
Sim900Status GetStatus();
//SMS
uint8_t Sms(const char* numero,const char* message);
uint8_t Sms_P(const char* numero,PGM_P message);
uint8_t SmsOpen(const char* numero);
uint8_t SmsMessage(const char* msg);
uint8_t SmsMessage_P(PGM_P msg);
uint8_t SmsSend();
uint8_t SmsDelete();

void WakeUp();
bool SetMode(Sim900SleepMode mode);

//GPIO
bool GPIOWrite(uint8_t pin,uint8_t mode);


//Sim
bool BorrarContactoSIM(uint8_t index);
bool GuardarEnSIM(uint8_t index,char *phone_number,char *contact_name);
bool SIMEstaLista();
bool GetSIMContact(uint8_t position, char *phone_number,char *contact_name);
bool ExisteContactoSIM(uint8_t entrySim);

//uint8_t Llamar(callType_t tipo,const char *telefono);
/*
bool LlamarTelefono(const char *telefono);
bool LlamarDesdeSim(uint8_t entrySim);
bool Llamar(callType_t tipo,const char *telefono);
bool TieneLlamadas();
*/


/*
 -1 Error...
 0...1   0-Nada
 1...7:  1-Mala
 7...13: 2-Medio Bajo
 14..21: 3-Medio
 22..25: 4-Medio Alto
 26..31: 5-Buena
 99  99-Desconocido
 */
uint8_t GetCobertura() ;

//Punteros a funcion
void (*Sim900Message)(char*);
void (*Sim900Calling)();


void SetDefaultSMSNum (char* numero);
void SwitchModule();


	//void (*Sim900HTTPGet)(const char*,int);//puntero a funcion para pasar url y longitud del puerto serie

private:
	void init();
	uint16_t _smslength;
	char _numeroSMS[NUM_SMS_LENGTH];
	Sim900Status _status;//Estado... enviando sms, peticion gueb etc, etc...


};


extern SIM900 Sim900;

#endif /* HILO_H_ */
