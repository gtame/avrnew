/*
 * hilo.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */

#include "hilo.h"

#include <string.h>

#include <pins_arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>


SoftwareSerial mySerial(3,4);

void SwitchModule() {


	LogInfo("Arrancando modulo");

	//PIN de control del mnodulo GSM
	pinMode(PIN_RESET, OUTPUT);
	digitalWrite(PIN_RESET, LOW);

/*
	digitalWrite(PIN_RESET, HIGH);
	delay(3000);
	digitalWrite(PIN_RESET, LOW);*/
	delay(10000);//10 segundos para dejarlo arrancar
	ActivaModulo();
}


bool ExisteContactoSIM(uint8_t entrySim) {
	char command[25];
	memset(command, 0, sizeof(command));
	sprintf(command, "AT+CPBR=%i", entrySim);
	SendCommand(command, command, sizeof(command));
	return (strncmp(command, "+CPBR", 5) == 0);
}


bool SIMEstaLista() {
	char respuesta[25];
	SendCommand("AT+CPIN?", respuesta, sizeof(respuesta));
	return (strncmp(respuesta, "+CPIN: READY", 12) == 0);
}

//Devuelve si el Modulo esta activo
//Si responde comando AT
bool EstaArrancado() {


	char respuesta[10];
	SendCommandTimeout("AT", respuesta, sizeof(respuesta), 0);
	return (strlen(respuesta) > 0);

}

//Comprueba si esta registrado
//AT+CREG?
//+CREG: 1,5
bool EstaRegistrado() {
	char respuesta[50];
	SendCommand("AT+CREG?", respuesta, sizeof(respuesta));
	//AT+CREG?
	//+CREG: 1,5
	return (strncmp(respuesta, "+CREG", 5) == 0 && (respuesta[9] == '5' || respuesta[9] == '1'));
}



bool Llamar(callType_t tipo,const char *telefono)
{
	bool result = true;
			char buffer[25];

				if (EstaRegistrado()) {
					//--LLamaada
					if (tipo==SIM)
						//Desde SIM
						sprintf(buffer, "ATD>\"SM\"%s;", telefono);
					else
						//Desde tfno
						sprintf(buffer, "ATD\"%s\";", telefono);

					SendCommandAsync(buffer);

					//--Fin llamada
					delay(20000);

					SendCommandAsync("ATH");


				} else {
					result = false;
				}




			return result;
}



bool LlamarDesdeSim(uint8_t entrySim)
{
	char entry[4];
	memset(entry, 0, sizeof(entry));
	sprintf(entry, "%i", entrySim);
	return Llamar(SIM,entry);
}

//-2 No se pudo registrar
//-1 Modulo GSM no se pudo activar
//0 -si todo fue ok
bool LlamarTelefono(const char *telefono) {
	return Llamar(Phone,telefono);
}



void LogATCommands (char tipo,const char *comando)
{

	char response[50];
	memset (response,0,sizeof(response));
	sprintf(response,"%c->%s",tipo,comando);
	mySerial.println(response);
	//Serial.println(response);

}
void LogResponse (const  char*comando)
{
	LogATCommands('R',comando);
}

void LogSend(const char *comando)
{
	LogATCommands('S',comando);
}

void LogError(const char *comando)
{
	LogATCommands('E',comando);
}
void LogInfo(const char *comando)
{
	LogATCommands('I',comando);
}

bool SendCommandTimeout(const char * comando, char * respuesta, int size, uint32 wait) {

	LogSend(comando);

	//Antes de enviar limpiamos lo que haya en el buffer
	Serial.flush();
	while(Serial.available())
		Serial.read();

	Serial.println(comando);


	if (wait != 0) {
		unsigned long t_inicial = millis();
		while ((millis() - t_inicial) < wait && Serial.available() == 0) {
		}
	}

	//Esperamos 100 ms para que llegue todo
	delay(250);

	int contador = ReadSerialData(respuesta, size);

	bool result= !(contador == size && Serial.available() > 0);



	return result;
}





int ReadSerialData(char *respuesta, int size) {
	int contador = 0;
	Serial.flush();

	if (size>0)
		memset(respuesta, 0, size);

	while (Serial.available()) {
		char readchar = (char) Serial.read();

		if (contador < (size-1) && readchar != 10 && readchar != 13) {
			respuesta[contador] = readchar;
			contador++;
		}
	}

	if (respuesta!=NULL)
		LogResponse(respuesta);

	return contador;
}

void SendCommandAsync(const char * comando)
{

	SendCommandTimeout(comando,NULL,0,0);


}

bool SendCommand(const char * comando, char * respuesta, int size)
{
	return SendCommandTimeout(comando,respuesta,size,5000);
}


void ApagaModulo() {
	//Apagamos modulo
	//Alarm.delay(1000);
	delay(5000);
	char reply[1];
	SendCommand("AT*PSCPOF", reply, sizeof(reply));

}




void DesconectaModulo() {
	//Apagamos modulo
	//Alarm.delay(1000);
	pinMode(PIN_RESET, OUTPUT);
	digitalWrite(PIN_RESET, HIGH);
	LogInfo("Apagando modulo!");

}


bool TieneLlamadas()
{
	char reply[10];
	memset (reply,0,sizeof(reply));
	SendCommand("AT+CPAS", reply, sizeof(reply));
	return ( (strncmp(reply, "+CPAS: 3", 8)==0) || (strncmp(reply, "+CPAS: 4", 8)==0));

}


//-2 Si se produjo un error con el pin
//-1 Si se produjo un error al arrancar el modulo
// 0 Si el modulo quedo activo y con el pin insertado
uint8_t ActivaModulo() {
	uint8_t result = 0;

	if (!EstaArrancado())
	{
		result=-1;
		SwitchModule();
	}

	if (EstaArrancado())
	{
		//Sin eco
		SendCommandAsync("ATE0");
		//Control hardware
		SendCommandAsync("AT&K3");

		result=0;
	}
	else
		result=-1;

	return result;
}
