/*
 * main.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */
//Referencia a la libreria comun de avr
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
//Referencia al core de arduino
#include <Arduino.h>


//Referencia libreria SIm900
#include <Sim900.h>

//NO BORRAR - esta funcion puede ser llamada durante la construccion/destruccion de objetos
extern "C" void __cxa_pure_virtual() {}

#define NUM_RELES 2
#define PIN_RELE 4

bool bSetupCompleted=false;

//Clase para hacer el log
#ifdef LOG
	#ifdef LOG_SOFTSERIAL
		Logger Log(2,3);
	#else
		Logger Log;
	#endif
#endif


#ifdef SIM900_SOFTSERIAL
	SIM900 Sim900(10,11);
#else
	SIM900 Sim900;
#endif

#define PIN_STATUS 6 //Indicador de si esta working o esta en estado de error
#define PIN_MODE 7 // Pin que indica si trabaja en 1- modo SMS o 2- en modo Phone o SMS

/*
 --Automaicamente coger llaamada pasado dos tonos
ATS0=3
--DESCOLGAMOS LA lamada
ATA
--Activamos DT
AT+DDET=1
--OK
AT+CLDTMF=5,"F,F",100,1
AT+CLDTMF=5,"F",500,1
--Plsacon de tecla
AT+CLDTMF=1,"F",50,1
 * */
bool ProcessATMensajes(char * msg)
{
		char command[25];
		uint8_t index=0;

		if (msg!=NULL && bSetupCompleted)
		{
			//LLamadas
			if (strncmp_P(msg,(char *)F("+CRING"),6)==0)
			{
				memset(command,0,25);
				index=0;
				//Log.Info("LLamada entrante");
				LOG_INFO_ARGS("LLAMADA => %s",msg);
				//Descolgamos
				Sim900.SendCommandCheck(F("ATA"),F("OK"));
				//Activamos DT
				Sim900.SendCommandCheck(F("AT+DDET=1"),F("OK"));

				//REalizamos pitido largo
				delay(1000);
				Sim900.SendCommandCheck(F("AT+CLDTMF=5,\"F\",200,1"),F("OK"));

				//Ahora esperamos que cuelgue la llamada 'NO CARRIER', Pulsacion de tecla,
			 	uint8_t result=RX_NO_DATA;
			 	do
			 	{
			 	     Sim900.ProcessResults(10);
					 //Esperamos respuesta
					 Sim900.WaitResponse(10000);


					 //Si la lectura ha ido bien lo mandamos a procesar
					 if ((result=Sim900.ReadSerialLine())==RX_OK_READ)
					 {
						char * response=Sim900.GetLastResponse();
						//Fin de llamada salir
						if (strcmp_P(response,(char *)F("NO CARRIER"))==0)
						{
							Log.Debug("Salida no carrier");
							result=RX_NO_DATA;
						}
						//Password
						else if (strncmp_P(response,(char *)F("+DTMF:"),6)==0)
						{
								strcpy(command+index,response+6);
								Log.Debug(response+6);

								//delay(100);
								//Sim900.SendCommandCheck(F("AT+CLDTMF=1,\"%c\",40,1"),F("OK"),command[index]);
								//delay(50);
								//EL index se suma al final por eso es uno mes
								if (index==5)
								{
									if (strcmp(command,"#1234#")==0)
									{
										//Enviamos otro pitido largo
										Sim900.SendCommandCheck(F("AT+CLDTMF=5,\"F\",100,1"),F("OK"));

										delay(50);
										Sim900.SendCommandCheck(F("AT+CLDTMF=5,\"F\",100,1"),F("OK"));

									}
									else
									{
										//PWd incorrecta --Salir
										Log.Debug("PWd incorrecta");
										result=RX_NO_DATA;
									}
								}
								else if (index>5 && index<=(5+NUM_RELES))
								{
										if (command[index]=='0')
										{
											//Apagar rele
											Log.Debug("desActiva rele");
											digitalWrite( PIN_RELE+(index-6),LOW);
										}
										else if (command[index]=='1')
										{
											Log.Debug("Activa rele");
											//Encender rele
											digitalWrite( PIN_RELE+(index-6),HIGH);

										}
										else if (command[index]=='#')
										{
											//DEjar como esta == (No hacer nada)
											Log.Debug("Deja rele =");
										}
										//No entiendo comando--SALIR
										else
										{
											result=RX_NO_DATA;
											Log.Debug("No entiendo");
										}

								}


								//Si era el ultimo comando , terminamos llamada
								if (index==5+NUM_RELES)
								{
									result=RX_NO_DATA;
									Log.Debug("Final comando");
								}


								index++;
							}
							else
							{
								Log.Debug("Salida comado desconocido");
									//>>dev/null
							}
						}
						//Otros msgs nos la soplan
						else
						{
							Log.Debug("Salida result false");
								//>>dev/null
						}




				 }while (result!=RX_NO_DATA  || Serial.available());


			 	Log.Debug("Salio dowhile");
			 	Sim900.SendCommandAsync(F("AT+DDET=0"));
			 	Sim900.ProcessResults();

				//Colgamos
				Sim900.SendCommandAsync(F("ATH"));

			}
			//Messages
			else if (strcmp_P(msg,(char *)F("NO CARRIER"))==0)
			{
				//Log.Info("fin de llamada");
				LOG_INFO("FIN DE LLAMADA");

			}
			//SMS
			else if (strncmp_P(msg,(char *)F("+CMT"),4)==0)
			{
				LOG_INFO_ARGS("SMS => %s",msg);


			}

			LOG_INFO_ARGS("MSG => %s",msg);
		}

}



//Configuramos pins, serials y lo que se ejecuta en la inicialización
void setup() {

	//Configura puerto Serie
	Serial.begin(9600);


	//Pin de salida
	pinMode(PIN_STATUS,OUTPUT);
	pinMode(PIN_GSM_ON,OUTPUT);
	digitalWrite(PIN_GSM_ON,LOW);

	for (uint8_t i=0;i<NUM_RELES;i++)
	{
		pinMode(PIN_RELE+i,OUTPUT);
		digitalWrite(PIN_RELE+i,LOW);
	}

	//Establecemos los punteros a funcion
	 Sim900.ProcessResultPtr = ProcessATMensajes;

	 uint8_t num_veces=0;
	 //Arranca Modulo
	 while(!Sim900.ActivaModulo())
	 {
		 delay(500);
		 num_veces++;
	 }

	 Log.Debug("Setup Completed");
	 bSetupCompleted=true;
}


void loop() //----( LOOP: RUNS CONSTANTLY )----
{
	//Activa pin 13
	digitalWrite(PIN_STATUS,HIGH);
    Sim900.ProcessResults(1000);

	//Desactiva pin 13
	digitalWrite(PIN_STATUS,LOW);
	Sim900.ProcessResults(1000);

}

int main()
{

	//Necesario para la inicializacion de arduino - No sobreescribir
	init();

	//Setup - Para guardar 'compatibilidad' con arduino
	setup();


	//Bucle infinito Loop - Para guardar 'compatibilidad' con arduino
	while (true)
		loop();

}


