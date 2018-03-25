/* 
* Configuracion.h
*
* Created: 18/03/2018 16:24:32
* Author: Gabi
*/

#include <stdint.h>
#include <Logger.h>
#include "../types.h"
#include "../settings.h"
#include "../Utils/util.h"

#ifndef __CONFIGURACION_H__
#define __CONFIGURACION_H__


class Configuracion
{
//variables
public:
protected:
private:
	bool changed;//Flag si indica que cambio la configuracion 
  	char * internalbuffer;
  	uint8_t sizebuffer;


//functions
public:
	Configuracion(char * internalbuffer,uint8_t sizebuffer); 

	tConfiguracion config;
	/*
	//Getters
	inline char * GetAPN() { return config.APN; }
	//Setters
	inline void SetAPN(char *apn) 
	{ 
		if (strcmp(config.APN,apn)!=0)
		{
			strcpy(config.APN,apn);
			changed==true
		}
	}**/
		
	void ShowConfigInfo();
	void ResetConfig();
	void ConfiguracionToString(char *text);
	bool CargaConfigDesdeSim();
	bool CargaConfigDesdeString(char* configstr);
	void EEPROMGuardaConfig();
	bool EEPROMCargaConfig();

	//Indica si Gtkeeper esta configurado con modulo gsm
	inline bool IsGSMEnable(){	return config.GSMAvailable;}
	inline bool GetChangedConfig() { return changed;}
protected:
	inline void SetChangedConfig(bool value) { changed=value;}
	bool EEPROMCargaConfig(tConfiguracion* configload,bool validateflag);
private:


}; //Configuracion


#endif //__CONFIGURACION_H__
