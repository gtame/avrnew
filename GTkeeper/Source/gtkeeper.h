/*
 * gtkeeper.h
 *
 *  Created on: 12 de ene. de 2016
 *      Author: gtame
 */

#ifndef GTKEEPER_H_
#define GTKEEPER_H_

#include "SIM900.h"
#include "settings.h"
#include <string.h>
#include <inttypes.h>
#include <Time.h>
#include <stdlib.h>
#include <WString.h>
#include <stdio.h>
//#include "EEPROMAnything.h"
#include <avr/eeprom.h>
#include "EEPROM.h"
#include "pinout.h"

#define LEN_CONFIG_STRING 20
//#define LEN_CONFIG_STRING_CR_LF LEN_CONFIG_STRING+2

#define LEN_PROGRAMA_STRING 17
#define LEN_PROGRAMA_STRING_CR_LF LEN_PROGRAMA_STRING+2
#define EEPROM_LENGTH E2END + 1


//Sumatorio total 126 para todos los dias
typedef enum {
	SMSNone =0,//
	SMSInicioSector = (1 << 0),//1
	SMSFinSector= (1 << 1),//2
	SMSProgramacionWeb= (1 << 2),//4
	SMSReset= (1 << 3),//8
	SMSReservado1= (1 << 4),//16
	SMSReservado2= (1 << 5),//32
	SMSReservado3= (1 << 6)//64
} SmsAviso;


//--Numero para enviar avisos
//--Configuracion APN
//--UserAPN
//--PwdAPN
//--Password para sms
//--Nivel de envios de sms
typedef struct  {
	char MovilAviso[15];
	char PasswordSMS[5];
	uint8_t AvisosSMS;
	char Imei[16]; //Imei del modulo GSM - tiene 14 o 15 caracters mas el NULL =16
	char APN[25]; //APN utilizado para conexion internet x modulo GSM
	char userAPN[25]; // Usuario APN (en el caso que lo tenga)
	char pwdAPN[25];// Pwd APN (en el caso que lo tenga)
	bool GSMAvailable; //Modulo GSM disponible
	uint8_t numpuertos; //Numero de puertos (sectores)
	uint8_t numabono;//Numero de bombas  de abono.
	bool motor_diesel;//indicador si hay motor para riego


	time_t lastupdateconfig;//Cuando se cargo por ultima vez la configuracion desde web
	time_t lastupdateprog;//Cuando se cargaron por ultima vez los programas
	time_t lasthora;//Ultima hora fijada

	char flag_check; //Este flag es para comprobar que la configuracion se lee correctamnete, siempre que se lee debe ir a 'X';

} 	tConfiguracion_t , Configuracion, *tmConfiguracionPtr_t;






//Sumatorio total 126 para todos los dias
typedef enum {
	 NONE =0,
    L = (1 << 0),//1
    M= (1 << 1),//2
    X= (1 << 2),//4
    J= (1 << 3),//8
    V= (1 << 4),//16
    S= (1 << 5),//32
    D= (1 << 6)//64
} DiasSemana;

//SSDDDHHMMRRRRAAAA
//SS   -> Sector
//DDD  -> Dias que se ejecutara (enum)
//HHMM -> Hora inicio ó Ejecución en el otro bloque
//RRRR -> Tiempo de riego
//AAAA -> Tiempo de abono

//Ej, 03101010120001200000
//03->Sector
//0101010-> Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
typedef struct  {

  uint8_t Sector; //Sector
  uint8_t Dias; //Dias en los que se debe ejecutar
  uint8_t HoraInicio;//Cuando debe ejecutarse , tiene dos variantes
  uint8_t MinutoInicio;//99:XX --> Indicara que se lanze cuando termine el programa XX
  	  	  	  	  	   //HH:MM --> Indicara la hora a la que se lanzara si el dia esta marcado
  uint32_t TiempoRiego; //Tiempo de riego , en segundos
  uint32_t TiempoAbono; //Tiempo de abono,  en segundos


} 	tPrograma_t, Programa, *tmProgramaPtr_t;


///Definimos el peso para que la ordenacion tenga efecto
typedef enum {
	actNone=10,//No hay salida activa
	actPrograma=1,
	actSector=2,
	actAbono=3,
	actMotor=4,
	actLimpieza=5
} TipoSalidaActiva;


typedef struct {

  uint8_t Ident; //Sector(salida) ó Programa que lanzo
  TipoSalidaActiva Tipo; //Tipo, si es salida o
  uint8_t Sector; //Este campo es para asegurar cual era el sector cuando el programa se lanzo para evitar modificaciones entre que salta y se para
  time_t Desde; //Indica cuando se activo la salida ;)
  time_t Hasta;//Indica hasta cuando va a estar 'encendido el programa', si esta parado valor 0;

} SalidasActivas;

/*
typedef struct {

	bool pendingWeb; //Pendiente de envio web
	struct SalidasActivas salida; //Datos a enviar

} EnvioWeb;
*/

//Struct pra almacenar las estadisticas de riego
typedef struct
{
	uint8_t sector;
	time_t tiempo_riego;
	time_t tiempo_abono;

} Estadistica;

//Defines para operar con puertos
#define APAGA_RELE(x)  LOG_INFO_ARGS_B("RELE OFF %i",x); digitalWrite(x,LOW);
#define ENCIENDE_RELE(x) LOG_INFO_ARGS_B("RELE ON %i",x);  digitalWrite(x,HIGH);


/*

//Esto abrira la valvula ;)
#define ABRE_VALVULA(x) ENCIENDE_RELE(X);\
						delay(500);\
						APAGA_RELE(X);

#define CIERRA_VALVULA(x) ENCIENDE_RELE(X);\
						delay(500);\
						APAGA_RELE(X);*/


#ifdef DEBUG
	#define MAX_PROGRAMAS 5//Numero máximo de programas
#else
	#define MAX_PROGRAMAS 15//Numero max de programas
#endif




#define GET_ADDRES_PROGRAM(X) (LEN_PROGRAMA_STRING*X)+sizeof(Configuracion)
#define GET_ADDRES_ESTADISTICA(X) ((LEN_PROGRAMA_STRING*MAX_PROGRAMAS)+sizeof(Configuracion))+ (sizeof(Estadistica)*(X-1))

//EMPIEZAN LAS SALIDAS WEB EN -> PROGRAMAS + CONFIGURACION + ESTADISTICA
#define GET_ADDRES_SALIDAS_WEB ((LEN_PROGRAMA_STRING*MAX_PROGRAMAS)+sizeof(Configuracion))+ (sizeof(Estadistica)*PORTS_NUM)


#define PARSE_TO_SECONDS(a) ((a)/1000)
#define MAX_SECONDS_ELAPSED 120


#define NUMERO_HORAS(SEG) (SEG/SECS_PER_HOUR)
#define NUMERO_MINUTOS(SEG) (SEG % SECS_PER_HOUR) / SECS_PER_MIN


class GTKeeper {

public:

	GTKeeper();
	virtual ~GTKeeper();

    inline bool IsSetupCompleted() { return bSetupCompleted; }
    inline bool IsPendingWeb() { return bpendingWeb; }

    //Funciones de clase
    void GetURL(const char * url,int length);
    bool ProcessATMensajes(char * msg);
    void Initializate();
    void EndInitializate();
    bool CheckSIM();
    void CheckRiegos(bool sendWeb);
    void CheckWeb();


    bool EEPROMCargaConfig();
    void CargaConfigDesdeString(char *config );
    void EEPROMGuardaConfig();





    bool EEPROMCargaProgramas();

    void CargaConfigWeb();


    //Punteros a funcion
    void (*ChangeStatus)(uint8_t status);//Cuando cambia el estado ..


    bool Loop();
    bool IsGSMEnable();


    Programa programas[MAX_PROGRAMAS];


    bool GrabarProgramaAEEPROM(uint8_t posicion,Programa* programa);
    bool CargarProgramaDesdeEEPROM(uint8_t posicion,Programa* programa);


    bool CargaProgramaDesdeString(char *programa,Programa * tprograma);

    void ProgramaToString(char *sms_text,Programa *programa);
    void ShowInfoProgramas();


    void AbrirValvulaLatch(uint8_t sector);
    void CerrarValvulaLatch(uint8_t sector);

    void LanzaRiego(uint8_t contador,bool sendsms) ;
    void ApagarRiegos();

    void ResetPrograma(Programa* programa);
    void ResetProgramas();

    void SetHora(time_t time);

    bool EstaEnHora();

    void EnciendeSectorSMS(uint8_t sector);

    bool FijarHoraGSM();
    bool EstaRegistradoGSM();


     void  ResetConfig();
     //Carga el programa desde la sim
     void SendSmsFinReinicio();
     void SendSmsIniReinicio();



     //Configuración
     Configuracion config;


     //struct para almacenar estadisticas
     Estadistica estadistica;



     //Arranca-para...
     bool EnciendePrograma(uint8_t program);
     bool ApagaPrograma (uint8_t program);
     bool EnciendeSector(uint8_t sector);
     bool ApagaSector (uint8_t sector);
     bool EnciendeAbono(uint8_t unidAbono);
     bool ApagaAbono (uint8_t unidAbono);
     bool ApagaMotor();
     bool EnciendeMotor ();



     //Estos metodos manejan las salidas activas ;)
     uint8_t salidas_activas;//Contador para almacenar las salidas activas
     SalidasActivas salidas [MAX_PROGRAMAS];//Array para guardar las salidas activas


     uint32_t salidas_web;//Almacen la posicion EEPROM donde esta actualmente el cursor de salidas.
     //SalidasActivas pendingWeb[MAX_PROGRAMAS*2];//Array para guardar las salidas


     void ShowInfoSalidas();
     uint8_t GetSalidasActivas() { return salidas_activas;}
     void RegistrarSalida(uint8_t salida , TipoSalidaActiva tipo);
     int8_t GetPosicion(uint8_t salida , TipoSalidaActiva tipo);
     bool SalidaRegistrada(uint8_t salida , TipoSalidaActiva tipo);
     void EliminarSalida(uint8_t salida , TipoSalidaActiva tipo);
     uint8_t RiegosActivosEnSector(uint8_t sector);

     //Suma 1 min a los sectores que estan en ejecucion
     void RegistrarEstadisticas();
     void EEPROMGuardarEstadistica(Estadistica * stat);
     void EEPROMLeerEstadistica(uint8_t sector,Estadistica * stat);
     void ResetearEstadisticas();
     void RegistrarSalidaEnEEPROM(SalidasActivas * stat,bool finaliza);
     bool RegistrarSalidaEnWeb();
     bool CargaConfigDesdeSim();

     void ShowConfigInfo();

     static bool  PostHttpParametersCallback();
     static void PostHttpResultCallback(const char* url,int len);
     char buff_parse[MAIN_BUFFER_PARSE]; //Parser para Comandos desde MemoryPrograms
     bool GetURL(char *url);
     void GetHttpResultCallback(const char* url,int len);


     //Metodo para comprobar si necesita refrescar la config desde la web.
     //Si la necesita llama al metodo
     void CheckWebConfig();


     static const uint8_t ports[PORTS_NUM] ;
     static const uint8_t ports_abono[PORTS_ABONO] ;

protected:

     static void DefaultHttpResultCallbackStatic(const char* url,int len);


     bool GetConfig4Web(char *url);
     static void ConfigHttpResultCallbackStatic(const char* url,int len);
     static void ConfigHttpResultCallback(const char* url,int len);



  private:


    void SendSmsProgramacion();
    void SendSmsSectoresEjecucion();
    void SendSmsHora();

    static int sortmethod(const void* a, const void* b);

    void PararRiego(uint8_t contador);
    void ChequearRiegos(time_t tiempo);


    inline void OnChangeStatus(uint8_t status);

    char * PBB (const __FlashStringHelper * p1,...);


    time_t GetTimeWithoutSeconds(time_t tiempo);
    TimeElements timeEl;



    time_t t_last_web; //Tiempo para controlar los tiempos de la web.
    time_t last_RiegosCheck;
    time_t stop_abono;
    uint8_t error_web;//Numero de errores que se producen al intentar acceder al dispositivo
    uint8_t error_web_salidas;//Numero de intentos erroneos de subida web de salidas
    bool bSetupCompleted; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
    bool bWebInProcess;//Flag para indicar que estamos actualmente refrescando desde la web
    bool bpendingWeb;//Flag para indicar que es necesario el update de web
    bool isHourSet;//Flag para saber si se ha fijado la hora
    bool isGSMOn;//Flag para saber si el modem esta presente... (Responde)

	char buffer[MAIN_BUFFER_SIZE];





    static const uint16_t EEADDR_SW =GET_ADDRES_SALIDAS_WEB;


    bool bRebootSIM;//Flag para indicar que hemos reiniciado el modulo GSM, y necesitamos reconfigurarlo

};



extern GTKeeper gtKeeper;



#endif /* GTKEEPER_H_ */
