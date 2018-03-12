
#ifndef Main_h
#define Main_h
#include <inttypes.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <Frame.h>


//#define SEND_SMS //Envia SMS de aviso para carga y inicio fin de riego
#define INICIO_RAPIDO

//Valores tomados en binario base 2
/*
enum {
  kCGDisplayBeginConfigurationFlag  = (1 << 0), // 0b0000000000000001
  kCGDisplayMovedFlag               = (1 << 1), // 0b0000000000000010
  kCGDisplaySetMainFlag             = (1 << 2), // 0b0000000000000100
  kCGDisplaySetModeFlag             = (1 << 3), // 0b0000000000001000
  kCGDisplayAddFlag                 = (1 << 4), // 0b0000000000010000
  kCGDisplayRemoveFlag              = (1 << 5), // 0b0000000000100000
  kCGDisplayEnabledFlag             = (1 << 8), // 0b0000000100000000
  kCGDisplayDisabledFlag            = (1 << 9), // 0b0000001000000000
  kCGDisplayMirrorFlag              = (1 << 10),// 0b0000010000000000
  kCGDisplayUnMirrorFlag            = (1 << 11),// 0b0000100000000000
  kCGDisplayDesktopShapeChangedFlag = (1 << 12) // 0b0001000000000000
};

Now you can add them (or "or" them) and get different values

kCGDisplayAddFlag | kCGDisplayDisabledFlag  // 0b0000001000010000

*/

//Callback function para el envio de los sms..
//typedef void (*OnSms)();  // alarm callback function typedef

typedef enum {
    L = (1 << 0),//0
    M= (1 << 1),//2
    X= (1 << 2),//4
    J= (1 << 3),//8
    V= (1 << 4),//16
    S= (1 << 5),//32
    D= (1 << 6)//64
} DiasSemana;

//SSDDDHHMMRRRR
typedef struct  {
	char Sector[2];
	char DiasSemana[3]; // Dia de la semana de modo enumeracion
	char HoraInicio[2];
	char MinutoInicio[2];
	char HoraRiego[2];
	char MinutoRiego[2];

} 	Programa, *ProgramaPtr_t;

typedef struct
{
	char Sector[2];
	char FechaInicio[12];
	time_t Hasta;
	//unsigned int Minutos;
} Riego, *RiegoPtr_t;

//En todos campos alamacenamos un caracter mas para indicar fin de cadena
typedef struct {
	char FechaActualizacion[15];//Fecha de ultima actualizacion de los programas YYYYMMDDHHmmss

	//-- Datos para almacenar la configuracion gprs
	char ServerAPN[64];
	char Login[25];
	char Password[25];
	char IPAddress[13];
	char Dns1[13];
	char Dns2[13];

	//-- Datos del ftp
	char FtpServer[26];
	char FtpUser[26];
	char FtpPassword[26];
	char FtpPuerto[6];

	//-- Numero de telefono a avisar
	char Pin[5];
	char Telefono1[10];
	char Telefono2[10];

	//-- Settings a avisar
	// Sms cuando inicia riego, sms cuando cambia config.. etc

} Configuracion, *ConfiguracionPtr_t;

#define PROGRAMAS_FILE "programas.txt"
#define CONFIG_FILE "settings.config"
#define PROGRAMAS_SIZE 50

#define FREQUENCY_FTP 5 //Frequencia en Minutos para que compruebe si hay nueva programación en el FTP
#define SECTORES_COUNT 8
#define PIN_INIT_SECTORES 22
#define PIN_CODE "2259"
#define NUM_TELEFONO "653316799"
#define MAX_CHARACTERS_SMS 140



//Constantes para las pantallas
//Pantallas
//Numero de pantallas de la app
#define PANTALLAS_NUM 5

#define MENU_PANTALLA 0
#define PROGRAMAS_PANTALLA 1
#define MANUALMENU_PANTALLA 2
#define CONSULTA_PANTALLA 3
#define PARAMETROS_PANTALLA 4

/*
#define TELEFONOMENU_PANTALLA 5
#define SIMPIN_PANTALLA 6
#define MENSAJE_PANTALLA 7
*/

extern Frame** pantallas[PANTALLAS_NUM];
extern Programa programas[PROGRAMAS_SIZE];
extern Riego riegos[PROGRAMAS_SIZE];
extern Configuracion config;
extern bool IsLockedATCommand;


bool FlashGuardaFichero(const char * archivo, const char *datos, const int size);
bool FtpEnviamosProgramas() ;
void setPantallaActual(uint8_t pantalla);
int Cobertura();
void LanzaRiego(uint8_t contador);
#endif /* TimeAlarms_h */
