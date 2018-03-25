/*
 * gtkeeper.h
 *
 *  Created on: 12 de ene. de 2016
 *      Author: gtame
 */


 
 //Thirdparty
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
 #include <LowPower.h>
 #include <Time.h>

 //Libs Propias
 #include <ScreenManager.h>
 #include <LiquidCrystal_I2C.h>
 #include <Keypad.h>
 #include <StateMachineLib.h>
 #include <LogSD.h>
 #include <Hora.h>


 //Locales H
 #include "types.h"
 #include "pinout.h"
 #include "variables.h"
 #include "settings.h"
 #include "textos.h"
 #include "Utils/util.h"

 //Clases proyecto
 #include  "Clases/Configuracion.h"
 #include  "Clases/Programa.h"
 #include  "Clases/Salida.h"
 #include  "Clases/Estadistica.h"
 #include  "clases/Riegos.h"
 #include  "Clases/GSM.h"


 //Screens
 #ifdef PANTALLA_TECLADO
 #include <GTKeeperScreens.h>
 #endif




#ifndef GTKEEPER_H_
#define GTKEEPER_H_


#define SCREEN_ACTIVE() screenManager.IsActive()


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


uint8_t PostHttpResultCallback(const char* url,uint16_t length);
bool PostHttpParametersCallback();

class GTKeeper : public StateMachine,public Hora {
//variables
public:

protected:
	tConfiguracion *config;
	Configuracion *configuracion;
	GSM* gsm;
	Riegos *riegos;
	LogSD* logsd;

private:
	//TimeElements timeEl;
	time_t t_last_web;			//Tiempo para controlar la ultima actualizacion correcta web.
	time_t t_last_error_web;	//Tiempo para controlar la ultima actualizacion erronea web.
	uint8_t error_web;//Numero de errores acumulados al intentar sincronizar via web
	uint8_t error_web_salidas;//Numero de intentos erroneos de subida web de salidas
	bool bSetupCompleted; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
	bool bpendingWeb;//Flag para indicar que se recibio el comando para la actualizacion via web
	uint16_t error_code;//Codigo de error del dispositivo. ¿?
	
	char  * buffer;	  //buffer
	uint8_t buffersize; //buffer size
	char buff_parse[MAIN_BUFFER_PARSE]; //Parser para Comandos desde MemoryPrograms
	
//functions
public:

//CONSTRUCTOR & DESTRUCTOR
	GTKeeper(Configuracion *configuracion,	GSM* gsmmodem,Riegos * riegos,LogSD* logsd,char *buff,uint8_t sizebuff);
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
	//bool LoopGSM();
	void Sleep();

	void setLed(uint8_t led );
	bool ExecuteCommand(char* commandstr);//Ejecuta un comando desde string , bien sea desde SMS o desde CALL
	

	//Funciones de clase WEB
	void UpdateWebSuccess(); // Se llama cuando se actualiza la web correctamente
	inline void AddErrorWeb() { error_web++; t_last_error_web=now();}  //Contador de errores en sincronizacion web



protected:
	void setupStateMachine();
	
	inline void ClearErrorWebs() { error_web=0;} //Limpia Contador de errores en sincronizacion web
	inline uint8_t	 GetErroresWeb() { return error_web;} //Nº de errores web
	inline time_t GetLastTimeWebError() {return t_last_error_web; } //Utlima vez que la sincronizacion dio error
	
	inline time_t GetLastWebSync() { return t_last_web;} //Retorna ultima vez que la web fue actualiza satisfactoriamente
	inline void SetLastWebSync() { t_last_web=now();} //Actualiza la fecha de actualizacion de la web

private:
 	//Utils ..   
    //char * PBB (const __FlashStringHelper * p1,...);

};


extern GTKeeper gtKeeper; //Machine
#endif /* GTKEEPER_H_ */
