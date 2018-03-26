/* 
* Configuracion.cpp
*
* Created: 18/03/2018 16:24:32
* Author: Gabi
*/


#include "Configuracion.h"

// default constructor
Configuracion::Configuracion(char * ibuffer,uint8_t isizebuffer)
{
	internalbuffer=ibuffer;
	sizebuffer=isizebuffer;
	changed=false;
	strcpy_P(config.MovilAviso, PSTR("000000000"));
	strcpy_P(config.PasswordSMS,PSTR("0000"));
} //Programa


void Configuracion::ShowConfigInfo()
{
	LOG_INFO_ARGS("Imei %s",config.Imei);
	LOG_INFO_ARGS("Movil %s",config.MovilAviso);
	LOG_INFO_ARGS("PwdSMS %s",config.PasswordSMS);
	LOG_INFO_ARGS("APN %s",config.APN);
	LOG_INFO_ARGS("PwdAPN %s",config.userAPN);
	LOG_INFO_ARGS("UserAPN %s",config.pwdAPN);




	LOG_INFO_ARGS("AvisoSMS %i",config.AvisosSMS);
	LOG_INFO_ARGS("\tReset %s",(config.AvisosSMS & SMSReset?"si":"no"));
	LOG_INFO_ARGS("\tInicio %s",(config.AvisosSMS & SMSInicioSector?"si":"no"));
	LOG_INFO_ARGS("\tFin %s",(config.AvisosSMS & SMSFinSector?"si":"no"));
	LOG_INFO_ARGS("\tProgramacion %s",(config.AvisosSMS & SMSProgramacionWeb?"si":"no"));



	LOG_INFO_ARGS("MotorDiesel %i",config.motor_diesel);
	LOG_INFO_ARGS("NumAbono %i",config.numabono);
	LOG_INFO_ARGS("NumPuertos %i",config.numpuertos);
	LOG_INFO_ARGS("GSM %s",config.GSMAvailable?"si":"no");
}

void Configuracion::ResetConfig()
{
	memset(&config,0,sizeof(tConfiguracion));
	config.motor_diesel=false;

	strcpy_P(config.PasswordSMS,PSTR("1111"));
	config.numabono=0;
	config.numpuertos=PORTS_NUM;
	config.GSMAvailable=true;
	//Mandar sms por defecto
	config.AvisosSMS=(uint8_t)( SMSNone);
}

bool Configuracion::CargaConfigDesdeSim()
{

/*
	if (ExisteContactoSIM(1) && 	ExisteContactoSIM(2) && 	ExisteContactoSIM(3))
	{
		SetChangedConfig(true);
		GetSIMContact(1,config.MovilAviso,NULL);
		GetSIMContact(2,config.PasswordSMS,NULL);
		GetSIMContact(3,NULL,config.APN);
		GetSIMContact(4,NULL,config.userAPN);
		GetSIMContact(5,NULL,config.pwdAPN);
		return true;
	}
	else
	return false;*/
}

bool Configuracion::CargaConfigDesdeString(char* configstr)
{
	 char buffernumber[4];

////// VALIDACIONES
	//El internalbuffer es usado
	//if (configstr==internalbuffer)
	//	return false;
		
	if (strlen(configstr)!= LEN_CONFIG_STRING)
		return false;

	//Todos objetos deben ser numericos
	if (!isValidNumber(configstr))
		return false;
		
	//12765331679911111215
////// ENDVALIDACIONES
	 
	//3 para AvisosSMS
	memset(buffernumber,0,4);
	strncpy(buffernumber,configstr,3);
	config.AvisosSMS=atoi(buffernumber);

	//6 siguientes para movil
	strncpy(config.MovilAviso,configstr+3,9);

	//NUM 4 password sms
	strncpy(config.PasswordSMS,configstr+12,4);

	//BOOL 1 o 0 para motor diesel
	memset(buffernumber,0,4);
	strncpy(buffernumber,configstr+16,1);
	config.motor_diesel=atoi(buffernumber);
	//
	//NUM 1 para abono
	memset(buffernumber,0,4);
	strncpy(buffernumber,configstr+17,1);
	config.numabono=atoi(buffernumber);

	//NUM 2 para puertos
	memset(buffernumber,0,4);
	strncpy(buffernumber,configstr+18,2);
	config.numpuertos=atoi(buffernumber);

	SetChangedConfig(true);

	return true;
}

//12765331679911111215
//3 para AvisosSMS
//9 siguientes para movil
//NUM 4 password sms
//BOOL 1 o 0 para motor diesel
//NUM 1 para abono
//NUM 2 para puertos
void Configuracion::ConfiguracionToString(char *text)
{
	sprintf_P(text, PSTR("%03i%s%s%c%i%02i"), config.AvisosSMS, config.MovilAviso, config.PasswordSMS,(config.motor_diesel?'1':'0'),config.numabono,config.numpuertos);
}

void Configuracion::EEPROMGuardaConfig()
{
	
	tConfiguracion configEeprom;

	 if (EEPROMCargaConfig(&configEeprom,false))
	 {
		//La lectura de la eeprom es mucha mas rapida que la escritura, ademas el nº de escrituras en una eeprom es limitado
		//Por lo que comprobaremos antes de escribir que ha cambiado (memcmp)
		if (memcmp((void *)&config,(void *)&configEeprom,sizeof(tConfiguracion)) !=0)
		{	

			uint16_t address=GET_ADDRES_CONFIG;
			config.flag_check='X';
			//Cargamos la configuracion
			while (!eeprom_is_ready());
			eeprom_write_block((void*)&config, ( void*)address , sizeof(tConfiguracion));
		}
	} 

	SetChangedConfig(false);
}

bool Configuracion::EEPROMCargaConfig(tConfiguracion* configload,bool validateflag)
{
	configload->flag_check='\0';//Lo ponemos a false para comprobar que carga bien la config

	//Cargamos la configuracion
	eeprom_read_block((void*)configload, (const void*) GET_ADDRES_CONFIG, sizeof(tConfiguracion));
	
	if(validateflag)
		return (configload->flag_check=='X');
	else 
		return true;
}

bool Configuracion::EEPROMCargaConfig()
{
	return EEPROMCargaConfig(&config,true);

}
 