/* 
* GSM.h
*
* Created: 21/03/2018 16:21:20
* Author: gtame
*/

#include <Logger.h>
#include <Time.h>
#include <SIM900.h>
#include "../settings.h"
#include "../Utils/util.h"
#include "Programa.h"
#include "Salida.h"
#include "Configuracion.h"


#ifndef __GSM_H__
#define __GSM_H__


class GSM:public SIM900
{
//variables
public:
protected:
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;
	tConfiguracion *config;

//functions
public:
	GSM(tConfiguracion *configuracion, Stream * stream,char *ibuffer,uint8_t isizebuff);



	
	bool CargaConfigWeb();//Carga los parametros de configuración web para que el modulo pueda conectar a internet

	//SMS
	void SendSmsFinReinicio(Programa *prog);
	void SendSmsIniReinicio();
	void SendSmsProgramacion(Programa *prog);
	void SendSmsSectoresEjecucion(Salida *salida);
	void SendSmsHora();

	virtual bool OnProcessResult( char * ) override;//overrides


	Stream * GetStream();

protected:

private:


}; //GSM

#endif //__GSM_H__
