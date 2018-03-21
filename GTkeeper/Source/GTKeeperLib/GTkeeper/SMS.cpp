/*
 * gtKeeperSMS.cpp
 *
 * Created: 18/03/2018 18:30:15
 *  Author: Gabi
 */ 

 #include "../gtkeeper.h"

 

 void GTKeeper::SendSmsHora()
 {
	 #ifdef SMS
	 time_t current_minute=now();
	 breakTime( current_minute, timeEl);
	 SmsMessage(PBB(F("Hora: %02i/%s/%02i %02i:%02i:%02i\n"),timeEl.Day, monthStr(timeEl.Month),tmYearToY2k(timeEl.Year), timeEl.Hour,timeEl.Minute,timeEl.Second));
	 #endif
 }

 //Envia info con los sectores en ejecucion
 void GTKeeper::SendSmsSectoresEjecucion()
 {
	 //#ifdef SMS
	 LOG_INFO("Enviando SMS Sectores en ejecucion!!");


	 if (config.MovilAviso!=NULL && strlen(config.MovilAviso)>0)
	 {
		 SmsOpen(config.MovilAviso);


		 if (GetSalidasActivas()>0)
		 {
			 SmsMessage_P(PSTR("Sectores con riegos activos:\n"));

			 //Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
			 for(uint8_t i=0;i< MAX_PROGRAMAS;i++)
			 {
				 switch (salidas[i].Tipo)
				 {
					 case actPrograma:
					 {
						 SmsMessage(PBB(F("Programa %i Sector %i\n"),salidas[i].Ident,salidas[i].Sector));
					 }
					 break;
					 case actSector:
					 {
						 SmsMessage(PBB(F("Sector %i\n"),salidas[i].Ident));
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
		 SmsMessage(PBB(F("No hay ningun riego activado")));

		 SmsSend();
	 }
	 //#endif
 }

 //Envia alerta cuando se completo el reinicio
 void GTKeeper::SendSmsFinReinicio()
 {
	 #ifdef SMS


	 if (IsGSMEnable() && (config.AvisosSMS & SMSReset))
	 {
		 SmsOpen(config.MovilAviso);

		 SmsMessage_P(PSTR("Reiniciado GTKeeper con exito\n"));
		 SendSmsHora();
		 SmsMessage(PBB(F("Password SMS: %s\n"),config.PasswordSMS));
		 SmsMessage(PBB(F("Aviso SMS: %i\n"),config.AvisosSMS));
		 SmsMessage(PBB(F("Imei: %s\n"),config.Imei));
		 SendSmsProgramacion();
		 SmsSend();

	 }
	 #endif

 }

 //Envia por SMS alerta de reinicio
 void GTKeeper::SendSmsIniReinicio()
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
 void GTKeeper::SendSmsProgramacion()
 {
	 #ifdef SMS
	 if (IsGSMEnable())
	 {
		 LOG_INFO("Enviando SMS!!");

		 bool tiene_programas=false;
		 //Hacemos un bucle para recorrer todos los programas iremos agregandolos a los sms.
		 for(uint8_t program=0;program< MAX_PROGRAMAS;program++)
		 {

			 if (programas[program].TiempoRiego>0)
			 {
				 LOG_DEBUG_ARGS_B("Enviando programa %i",program);

				 tiene_programas=true;
				 memset(buffer,0,MAIN_BUFFER_SIZE);
				 ProgramaToString(buffer,&programas[program]);
				 buffer[strlen(buffer)]='\n'; //Salto linea por programa
				 SmsMessage(buffer);

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