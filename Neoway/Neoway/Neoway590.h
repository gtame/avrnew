
/*
 * Neoway590.h
 *
 *
 *  Defines utilizados
 *  Neoway590_SOFTSERIAL -> Para indicar que queremos utilizar un SoftwareSerial ó HardwareSerial (Ver ATSerial.cpp es dnd esta implementado)
 *  PIN_GSM_ON -> PIN Utilizado para encender por software el modulo
 */


#ifndef HILO_H_
#define HILO_H_


#include "WebSockets.h"
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
 


typedef enum {
	Neoway590Unknown=1,
	Neoway590Starting =0,//
	Neoway590Ready = 1,//Listo
	Neoway590Busy= 2,//Enviando SMS
	Neoway590Disconnected=3,//Peticion URL en curso
} Neoway590Status;


class Neoway590:public WebSockets {
//class Neoway590:public ATSerial{
public:
	//Constructor de logger
 

	Neoway590(Stream *serial,uint8_t bootPin);


time_t GetTime(TimeElements &);
time_t ParseTime(TimeElements &elements,char *charptr);
int getIMEI(char * imei);


bool EstaArrancado();
bool ActivaModulo();
int IsReady();

int EstaRegistrado();
Neoway590Status GetStatus();
int Apagar();
void WakeUp();
 

//Sim
int BorrarContactoSIM(uint8_t index);
int GuardarEnSIM(uint8_t index,char *phone_number,char *contact_name);
int SIMEstaLista();

int ExisteContactoSIM(uint8_t entrySim);

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
int GetCobertura() ;

//Punteros a funcion

void SwitchModule();
 

 void DetachRing();
 void AttachRing(void (*ring)() ,uint8_t pin);


	//void (*Neoway590HTTPGet)(const char*,int);//puntero a funcion para pasar url y longitud del puerto serie
protected:
	virtual  bool OnProcessResult( char * )  override;	
	void OnReady();
	void OnStart();

private:
	void init();
	uint16_t _smslength;
	Neoway590Status _status;//Estado... enviando sms, peticion gueb etc, etc...
	uint8_t bootPin;
	int8_t ringPing;

};


extern Neoway590 tNeoway590;

#endif /* HILO_H_ */
