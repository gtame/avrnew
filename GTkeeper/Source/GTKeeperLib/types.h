/*
 * types.h
 *
 * Created: 12/03/2018 21:24:41
 *  Author: Gabi
 */ 


#ifndef TYPES_H_
#define TYPES_H_




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


#endif /* TYPES_H_ */