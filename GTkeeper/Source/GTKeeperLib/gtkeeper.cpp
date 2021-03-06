#include "gtkeeper.h"

/*
 * gtkeeper.c
 *
 *  Created on: 12 de ene. de 2016
 *      Author: gtame
 */

GTKeeper::GTKeeper(Configuracion *iconfiguracion,	GSM* gsmmodem,Riegos * iriegos,LogSD* ilogsd,char * ibuffer,uint8_t isize) :StateMachine(9,13),Hora()
{
	buffer=ibuffer;
	buffersize=isize;
	configuracion=iconfiguracion;
	config=&configuracion->config;
	gsm=gsmmodem;
	riegos=iriegos;
	logsd=ilogsd;
}

void GTKeeper::Setup()
{
	//Configuramos la maquina
	setupStateMachine();
	//La inicializamos a ON
	SetState(ON, false, true);
}


void GTKeeper::Sleep()
{
	//Se duerme el micro y el modulo gsm
	gsm->Sleep();
	//Utilizo macro pq en proteus no le sienta bien asi que hago un sleep;
	GTKEEPER_SLEEP(SLEEP_8S);//LowPower.powerDown(SLEEP_8S, ADC_OFF,	 BOD_OFF);
}

void GTKeeper::setLed(uint8_t led )
{
	(led!=LED_WORKING_PIN? digitalWrite(LED_WORKING_PIN,LOW):digitalWrite(LED_WORKING_PIN,HIGH));
	(led!=LED_ERROR_MODULE_PIN? digitalWrite(LED_ERROR_MODULE_PIN,LOW):digitalWrite(LED_ERROR_MODULE_PIN,HIGH));
	(led!=LED_ERROR_SIM_PIN? digitalWrite(LED_ERROR_SIM_PIN,LOW):digitalWrite(LED_ERROR_SIM_PIN,HIGH));
	(led!=LED_ERROR_NETWORK_PIN?digitalWrite(LED_ERROR_NETWORK_PIN,LOW):digitalWrite(LED_ERROR_NETWORK_PIN,HIGH));
}

void GTKeeper::setupStateMachine()
{
	
 	//Fijamos las transiciones de estados
	
	//ON
	AddTransition(ON, Init, []() {  return gtKeeper.CheckInit() ;});
	AddTransition(ON, Reset, []() {  return gtKeeper.CheckReset();});

	//Init
	AddTransition(Init, Error, []() { return gtKeeper.CheckError(); });
	AddTransition(Init, User, []() { return true; });	
    
    //User
	AddTransition(User, Run, []() { return true; });
    
	//Run
	AddTransition(Run, User, []() { return gtKeeper.CheckUser(); });
	AddTransition(Run, SMS , []() { return gtKeeper.CheckSMS(); });
	AddTransition(Run, Call , []() { return gtKeeper.CheckCall(); });
	AddTransition(Run, Web , []() { return gtKeeper.CheckWeb(); });
	AddTransition(Run, Run , []() { return gtKeeper.CheckRun(); });
	
	//SMS
	AddTransition(SMS, Run , []() { return true; });
	
	//Web
	AddTransition(Web, Run , []() { return true; });
		
	//Call
	AddTransition(Call, Run , []() { return true; });	
		
		
	//AddTransition(Run, Run,[]() { return true;});//(gtKeeper.GetState()==Run); });
/*
	stateMachine.AddTransition(PosicionA, PosicionB, []() { return input == Forward; });
	
	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionB, PosicionC, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Reset; });
	
	stateMachine.AddTransition(PosicionC, PosicionB, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionC, PosicionD, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionC, PosicionA, []() { return input == Reset; });
	
	stateMachine.AddTransition(PosicionD, PosicionC, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionD, PosicionA, []() { return input == Reset; });
	*/

	///Fijamos las acciones de salida para cada estado
	SetOnEntering(ON,  []() { gtKeeper.OnON();});
 	SetOnEntering(Init,  []() { gtKeeper.OnInit();});
	SetOnEntering(Reset,  []() { gtKeeper.OnReset();});
	SetOnEntering(Error,  []() { gtKeeper.OnError();});
	SetOnEntering(User,  []() { gtKeeper.OnUser();});
	SetOnEntering(Run,  []() { gtKeeper.OnRun();});
	SetOnEntering(SMS,  []() { gtKeeper.OnSMS();});
	SetOnEntering(Web,  []() { gtKeeper.OnWeb();});
	SetOnEntering(Call,  []() { gtKeeper.OnCall();});
	

	/*stateMachine.SetOnEntering(PosicionB, outputB);
	stateMachine.SetOnEntering(PosicionC, outputC);
	stateMachine.SetOnEntering(PosicionD, outputD);*/
	
	//Fijamos las acciones a ejecutar cuando se abandona un determinado estado
	SetOnLeaving(ON,  []() { gtKeeper.OnLeaveON();});
	SetOnLeaving(Init,  []() { gtKeeper.OnLeaveInit();});
	SetOnLeaving(Reset,  []() { gtKeeper.OnLeaveReset();});
	SetOnLeaving(Error,  []() { gtKeeper.OnLeaveError();});
	SetOnLeaving(User,  []() { gtKeeper.OnLeaveUser();});
	SetOnLeaving(Run,  []() { gtKeeper.OnLeaveRun();});
	SetOnLeaving(SMS,  []() { gtKeeper.OnLeaveSMS();});
	SetOnLeaving(Web,  []() { gtKeeper.OnLeaveWeb();});
	SetOnLeaving(Call,  []() { gtKeeper.OnLeaveCall();});
	
}

bool GTKeeper::ExecuteCommand(char* commandstr)
{

bool error	=false;

//La password debera ir entre corchetes #1111#
memset(buff_parse,0,MAIN_BUFFER_PARSE);
sprintf(buff_parse,"#%s#",config->PasswordSMS);

	
//Comprobamos que contiene la pwd correcta
if (strncmp(commandstr,buff_parse,strlen(buff_parse))==0)
{
	
	//Puntero al comando
	char* command =commandstr+strlen(buff_parse);
	//Puntero al Sector
	char * sectorptr=command;

	//Si es comando de arrancar sector-- Mays or Min
	if (command[0]=='P' || command[0]=='p')
	{
		sectorptr=sectorptr+1;
		int programa=atoi(sectorptr);
		LOG_DEBUG_ARGS ("PROGRAMA %i %s",programa,sectorptr);
		if (programa>0 && programa<=MAX_PROGRAMAS)
		{
			#ifdef SMS
			gsm->SmsOpen(config->MovilAviso);
			#endif
			riegos->LanzaRiego(programa-1,true);
			#ifdef SMS
			gsm->SendSmsHora();
			gsm->SmsSend();
			#endif
			
		}
		else
			error=true;
	}
	else if (strncmp(sectorptr,"APN:",4)==0)
	{
		bool bresult=false;
		sectorptr=sectorptr+4;
		if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
		{
			strcpy(config->APN,sectorptr);
			configuracion->EEPROMGuardaConfig();
			if (gsm->CargaConfigWeb())
			{
				gsm->Sms(config->MovilAviso,PBB(buffer,buffersize, F("APN Configurado %s"),config->APN));
				bresult=true;
			}
		}
		
		
		if (!bresult)
		{
			gsm->Sms(config->MovilAviso,PBB(buffer,buffersize,F("Incorrecto APN: %s"),sectorptr));
			error=true;
		}
	
	}
	else if (strncmp(sectorptr,"USERAPN:",8)==0)
	{
		sectorptr=sectorptr+8;
		if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
		{
			strcpy(config->userAPN,sectorptr);
			configuracion->EEPROMGuardaConfig();
			gsm->CargaConfigWeb();
			gsm->Sms(config->MovilAviso,PBB(buffer,buffersize,F("User APN Configurado %s"),sectorptr));
		}
		else
		{
			gsm->Sms(config->MovilAviso,PBB(buffer,buffersize,F("Incorrecto User APN: %s"),sectorptr));
			error=true;
		}
		


	}
	else if (strncmp(sectorptr,"PWDAPN:",7)==0)
	{
		sectorptr=sectorptr+7;
		if (strlen(sectorptr)<25 && strlen(sectorptr)>0)
		{
			strcpy(config->pwdAPN,sectorptr);
			configuracion->EEPROMGuardaConfig();
			gsm->CargaConfigWeb();
			gsm->Sms(config->MovilAviso,PBB(buffer,buffersize,F("Pwd APN Configurado %s"),sectorptr));
		}
		else
		{
			error=true;
			gsm->Sms(config->MovilAviso,PBB(buffer,buffersize,F("Incorrecto PWd APN: %s"),sectorptr));
		}
		


	}
	else
	{

		//Comandos numericos
		int sector=atoi(sectorptr);

		//El comando  77 -- Quiere muestra info! :)
		if (sector==77)
		{

			#ifdef SMS

			gsm->SmsOpen(config->MovilAviso);
			gsm->SmsMessage_P(PSTR("Estoy preparado!\n"));
			gsm->SendSmsHora();
			gsm->SendSmsProgramacion();
			gsm->SmsSend();

			#endif
						

		}
		//Si el comando es 88 -- Quiere decir que queremos forzar el chequeo web
		//Comprobamos tb que no estamos en pleno proceso de actualizacion web
		if (sector==88)
		{

			bpendingWeb=true; //Ponemos pendiente
	 	}
		//else if (sector==89)
		//{
			//this->CheckWebConfig();
//
		//}
		//Si el comando es 99 quiere decir que apague todos sectores
		//Y si esta ejecutandose la programacion que la cancele
		else if ( sector==99)
		{
			riegos->ApagarRiegos();
			#ifdef SMS
			if (config->AvisosSMS & SMSFinSector)
			{

				Sms_P(config->MovilAviso,PSTR("Parados todos programas"));
			}
			#endif
		}
		//Comando 101 este comando quiere decir que enciende la bomba
		else if (sector==101)
		{

			//Dependiendo si esta encendido o no
			//OJO!!!NOTAR QUE ENCENDIDO ES ESTADO LOW, SI ES HIGH LA BOMBA NO FUNCIONA
			/*
			if (riegos->GetPosicion(1,actAbono)!=-1)
			{
				riegos->ApagaAbono(1);
				#ifdef SMS
				if (config->AvisosSMS & SMSFinSector)
				gsm->Sms_P(config->MovilAviso,PSTR("Apagada bomba de abono por SMS"));
				#endif
			}
			else
			{
				riegos->EnciendeAbono(1);
				#ifdef SMS
				if (config->AvisosSMS & SMSInicioSector)
					gsm->Sms_P(config->MovilAviso,PSTR("Arrancada bomba de abono por SMS"));
				#endif
			}*/

		}
		//Comando para mostrar la información de estado del reiego
		else if (sector==100)
		{
			gsm->SendSmsSectoresEjecucion(riegos);

		}
		//Si el comando esta dentro del rango de sectores lo arrancamos
		else if (sector>0 && sector<=PORTS_NUM )
		{
			//Apaga todos riegos
			riegos->ApagarRiegos();
			//Arranca el sector indicado
			LOG_INFO_ARGS("Arrancar sector %i",sector);
			riegos->EnciendeSector(sector);

		}
		else
			error=true;
	}
}
else
	error=true; // No password
	
	return !error;
}
 
//Loop para procesar
/*
bool GTKeeper::LoopGSM()
{
	bool hashError=false;
	//if (IsGSMEnable())
	//{
			////Comprobamos si hay que lanzar/parar sectores
			////Chequeamos si esta arrancado
			//if (EstaArrancado() && error_web_salidas<2)
			//{
				////Si la sim esta ok
				//if (CheckSIM())
				//{
					////Chequeamos si esta registrado en la red movil
					//if (EstaRegistrado())
					//{
//
//
						////Si se ha sido re-inicializado el modulo GSM
						////Volvemos a cargar la configuración web
						//if (bRebootSIM)
						//{
							//CargaConfigWeb();
							//bRebootSIM=false;
						//}
//
						////Encendemos los leds de trabajando
						//OnChangeStatus(LED_WORKING_PIN);
//
						////Si ha pasado el tiempo desde la ultima actualización comprobamos si hay
		//#ifndef NO_CHECK_WEB //Preprocesador para chequeo web
						//CheckWeb(); //Se hará por comando SMS #PWDSMS#,88
		//#else
						////En caso que tengamos pendiente refresco web...
						//if (IsPendingWeb())
								//CheckWeb();
		//#endif
//
					//}
					//else
					//{
						//hashError=true;
//
						//LOG_ERROR_ARGS("GT-CREG-%i",ERROR_NO_REGISTER);
//
						//OnChangeStatus(LED_ERROR_NETWORK_PIN);
					//}
				//}
				//else
				//{
					//hashError=true;
					////Registrando en la red
					//LOG_ERROR_ARGS("GT-SIM-%i",ERROR_SIM);
					//OnChangeStatus(LED_ERROR_SIM_PIN);
//
				//}
			//}
			//else
			//{
					//hashError=true;
					//LOG_CRITICAL_ARGS("GT-AT-%i",ERROR_NO_RESPOND);
					////Encendemos los led error en modo tranquilo
					//OnChangeStatus(LED_ERROR_MODULE_PIN);
					//if (error_web_salidas>2)
						//SwitchModule();
//
					//error_web_salidas=0;
					//bRebootSIM=true;
					//ActivaModulo();
			//}
	//}

	return hashError;
}

*/