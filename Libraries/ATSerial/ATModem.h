/* 
* ATModem.h
*
* Created: 21/03/2018 9:01:26
* Author: gtame
*/

#include "ATSerial.h"
#include <Time.h>

#ifndef __ATMODEM_H__
#define __ATMODEM_H__

#define SMS_NO_OPENED -1
#define SMS_OPEN_ALREADY -1
#define NUM_SMS_LENGTH 15 //Numero de sms Ej.. 0034653316799


typedef enum
{
	ATMSUnknown =0,//
	ATMSReady = 1,//Listo
	ATMSSMS= 2,//Enviando SMS
	ATMSURL=3,//Peticion URL en curso
	ATMSSleep=4,
	ATMSPowerOff=5,
} ATModemStatus; //AT Modem Status=ATMS

typedef enum 
{
	 CSQError=-1,//-1 Error...
	 CSQNada=0,//0...1   0-Nada
	 CSQMala=1,//1...7:  1-Mala
	 CSQMedioBajo=2,//7...13: 2-Medio Bajo
	 CSQMedio,//14..21: 3-Medio
	 CSQMedioAlto,//22..25: 4-Medio Alto
	 CSQBuena,//26..31: 5-Buena
	 CSQDesconocido,//99  99-Desconocido
} GSMCobertura;

typedef enum
{
	SIM,
	Phone
}GSMCallType;

class ATModem: public ATSerial
{
//variables
public:
protected:
private:

//functions
public:
	ATModem(Stream *stream);
	
	//Metodos de estado ..
	bool EstaArrancado();
	bool EstaRegistrado();
	GSMCobertura GetCobertura();
	bool GetIMEI(char * imei);
	
	//Hora GSM
	time_t GetTime();
	
	//Metodos para acceder a la Sim
	bool SIMEstaLista();
	bool BorrarContactoSIM(uint8_t index);
	bool GuardarEnSIM(uint8_t index,char *phone_number,char *contact_name);
	bool GetSIMContact(uint8_t position, char *phone_number,char *contact_name);
	bool ExisteContactoSIM(uint8_t entrySim);



	//SMS
	uint8_t Sms(const char* numero,const char* message);
	uint8_t Sms_P(const char* numero,PGM_P message);
	uint8_t SmsOpen(const char* numero);
	uint8_t SmsMessage(const char* msg);
	uint8_t SmsMessage_P(PGM_P msg);
	uint8_t SmsSend();
	uint8_t SmsDelete();
	void SetDefaultSMSNum (char* numero);
	
	inline ATModemStatus GetStatus() { return _status;}
	
	///Metodos de llamada
	bool TieneLlamadas();
	bool DescolgarLlamada();
	bool RechazarLlamada();
	bool LlamarDesdeSim(uint8_t entrySim);
	bool LlamarTelefono(const char *telefono);

	//Gestion del telefono
	virtual void Sleep();//Metodo utilizado para entrar en modo sleep
	virtual void WakeUp();//Metodo utilizado para salir del modo sleep
	virtual void PowerOff();//Metodo para Apagar
	virtual void PowerOn();//Encendido del telefono
	
protected:
	time_t ParseTime(TimeElements *elements,char *charptr);
	bool Llamar(GSMCallType tipo,const char *telefono);
	
private:

	ATModemStatus _status;//Estado... enviando sms, peticion gueb etc, etc...
	uint16_t _smslength; 
	char _numeroSMS[NUM_SMS_LENGTH];
	
}; //ATModem

#endif //__ATMODEM_H__
