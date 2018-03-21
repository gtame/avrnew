
#ifndef TEXTOS_H_
#define TEXTOS_H_



const static char TXT_UNKNOW[] PROGMEM= "**";;
const static char TXT_OK[] PROGMEM= "OK";;
const static char TXT_ERROR[] PROGMEM="ERROR";
const static char TXT_CONFIG[] PROGMEM= "CONFIGURACION";
const static char TXT_MOD_GSM[] PROGMEM="MODULO GSM";
const static char TXT_SIM[] PROGMEM="SIM";
const static char TXT_CONECTIVIDAD[] PROGMEM="RED GSM";
const static char TXT_SMS[] PROGMEM="ENVIO SMS";
const static char TXT_SD[] PROGMEM ="TARJETA SD";
const static char TXT_RELOJ[] PROGMEM ="RELOJ";
const static char TXT_PROGRAMAS[] PROGMEM ="PROGRAMAS";
const static char TXT_WWW[] PROGMEM ="PETICION WEB";

#define TEXT_PROGRAMAS "1-Programas"
#define TEXT_MANUAL "2-Manual"
#define TEXT_PARAMETROS "3-Parametros"
#define TEXT_ESTADISTICAS "4-Estadisticas"


//ManualMenu
#define MNU_MAN_TEXT_LANZAR_PROGRAMA "1-Lanzar Programa"
#define MNU_MAN_TEXT_LANZAR_SECTOR "2-Activar Sector"
#define MNU_MAN_TEXT_PARAR "3-Parar"
#define MNU_MAN_TEXT_LIMPIAR "4-Limpiar"


#define MNU_PRG_TEXT_PROGRAMAR "1-Programas"
#define MNU_PRG_TEXT_PRG_TANDA "2-Programar tanda"
#define MNU_PRG_TEXT_RESET "3-Resetar"


#define MNU_PARAM_TEXT_GSM "1-GSM"
#define MNU_PARAM_TEXT_HORA "2-HORA"
#define MNU_PARAM_TEXT_SYS "3-SISTEMA"


#define MNU_GSM_TEXT_NUM "1-NUMERO AVISO"
#define MNU_GSM_TEXT_SMS "2-SMS CONFIG"
#define MNU_GSM_TEXT_APN "3-APN"
#define MNU_GSM_TEXT_USR "4-APN USUARIO"
#define MNU_GSM_TEXT_PWD "5-APN CONTRASENA"
#define MNU_GSM_TEXT_TEST_SMS "6-TEST SMS"
#define MNU_GSM_TEXT_TEST_WEB "7-TEST WEB"



#define MNU_STA_TEXT_ESTADISTICA "1-Consultar"
#define MNU_STA_TEXT_RESET "2-Resetear"
#endif
