/*
 * gtkeeper.h
 *
 *  Created on: 12 de ene. de 2016
 *      Author: gtame
 */

#ifndef GTKEEPER_H_
#define GTKEEPER_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <WString.h>
#include <Arduino.h>
#include <Time.h>
#include "LogSD.h"

#include "types.h"
#include "pinout.h"
#include "settings.h"
#include "textos.h"

#include "Utils/util.h"



//Referencia al core de arduino


#include <SIM900.h>
#include <ScreenManager.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <StateMachineLib.h>
#include <SD.h>
#include <DS1307RTC.h>
#include <LowPower.h>
#include  "Configuracion.h"
#include  "Programa.h"
#include  "Salida.h"

//Screens
#include "Screens\Screens.h"

#define SCREEN_ACTIVE() screenManager.IsActive()
//Defines para operar con puertos
#define APAGA_RELE(x)  LOG_INFO_ARGS_B("RELE OFF %i",x); digitalWrite(x,LOW);
#define ENCIENDE_RELE(x) LOG_INFO_ARGS_B("RELE ON %i",x);  digitalWrite(x,HIGH);


//Estados de la maquina , gtKeeper
enum MachineStates
{
	ON=0,
	Init=1,
	Error=2,
	Reset=3,
	Run=4,
	User=5,
	SMS=6,
	Call=7,
	Web=8,
	Off=99
};


class GTKeeper : public SIM900, public StateMachine, public Configuracion,public Salida,public Programa,public LogSD {

//variables
public:
     //struct para almacenar estadisticas
     Estadistica estadistica;
protected:


private:
	TimeElements timeEl;
	time_t t_last_web; //Tiempo para controlar los tiempos de la web.
	time_t t_last_error_web; //Tiempo para controlar los tiempos de la web.
	time_t last_RiegosCheck;
	time_t stop_abono;
	uint8_t error_web;//Numero de errores que se producen al intentar acceder al dispositivo
	uint8_t error_web_salidas;//Numero de intentos erroneos de subida web de salidas
	bool bSetupCompleted; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
	//bool bWebInProcess;//Flag para indicar que estamos actualmente refrescando desde la web
	bool bpendingWeb;//Flag para indicar que es necesario el update de web
	bool isHourSet;//Flag para saber si se ha fijado la hora
	bool isGSMOn;//Flag para saber si el modem esta presente... (Responde)
	bool bRebootSIM;//Flag para indicar que hemos reiniciado el modulo GSM, y necesitamos reconfigurarlo
	uint16_t error_code;
	
	char  * buffer;	  //buffer
	uint8_t buffersize; //buffer size
	
	char buff_parse[MAIN_BUFFER_PARSE]; //Parser para Comandos desde MemoryPrograms
	static const uint8_t ports[PORTS_NUM] ;
	static const uint8_t ports_abono[PORTS_ABONO] ;


//functions
public:

//CONSTRUCTOR & DESTRUCTOR
	GTKeeper(char *buff,uint8_t sizebuff);
//END CONSTRUCTOR & DESTRUCTOR

///ESTADOS
	 bool CheckON();
	 void OnON();
	 void OnLeaveON();
	 
	 bool CheckOff();
	 void OnOff();
	 void OnLeaveOff();

	 bool CheckReset();
	 void OnReset();
	 void OnLeaveReset();

	 bool CheckInit();
	 void OnInit();
	 void OnLeaveInit();

	 bool CheckError();
	 void OnError();
	 void OnLeaveError();
	 
	 bool CheckRun();
	 void OnRun();
	 void OnLeaveRun();
	 
	 bool CheckUser();
	 void OnUser();
	 void OnLeaveUser();
	 
	 bool CheckSMS();
	 void OnSMS();
	 void OnLeaveSMS();
	 
	bool CheckCall();
	void OnCall();
	void OnLeaveCall();
	
	
	bool CheckWeb();
	void OnWeb();
	void OnLeaveWeb();
///END ESTADOS


	//Funciones principales ó de proceso
	void Setup();
	bool LoopGSM();
	void Sleep();
	virtual bool OnProcessResult( char * ) override;//overrides
	void setLed(uint8_t led );
	bool ExecuteCommand(char* commandstr);//Ejecuta un comando desde string , bien sea desde SMS o desde CALL
	void setupStateMachine();

	//Funciones de clase WEB
	void UpdateWebSuccess(); // Se llama cuando se actualiza la web correctamente
	inline void ClearErrorWebs() { error_web=0;} //Limpia Contador de errores en sincronizacion web
	inline uint8_t	 GetErroresWeb() { return error_web;} //Nº de errores web
	inline time_t GetLastTimeWebError() {return t_last_error_web; } //Utlima vez que la sincronizacion dio error
	inline void AddErrorWeb() { error_web++;}  //Contador de errores en sincronizacion web
	inline time_t GetLastWebSync() { return t_last_web;} //Retorna ultima vez que la web fue actualiza satisfactoriamente
	inline void SetLastWebSync() { t_last_web=now();} //Actualiza la fecha de actualizacion de la web

	void CargaConfigWeb();//Carga los parametros de configuración web para que el modulo pueda conectar a internet
    void GetURL(const char * url,int length);
	bool GetURL(char *url);
	void GetHttpResultCallback(const char* url,int len);
	void CheckWebConfig();	//Metodo para comprobar si necesita refrescar la config desde la web. Si es asi CargaConfigWeb
    bool GetConfig4Web(char *url);
    static void ConfigHttpResultCallbackStatic(const char* url,int len);
    static void ConfigHttpResultCallback(const char* url,int len);
    static void DefaultHttpResultCallbackStatic(const char* url,int len);
	static bool  PostHttpParametersCallback();
	static void PostHttpResultCallback(const char* url,int len);
	
	//Riegos Arranca-para...
    void CheckRiegos(bool sendWeb);
    void AbrirValvulaLatch(uint8_t sector);
    void CerrarValvulaLatch(uint8_t sector);
    void LanzaRiego(uint8_t contador,bool sendsms) ;
    void ApagarRiegos();
    void EnciendeSectorSMS(uint8_t sector);
	bool EnciendePrograma(uint8_t program);
	bool ApagaPrograma (uint8_t program);
	bool EnciendeSector(uint8_t sector);
	bool ApagaSector (uint8_t sector);
	bool EnciendeAbono(uint8_t unidAbono);
	bool ApagaAbono (uint8_t unidAbono);
	bool ApagaMotor();
	bool EnciendeMotor ();
	void PararRiego(uint8_t contador);
	void ChequearRiegos(time_t tiempo);


	//Hora
    void SetHora(time_t time);
    bool EstaEnHora();
    bool FijarHoraGSM();
	bool FijarHoraRTC();

     //SMS
    void SendSmsFinReinicio();
    void SendSmsIniReinicio();
	void SendSmsProgramacion();
	void SendSmsSectoresEjecucion();
	void SendSmsHora();


	 //Estadisticas
     //Suma 1 min a los sectores que estan en ejecucion
     void RegistrarEstadisticas();
     void EEPROMGuardarEstadistica(Estadistica * stat);
     void EEPROMLeerEstadistica(uint8_t sector,Estadistica * stat);
     void ResetearEstadisticas();


protected:
private:
 	//Utils ..   
    char * PBB (const __FlashStringHelper * p1,...);
    time_t GetTimeWithoutSeconds(time_t tiempo);

};


//Variables
extern char bufferapp[MAIN_BUFFER_SIZE];

//extern variables
extern	Keypad keypad;
extern LiquidCrystal_I2C lcd;
extern GTKeeper gtKeeper;

//Interrupciones variables
extern volatile bool int_input_user;//El user pulso una key
extern volatile bool int_input_gsm;//El user pulso una key



#endif /* GTKEEPER_H_ */
