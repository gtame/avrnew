/* 
* GSM.cpp
*
* Created: 21/03/2018 16:21:20
* Author: gtame
*/


#include "GSM.h"

// default constructor
GSM::GSM(tConfiguracion *configuracion, Stream * stream,char *ibuffer,uint8_t isizebuff):SIM900(stream)
{
		internalbuffer=ibuffer;
		sizebuffer=isizebuff;
		config=configuracion;
		
} //GSM


Stream * GSM::GetStream()
{
	return this->mSerial;
}

bool GSM::OnProcessResult( char * msg)
{
	
	return false;

	if (msg!=NULL)
	{


		//LLamadas
		//		if (strcmp_P(msg,(char *)F("RING"))==0)
		//		{
		//			//Log.Info("LLamada entrante");
		//
		//		}
		//		//Messages
		//		else if (strcmp_P(msg,(char *)F("NO CARRIER"))==0)
		//		{
		//			//Log.Info("fin de llamada");
		//
		//		}
		//SMS
		if (strncmp_P(msg,(char *)F("+CMT"),4)==0)
		{

			//uint8_t contador=0;
			memset(internalbuffer,0,sizebuffer);
			//Esperamos medio seg, para que reciba las siguientes lineas
			WaitResponse(400);
			if (ReadSerialLine(internalbuffer,sizebuffer)==RX_OK_READ)
			{
				///What do I do?
			}
		}

		LOG_INFO_ARGS("MSG => %s",msg);

	}
	return true;
}


bool GSM::CargaConfigWeb()
{
	bool result=false;
	ConfigAPN(F("CONTYPE"),"GPRS");
	SendCommandCheckError(F("AT+CSTT=\"%s\",\"%s\",\"%s\""),(const __FlashStringHelper*)ATSerial::AT_OK,(const __FlashStringHelper*)ATSerial::AT_ALL_ERRORS,config->APN,config->userAPN,config->pwdAPN);

	if (strlen(config->APN)>0)
		result=ConfigAPN(F("APN"),config->APN);

	//--Configuracion APN
	if (strlen(config->userAPN)>0)
		result=ConfigAPN(F("USER"),config->userAPN);

	//--UserAPN
	if (strlen(config->pwdAPN)>0)
		result=ConfigAPN(F("PWD"),config->pwdAPN);


	return result;
}


void GSM::SendSmsHora()
{
	#ifdef SMS
	time_t current_minute=now();
	breakTime( current_minute, timeEl);
	SmsMessage(PBB(F("Hora: %02i/%s/%02i %02i:%02i:%02i\n"),timeEl.Day, monthStr(timeEl.Month),tmYearToY2k(timeEl.Year), timeEl.Hour,timeEl.Minute,timeEl.Second));
	#endif
}

//Envia info con los sectores en ejecucion
void GSM::SendSmsSectoresEjecucion(Salida *salida)
{
	//#ifdef SMS
	LOG_INFO("Enviando SMS Sectores en ejecucion!!");


	if (config->MovilAviso!=NULL && strlen(config->MovilAviso)>0)
	{
		SmsOpen(config->MovilAviso);


		if (salida->GetSalidasActivas()>0)
		{
			SmsMessage_P(PSTR("Sectores con riegos activos:\n"));

			//Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
			for(uint8_t i=0;i< MAX_PROGRAMAS;i++)
			{
				switch (salida->salidas[i].Tipo)
				{
					case actPrograma:
					{
						SmsMessage(PBB(internalbuffer,sizebuffer, F("Programa %i Sector %i\n"),salida->salidas[i].Ident,salida->salidas[i].Sector));
					}
					break;
					case actSector:
					{
						SmsMessage(PBB(internalbuffer,sizebuffer,F("Sector %i\n"),salida->salidas[i].Ident));
					}
					break;
					case actAbono:
					{
						SmsMessage_P(PSTR("Bomba abono activada\n"));

					}
					break;
					case actMotor:
					{
						SmsMessage_P(PSTR("Motor activado\n"));
					}
					break;
					case actLimpieza:
					{
						//---No implementado
					}
					break;
				}
			}
		}
		else
		//Si no hay en ejecucion . lo indicamos
		SmsMessage_P(PSTR("No hay ningun riego activado"));

		SmsSend();
	}
	//#endif
}

//Envia alerta cuando se completo el reinicio
void GSM::SendSmsFinReinicio(Programa* programa)
{
	#ifdef SMS


	if (IsGSMEnable() && (config.AvisosSMS & SMSReset))
	{
		SmsOpen(config.MovilAviso);

		SmsMessage_P(PSTR("Reiniciado GTKeeper con exito\n"));
		SendSmsHora();
		SmsMessage(PBB(internalbuffer,sizebuffer,F("Password SMS: %s\n"),config.PasswordSMS));
		SmsMessage(PBB(internalbuffer,sizebuffer,F("Aviso SMS: %i\n"),config.AvisosSMS));
		SmsMessage(PBB(internalbuffer,sizebuffer,F("Imei: %s\n"),config.Imei));
		SendSmsProgramacion(riegos);


		SmsSend();

	}
	#endif

}

//Envia por SMS alerta de reinicio
void GSM::SendSmsIniReinicio()
{

	#ifdef SMS

	//Esperamos unos segundos..
	if (IsGSMEnable() && (config.AvisosSMS & SMSReset))
	{
		delay(1000);
		Sms_P(config.MovilAviso,PSTR("Reiniciando GTKeeper.."));
	}

	#endif
}

//Envia por SMS la programacion, logicamente ha de estar  GSM enable
void GSM::SendSmsProgramacion(Programa *prog)
{
	#ifdef SMS
	if (IsGSMEnable())
	{
		LOG_INFO("Enviando SMS!!");

		bool tiene_programas=false;
		//Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
		for(uint8_t program=0;program< MAX_PROGRAMAS;program++)
		{

			if (prog->programas[program].TiempoRiego>0)
			{
				LOG_DEBUG_ARGS_B("Enviando programa %i",program);

				tiene_programas=true;
				memset(internalbuffer,0,sizebuffer);
				ProgramaToString(internalbuffer,&programas[program]);
				internalbuffer[strlen(internalbuffer)]='\n'; //Salto linea por programa
				SmsMessage(internalbuffer);

			}
		}



		//Si hay algun pdte de enviar..
		if (!tiene_programas)
		{
			SmsMessage_P(PSTR("No hay programas cargados"));
		}
	}
	#endif

}