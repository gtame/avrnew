/*
 * main.c
 *
 *  Created on: 21/09/2012
 *      Author: Administrador
 */
/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/

#include "main.h"
#include <WProgram.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <string.h>

#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <TimeAlarms.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "constants.h"
#include <Keypad.h>

#include <Pantalla.h>
#include "Pantallas/Menu.h"
#include "Pantallas/Programas.h"
#include "Pantallas/Manual.h"
#include "Pantallas/Consulta.h"
#include "Pantallas/Parametros.h"
//--variables


//Teclado
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = { { 'D', '#', '0', '*' }, { 'C', '9', '8', '7' }, {
		'B', '6', '5', '4' }, { 'A', '3', '2', '1' } };
byte rowPins[ROWS] = { 6, 5, 4, 3 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 10, 9, 8, 7 }; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/*-----( Declare objects )-----*/

Programa programas[PROGRAMAS_SIZE];
Riego riegos[PROGRAMAS_SIZE];
Configuracion config;
Frame** pantallas[PANTALLAS_NUM];
bool IsLockedATCommand = false;

bool inactive = false;
Menu pMenu;
Programas pProgramas;
Manual pManual;
Consulta pConsulta;
Parametros pParametros;

uint8_t programaActual = 0;
char buffer[100];
int onModulePin = 31; // the pin to switch on the module (without press on button)
int int_counter = 0;

LiquidCrystal_I2C lcd(0x20, 20, 4);

/*-----( Declare Variables )-----*/

//#define DS1307_ADDRESS 0x68

//Funcion necesaria para el compilador

extern "C" void __cxa_pure_virtual() {

}

void setPantallaActual(uint8_t pantalla) {

	//delay no se puede usar !!!
#ifndef PROTEUS
	delayMicroseconds(100);
#endif

	pActual.frame = ((Frame *) pantallas[pantalla]);

	//delay no se puede usar !!!
#ifndef PROTEUS
	delayMicroseconds(100);
#endif

	pActual.OnEnter();

}

/*
 void pulseDTR()
 {
 digitalWrite(onModulePin,HIGH);
 //Alarm.delay(3000);
 digitalWrite(onModulePin,LOW);
 }
 */

void switchModule() {
	digitalWrite(onModulePin, HIGH);
	Alarm.delay(3000);
	digitalWrite(onModulePin, LOW);

	digitalWrite(onModulePin, HIGH);
	digitalWrite(onModulePin, LOW);
	//Le damos 8 segundos para que se arranque
	Alarm.delay(8000);
}

int main() {

	//Necesario para la inicializacion de arduino
	init();

	//Setup
	setup();

	while (true)
		loop();

}

void Debug(const char *mensaje) {
	Serial.println(mensaje);
}

void SendCommand(const char* command) {
	Serial1.println(command);
	Serial.println(command);
	Alarm.delay(500);
}

void SendString(const char* command) {
	for (int i = 0; i < strlen(command); i++) {
		Serial1.print(command[i]);
	}
}

void SendChar(const char caracter) {
	Serial1.print(caracter);
}

int ReadSerialData(char *respuesta, int size) {
	int contador = 0;
	memset(respuesta, 0, size);
	while (Serial1.available() && contador < size) {
		char readchar = (char) Serial1.read();
		if (readchar != 10 && readchar != 13) {
			respuesta[contador] = readchar;
			contador++;
		}
	}
	return contador;
}

bool SendSerialWithReplyAndWait(const char * comando, char * respuesta,
		int size, int wait, bool IsCommand, bool IsLoop) {

	Serial1.flush();

	if (IsCommand)
		SendCommand(comando);
	else
		SendString(comando);

	if (wait != 0) {
		unsigned long t_inicial = millis();
		while ((millis() - t_inicial) < wait && Serial1.available() == 0) {
			if (IsLoop)
				loop();
		}
	}

	//Esperamos 100 ms para que llegue todo
	delay(200);
	int contador = ReadSerialData(respuesta, size);
	return !(contador == size && Serial1.available() > 0);
}

bool SendCommandWithReplyAndWait(const char * comando, char * respuesta,
		int size, int wait) {

	return SendSerialWithReplyAndWait(comando, respuesta, size, wait, true,
			false);
	//AT+CREG?
	//+CREG: 1,5
}

bool SendCommandWithReplyAndLoop(const char * comando, char * respuesta,
		int size, int wait) {

	bool result = SendSerialWithReplyAndWait(comando, respuesta, size, wait,
			true, true);
	return result;
}

bool SendStringWithReplyAndWait(const char * comando, char * respuesta,
		int size, int wait) {

	return SendSerialWithReplyAndWait(comando, respuesta, size, wait, false,
			false);
	//AT+CREG?
	//+CREG: 1,5
}

bool SendStringWithReplyAndLoop(const char * comando, char * respuesta,
		int size, int wait) {
	bool result = SendSerialWithReplyAndWait(comando, respuesta, size, wait,
			false, true);
	return result;
	//AT+CREG?
	//+CREG: 1,5
}

bool SendCommandWithReply(const char * comando, char * respuesta, int size) {
	return SendCommandWithReplyAndWait(comando, respuesta, size, 0);

	//AT+CREG?
	//+CREG: 1,5
}
bool SendStringWithReply(const char * comando, char * respuesta, int size) {
	return SendStringWithReplyAndWait(comando, respuesta, size, 0);
}

//Devuelve si el Modulo esta activo
//Si responde comando AT
bool EstaArrancado() {

	Serial1.flush();
	char respuesta[10];
	SendCommandWithReplyAndWait("AT", respuesta, sizeof(respuesta), 1000);
	return (strlen(respuesta) > 0);

}

bool NecesitaPin() {
	char respuesta[50];
	bool result = false;
	SendCommandWithReplyAndWait("AT+CPIN?", respuesta, sizeof(respuesta), 200);
	Debug("Respuesta pin");
	Debug(respuesta);
	result = !(strncmp(respuesta, "+CPIN: READY", 12) == 0);

	return result;
}

bool PutPinCode(const char * pinCode) {
	char pincommand[25];
	memset(pincommand, 0, sizeof(pincommand));
	sprintf(pincommand, "AT+CPIN=\"%s\"", pinCode);
	SendCommand(pincommand);
	return !(NecesitaPin());
}

//-2 Si se produjo un error con el pin
//-1 Si se produjo un error al arrancar el modulo
// 0 Si el modulo quedo activo y con el pin insertado
char ActivaModulo() {
	char result = 0;
	bool arrancado = true;
	if (!EstaArrancado()) {
		switchModule();

		if (EstaArrancado()) {
			result = 0;
			//Sin eco
			SendCommand("ATE0");
			//Control hardware
			SendCommand("AT&K3");

		} else {
			arrancado = false;
		}
	} else {
		//Sin eco
		SendCommand("ATE0");
		//Control hardware
		SendCommand("AT&K3");
	}

	if (arrancado) {

		//numero pin
		if (NecesitaPin()) {
			if (!PutPinCode(PIN_CODE)) {
				result = -2;
				Debug("No se a podido poner el codigo pin");
			}

		}
	} else {
		result = -1;
		Debug("No se puede arrancar el modulo GSM");
	}

	return result;
}

void ApagaModulo() {
	//Apagamos modulo
	//Alarm.delay(1000);
	char reply[1];
	SendCommandWithReply("AT*PSCPOF", reply, sizeof(reply));

}

//Comprueba si esta registrado

//AT+CREG?
//+CREG: 1,5
bool EstaRegistrado() {
	char respuesta[50];
	SendCommandWithReply("AT+CREG?", respuesta, sizeof(respuesta));
	//AT+CREG?
	//+CREG: 1,5
	return (strncmp(respuesta, "+CREG", 5) == 0 && respuesta[9] == '5');
}

bool EstaRegistradoCheck(unsigned char intentos) {
	unsigned char contador = 0;
	bool registrado = EstaRegistrado();
	while (contador < intentos && !registrado) {
		registrado = EstaRegistrado();
		Alarm.delay(1000);
		contador++;
	}
	return registrado;
}

/*
 -1 Error...
 0...1   0-Nada
 1...7:  1-Mala
 7...13: 2-Medio Bajo
 14..21: 3-Medio
 22..25: 4-Medio Alto
 26..31: 5-Buena
 99  99-Desconocido
 */
int Cobertura() {
	uint8_t rango = 0;
	char respuesta[50];
	SendCommandWithReply("AT+CSQ", respuesta, sizeof(respuesta));
	if (strncmp(respuesta, "+CSQ:", 5) == 0) {
		char * t1 = strtok(respuesta + 5, ",");
		/*if (t1[0]==' ')
		 t1[0]='0';*/
		/*
		 *
		 0...10 : Mala
		 11..22: Normal
		 23..31: Muy buena
		 *
		 *
		 * */
		Serial.print(t1);
		int signal = atoi(t1);
		if (signal <= 0)
			rango = 0;
		else if (signal >= 1 && signal <= 7)
			rango = 1;
		else if (signal >= 8 && signal <= 12)
			rango = 2;
		else if (signal >= 13 && signal <= 21)
			rango = 3;
		else if (signal >= 22 && signal <= 29)
			rango = 4;
		else if (signal >= 30 && signal < 99)
			rango = 5;
		else if (signal >= 99)
			rango = 99;

		return rango;

	} else
		return -1;

}

void Sms(const char* numero, const char* message) {
	char result = 0;
	bool encendido = EstaArrancado();
	char commands[50];
	if (ActivaModulo() == 0) {

		if (EstaRegistradoCheck(10)) {
			SendCommand("AT+CMGF=1"); //enviar modo texto
			Alarm.delay(1000);
			memset(commands, 0, sizeof(commands));
			sprintf(commands, "AT+CMGS=\"%s\"", numero);
			SendCommand(commands);
			Alarm.delay(1000);

			SendCommand(message); // the SMS body


			//Serial1.print(0x1A,BYTE);                // end of message command 1A (hex)


			memset(commands, 0, sizeof(commands));
			commands[0] = 0x1A;
			SendStringWithReplyAndWait(commands, commands, sizeof(commands),
					2000);

			Alarm.delay(500);
			Debug("Respuesta SMS");
			Debug(commands);
		} else {
			Debug("No se puede registrar el modulo");
			result = -2;
		}

		//Apagamos modulo si este no estaba encendido anteriormente
		if (!encendido)
			ApagaModulo();
	} else
		result = -1;

}

//-1 si archivo no existe
int FlashSizeArchivo(const char*archivo) {
	//Si no existe fichero
	//	AT+KFSFILE=3,"/data/benvido2.msg"
	//	+CME ERROR: 3
	//Si existe fichero
	//	AT+KFSFILE=3,"/data/benvido.msg"
	//	+KFSFILE: 0

	char respuesta[50];
	int result = -1;
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "AT+KFSFILE=3,\"/data/%s\"", archivo);
	SendCommandWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 500);
	if (strncmp(respuesta, "+KFSFILE:", 9) == 0)
		result = atoi(respuesta + 10);
	return result;
}

bool FlashReadFichero(const char*archivo, char * resultado) {
	int size = FlashSizeArchivo(archivo);
	if (size > 0) {
		char respuesta[50];
		unsigned int counter = 0;
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "AT+KFSFILE=1,\"/data/%s\",%i", archivo, size);
		SendCommand(buffer);
		memset(buffer, 0, sizeof(buffer));
		Alarm.delay(1000);

		//Cargamos el connect
		while (Serial1.available() && counter <= 10) {
			unsigned char charReaded = (unsigned char) Serial1.read();
			buffer[counter] = charReaded;
			counter++;

		}

		//Enviamos el contenido
		if (strncmp(buffer + 2, "CONNECT", 7) == 0) {
			counter = 0;
			while (Serial1.available() && size > counter) {
				unsigned char charReaded = (unsigned char) Serial1.read();
				resultado[counter] = charReaded;
				Serial.print(charReaded);
				counter++;
			}

			if (size == counter) {
				Alarm.delay(500); //eSperamos al OK..
				memset(respuesta, 0, sizeof(respuesta));
				counter = 0;
				while (Serial1.available() && sizeof(respuesta) > counter) {
					respuesta[counter] = Serial1.read();
					counter++;
				}

				if (strncmp(respuesta + 2, "OK", 2) != 0) {
					Debug("Error: Se esparaba OK, La respuesta ha sido..");
					Debug(respuesta);
					return false;
				}

			} else {
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "Leidos->%i", counter);
				Debug("No se leyeron todos lo datos esperados");
				Debug(resultado);
				Debug(buffer);
				return false;
			}
		} else {
			Debug("Error: Se esparaba CONNECT");
			Debug(buffer);
			return false;
		}

	} else {
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "No existe el fichero o esta vacio(Tamaño:%i)", size);
		Debug(buffer);
		return false;
	}

	//OK?
	return EstaArrancado();

}

bool FlashGuardaFichero(const char * archivo, const char *datos, const int size) {
	//Guardamos los programas en la flash
	char respuesta[50];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "AT+KFSFILE=0,\"/data/%s\",%i", archivo, size);
	SendCommandWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 3000);
	//Enviamos el contenido
	if (strncmp(respuesta, "CONNECT", 6) == 0) {
		Serial1.flush();
		int counter = 0;
		//hay que recorrer todo su tamaño
		for (int counter = 0; counter < size; counter++) {
			/*	if (datos[counter]==0)
			 SendChar('a');
			 else*/
			SendChar(datos[counter]);
		}

		Alarm.delay(500); //eSperamos al OK..
		memset(respuesta, 0, sizeof(respuesta));
		counter = 0;
		while (Serial1.available() && sizeof(respuesta) > counter) {
			char charReaded = Serial1.read();
			if (charReaded != 10 && charReaded != 13) {
				respuesta[counter] = charReaded;
				counter++;
			}
		}

		if (strncmp(respuesta, "OK", 2) != 0) {
			Debug("Error: Se esparaba OK, La respuesta ha sido..");
			Debug(respuesta);
			return false;
		}
	} else {
		Debug("Error: Se esparaba CONNECT");
		Debug(respuesta);
		return false;
	}

	//OK?
	return EstaArrancado();
}

bool FlashGuardaSettings() {
	//Reemplaza
	return FlashGuardaFichero("settings.config", config.FechaActualizacion,
			sizeof(Configuracion));
	/*
	 memset(&config,0,sizeof(Configuracion));
	 sprintf(config.FechaActualizacion,"20130106110400");
	 //-- GPRS Settings
	 sprintf(config.ServerAPN,"gprs-service.com");
	 sprintf(config.Login,"");
	 sprintf(config.Password,"");
	 sprintf(config.IPAddress,"");
	 sprintf(config.Dns1,"");
	 sprintf(config.Dns2,"");
	 //--FTP Settings--
	 sprintf(config.FtpServer,"gtame.no-ip.org");
	 sprintf(config.FtpUser,"anonymous");
	 sprintf(config.FtpPassword,"");
	 sprintf(config.FtpPuerto,"40000");
	 //Phone Settings
	 sprintf(config.Pin,"2259");
	 sprintf(config.Telefono1,NUM_TELEFONO);
	 sprintf(config.Telefono2,"976775432");

	 Debug("Fecha->");
	 Debug(config.FechaActualizacion);

	 if (FlashGuardaSettings())
	 {
	 Debug ("Fichero guardado");

	 memset(&config,0,sizeof(Configuracion));
	 if (FlashReadFichero("settings.config",config.FechaActualizacion))
	 {

	 Debug("Fichero leido correctamente");
	 Debug(config.FechaActualizacion);
	 Debug(config.FtpServer);
	 }
	 }*/

}

void PutConexionConfig() {
	//Chequeamos si ya fue configurada la conexion
	memset(buffer, 0, sizeof(buffer));
	SendCommandWithReply("AT+KFTPCFG?", buffer, sizeof(buffer));
	if (strlen(buffer) > 5) {
		//Configuramos la conexion y el ftp... ;>
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "AT+KCNXCFG=0,\"GPRS\",\"%s\",\"\",\"\"",
				config.ServerAPN);
		SendCommand(buffer);

		SendCommand("AT+KCNXTIMER=0,15,4,60");
		SendCommand("AT+KCNXPROFILE=0");
		SendCommand("AT+CGATT=1");

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "AT+KFTPCFG=0,\"%s\",\"%s\",\"%s\",%s,0",
				config.FtpServer, config.FtpUser, config.FtpPassword,
				config.FtpPuerto);
		SendCommand(buffer);

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "AT+KPATTERN=\"%c\"", 0x1A);
		SendCommand(buffer);
	}
}

void ExitFtpMode() {
	pActual.needrefresh = true;
	IsLockedATCommand = false;
}

bool SetFtpMode() {
	IsLockedATCommand = true;

	if (!EstaArrancado()) {
		if (ActivaModulo() == 0)
			PutConexionConfig();

	}

	if (EstaArrancado())
		return true;
	else {
		ExitFtpMode();
		return false;
	}
}

bool FtpEnviamosDatos(char * archivo, char *data) {
	//Guardamos los programas en la flash
	char respuesta[50];

	//Se puede enviar directamente con el comando un archivo que este en la flash, pero a de estar en el directorio raiz /
	//sprintf(buffer,"AT+KFTPSND=0,\"/data/%s\",\"/\",\"confirm-%s\",0",archivo,archivo);
	//	SendCommandWithReplyAndWait(buffer,respuesta,sizeof(respuesta),5000);
	//return (strncmp(respuesta,"OK",2)==0);


	if (SetFtpMode()) {

		sprintf(respuesta, "AT+KFTPSND=0,\"\",\"/\",\"%s\",0", archivo);

		SendCommandWithReplyAndLoop(respuesta, respuesta, sizeof(respuesta),
				20000);

		//Enviamos el contenido
		if (strncmp(respuesta, "CONNECT", 7) == 0) {
			//Enviamos los programas
			SendString(data);

			//Enviamos el caracter final de archivo -- EOF Pattern
			memset(respuesta, 0, sizeof(respuesta));
			sprintf(respuesta, "%c", 0x1A);
			SendCommandWithReplyAndLoop(respuesta, respuesta,
					sizeof(respuesta), 5000);

			//Comprobamos que la respuesta es ok
			if (strncmp(respuesta, "OK", 2) != 0) {
				Debug("Error: Se esparaba OK");
				Debug(respuesta);
				ExitFtpMode();
				return false;
			}

		} else {
			Debug("Error: Se esparaba CONNECT");
			Debug(respuesta);
			ExitFtpMode();
			return false;
		}

		ExitFtpMode();
	} else
		return false;

	//OK?
	return true;
}

bool FtpEnviamosEstados() {

	memset(buffer, 0, sizeof(buffer));
	//Ponemos el pinout de los reles de los sectores como salida
	for (uint8_t i = 0; i < SECTORES_COUNT; i++)
		buffer[i] = (digitalRead(PIN_INIT_SECTORES + i) == HIGH ? '1' : '0');

	char filename[25];
	sprintf(filename, "st_%lu", now());
	return FtpEnviamosDatos(filename, buffer);

	/*
	 //Guardamos los programas en la flash
	 char respuesta[50];
	 memset(buffer, 0, sizeof(buffer));

	 SetFtpMode();

	 sprintf(buffer, "AT+KFTPSND=0,\"\",\"/\",\"st_%lu\",0", now());
	 //SendCommandWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 10000);

	 SendCommandWithReplyAndLoop(buffer, respuesta, sizeof(respuesta), 20000);

	 Debug(respuesta);

	 //Enviamos el contenido
	 if (strncmp(respuesta, "CONNECT", 7) == 0) {
	 //Enviamos los estados de los sectores
	 memset(buffer, 0, sizeof(buffer));
	 //Ponemos el pinout de los reles de los sectores como salida
	 for(uint8_t i=0;i<SECTORES_COUNT;i++)
	 buffer[i]=(digitalRead(PIN_INIT_SECTORES+i)==HIGH?'1':'0');

	 SendString(buffer);

	 //Enviamos el caracter final de archivo -- EOF Pattern
	 memset(buffer, 0, sizeof(buffer));
	 sprintf(buffer, "%c", 0x1A);
	 SendStringWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 5000);

	 //Comprobamos que la respuesta es ok
	 if (strncmp(respuesta, "OK", 2) != 0) {
	 Debug("Error: Se esparaba OK");
	 Debug(respuesta);
	 ExitFtpMode();
	 return false;
	 }
	 } else {
	 Debug("Error: Se esparaba CONNECT");
	 Debug(respuesta);
	 ExitFtpMode();
	 return false;
	 }

	 ExitFtpMode();

	 //OK?
	 return EstaArrancado();
	 */
}

bool FtpEnviamosProgramas() {

	char filename[25];
	sprintf(filename, "pr_%lu", now());
	return FtpEnviamosDatos(filename, programas[0].Sector);

	/*
	 //Guardamos los programas en la flash
	 char respuesta[50];
	 memset(buffer, 0, sizeof(buffer));
	 //Se puede enviar directamente con el comando un archivo que este en la flash, pero a de estar en el directorio raiz /
	 //sprintf(buffer,"AT+KFTPSND=0,\"/data/%s\",\"/\",\"confirm-%s\",0",archivo,archivo);
	 //	SendCommandWithReplyAndWait(buffer,respuesta,sizeof(respuesta),5000);
	 //return (strncmp(respuesta,"OK",2)==0);


	 SetFtpMode();

	 sprintf(buffer, "AT+KFTPSND=0,\"\",\"/\",\"confirm-%s\",0", archivo);

	 SendCommandWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 10000);



	 //Enviamos el contenido
	 if (strncmp(respuesta, "CONNECT", 7) == 0) {
	 //Enviamos los programas
	 SendString(programas[0].Sector);

	 //Enviamos el caracter final de archivo -- EOF Pattern
	 memset(buffer, 0, sizeof(buffer));
	 sprintf(buffer, "%c", 0x1A);
	 SendStringWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 5000);



	 //Comprobamos que la respuesta es ok
	 if (strncmp(respuesta, "OK", 2) != 0) {
	 Debug("Error: Se esparaba OK");
	 Debug(respuesta);
	 return false;
	 }
	 } else {
	 Debug("Error: Se esparaba CONNECT");
	 Debug(respuesta);
	 return false;
	 }

	 ExitFtpMode();
	 //OK?
	 return EstaArrancado();*/
}

//Funcion para enviar por SMS la configuracion de programas
void SmsProgramas() {
	char dias_select[8];
	char sms_text[MAX_CHARACTERS_SMS];
	uint8_t numchar = 0;

	memset(sms_text, 0, sizeof(sms_text));

	if (strlen(programas[0].Sector) == 0) {
		sprintf(sms_text, "No hay ningún riego programado");
	} else {
		memset(dias_select, 0, sizeof(dias_select));

		sprintf(sms_text, "Riegos programados\n");
		numchar = strlen(sms_text);

		for (uint8_t contador = 0; contador < PROGRAMAS_SIZE; contador++) {
			if (strlen(programas[contador].Sector) > 0) {
				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].Sector, 2);
				uint8_t sector = atoi(buffer);

				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].HoraRiego, 2);
				uint8_t horas = atoi(buffer);

				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].MinutoRiego, 2);
				uint8_t minutos = atoi(buffer);

				//Miramos en programas
				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].HoraInicio, 2);
				uint8_t hora_inicio = atoi(buffer);

				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].MinutoInicio, 2);
				uint8_t minuto_inicio = atoi(buffer);

				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, programas[contador].DiasSemana, 3);
				DiasSemana dias = (DiasSemana) atoi(buffer);

				//Tenemos que ver para que dias de la semana esta configurado que salte. ;>
				dias_select[0] = ((dias & L) ? 'L' : '_');
				dias_select[1] = ((dias & M) ? 'M' : '_');
				dias_select[2] = ((dias & X) ? 'X' : '_');
				dias_select[3] = ((dias & J) ? 'J' : '_');
				dias_select[4] = ((dias & V) ? 'V' : '_');
				dias_select[5] = ((dias & S) ? 'S' : '_');
				dias_select[6] = ((dias & D) ? 'D' : '_');

				memset(buffer, 0, sizeof(buffer));
				if (hora_inicio == 88) {
					sprintf(buffer, "S %02i SA:%02i R %02i:%02i\n", sector,
							minuto_inicio, horas, minutos);

				} else {
					sprintf(buffer, "S %02i D %s H %02i:%02i R %02i:%02i\n",
							sector, dias_select, hora_inicio, minuto_inicio,
							horas, minutos);
				}

				//Tenemos que trocear el SMS si es demasiado largo!!
				if (numchar + strlen(buffer) > MAX_CHARACTERS_SMS) {
					//Enviamos el SMS

					Sms(NUM_TELEFONO, sms_text);
					memset(sms_text, 0, sizeof(sms_text));
					memcpy(sms_text, buffer, strlen(buffer));
					numchar = strlen(buffer);

				} else {

					memcpy(sms_text + strlen(sms_text), buffer, strlen(buffer));
					numchar += strlen(buffer);
				}
			}
		}
	}

	Sms(NUM_TELEFONO, sms_text);

}

//Carga los programas a partir de los datos que hay en el pto serie ;>
char CargaProgramasDesdeSerial() {
	uint8_t counter = 0;
	char programa = 0;

	//Esperamos para asegurarnos que recive todo
	Alarm.delay(2000);
	//LA info recibida sera de este modo:
	//	CONNECT
	//	0100112000130
	//	0200112000130
	//	OK


	memset(buffer, 0, sizeof(buffer));
	//Limpiamos el '\n\nCONNECT\n\n' para ello leemos
	if (Serial1.available()) {

		//Cargamos el connect
		while (Serial1.available() && counter <= 10) {
			buffer[counter] = (char) Serial1.read();
			counter++;
		}

		if (strncmp(buffer + 2, "CONNECT", 7) == 0) {

			Debug("Procesando registros");
			counter = 0;
			memset(buffer, 0, sizeof(buffer));
			//Procesamos archivo
			while (Serial1.available() && programa != -1) {

				char readChar = Serial1.read();

				if (readChar == (char) 0x1A) {
					if (counter > 0) {
						Debug("Formato de archivo incorrecto");
						programa = -1;
					}

					//Si es el final de fichero y programa actual es 0 es que el fichero esta vacio
					//limpiamos los programas
					if (programa == 0)
						memset(programas, 0, sizeof(Programa) * PROGRAMAS_SIZE);

					//Serial1.println("HOla");
					Debug("Fin de archivo");
					Serial1.flush();

				} else if (readChar == (char) 10) {

					if (counter != 13) {
						Debug(
								"Formato de archivo incorrecto, salto de linea inesperado");
						Debug(buffer);
						programa = -1;
					} else {

						//Validamos programa
						//¿??
						Debug("Leido programa correctametne:");
						Debug(buffer);

						//Si el programa es el 0 entonces limpiamos  el fichero de programas..
						//Limpiamos programas
						if (programa == 0)
							memset(programas, 0, sizeof(Programa)
									* PROGRAMAS_SIZE);

						//Procesamos programa..
						/* using memcpy to copy structure: */
						//memcpy (programas+(sizeof(Programa)*programa), buffer, sizeof(Programa) );
						memcpy((programas + programa), buffer, sizeof(Programa));
						programa++;
						//Limpiamos buffer
						memset(buffer, 0, sizeof(buffer));

						//Mostramos el programa leido.
						//						sprintf(buffer,"Programa--->%s",programas[programa-1].Sector);
						//						Debug(buffer);
						//						Debug(programas[0].Sector);
						//						memset(buffer,0,sizeof(buffer));

						//Validamos programa
						//Sector..
						//Horas, tiempos.. etc...

						//Reseteamos contador
						counter = 0;
					}

				} else if (counter > 13) {
					Debug(
							"Formato de archivo incorrecto, Se esperaba salto de linea");
					programa = -1;
				} else {
					buffer[counter] = readChar;
					counter++;
				}

			}
		} else {
			programa = -1;
			Debug("No se ha recibido el token CONNECT");
		}
	} else {
		programa = -1;
		Debug("Longitud recibida insuficiente");
	}

#ifdef SEND_SMS
	//Si se cargo correctamente
	if (programa==0)
	SmsProgramas();
#endif

	return programa;

}

bool FlashBorraArchivo(const char* archivo) {
	//	AT+KFSFILE=2,"/data/dbenvido.msg"
	//	+CME ERROR: 3
	//	AT+KFSFILE=2,"/data/benvido.msg"
	//	OK

	char respuesta[50];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "AT+KFSFILE=2,\"/data/%s\"", archivo);
	//SendCommandWithReply(buffer,respuesta,sizeof(respuesta));
	SendCommandWithReply(buffer, respuesta, sizeof(respuesta));
	return (strncmp(respuesta, "OK", 2) == 0);

}

bool FtpBorraArchivo(const char* archivo) {
	//	AT+KFSFILE=2,"/data/dbenvido.msg"
	//	+CME ERROR: 3
	//	AT+KFSFILE=2,"/data/benvido.msg"
	//	OK

	char respuesta[50];

	SetFtpMode();

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "AT+KFTPDEL=0,\"/\",\"%s\"", archivo);
	SendCommandWithReplyAndWait(buffer, respuesta, sizeof(respuesta), 5000);

	ExitFtpMode();

	return (strncmp(respuesta, "OK", 2) == 0);

}

//-2 No se pudo registrar
//-1 Modulo GSM no se pudo activar
//0 -si todo fue ok
void FtpObtenerArchivo(const char* archivo) {

	SetFtpMode();

	/*
	 //Chequeamos si ya fue configurada la conexion
	 memset(buffer, 0, sizeof(buffer));
	 SendCommandWithReply("AT+KFTPCFG?",buffer,sizeof(buffer));
	 if (strlen(buffer)>5)
	 {
	 //Configuramos la conexion y el ftp... ;>
	 memset(buffer, 0, sizeof(buffer));
	 sprintf(buffer, "AT+KCNXCFG=0,\"GPRS\",\"%s\",\"\",\"\"",config.ServerAPN);
	 SendCommand(buffer);

	 SendCommand("AT+KCNXTIMER=0,15,4,60");
	 SendCommand("AT+KCNXPROFILE=0");
	 SendCommand("AT+CGATT=1");

	 memset(buffer, 0, sizeof(buffer));
	 sprintf(buffer, "AT+KFTPCFG=0,\"%s\",\"%s\",\"%s\",%s,0", ftp, user,password, puerto);
	 SendCommand(buffer);

	 memset(buffer, 0, sizeof(buffer));
	 sprintf(buffer, "AT+KPATTERN=\"%c\"", 0x1A);
	 SendCommand(buffer);
	 }
	 */

	if (EstaRegistradoCheck(25)) {

		//Obtenarchivo();
		//Borrararchivo();
		//SubeConfirm();
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "AT+KFTPRCV=0,,\"\",\"%s\",0", archivo);
		SendCommand(buffer);
		Serial1.flush();

		int counter = 0;
		//Esperamos recibir respuesta...
		while (!Serial1.available() && counter < 600) {
			Alarm.delay(100);
			counter++;
		}

		if (!Serial1.available())
			Debug("Timeout conectando al ftp");
		else {

			Debug("Respondio el ftp");

			//Nos aseguramos que recibimos todo
			Alarm.delay(5000);

			//Si la carga fue OK
			if (CargaProgramasDesdeSerial() != -1) {
				//Borramos el archivo del ftp
				if (FtpBorraArchivo(archivo)) {
					//Guarda fichero en flash
					if (FlashGuardaFichero(archivo, programas[0].Sector,
							strlen(programas[0].Sector))) {
						//Enviamos el fichero de Cofirm al FTP
						Debug("Enviamos el confirm");
						if (!FtpEnviamosProgramas())
							Debug("No se pudo realizar el envio FTP");

					} else
						Debug("No se pudo almacenar el fichero en la flash");
				} else
					Debug("No se pudo eliminar el archivo");

				//strlen(programa)/sizeof(Programa)
				//Guardaremos el programa a la flash
				Debug("Ahora mostrarimos archivo...");
				//FlashReadFichero(archivo);
			}
			//Sms(NUM_TELEFONO,"Recibido archivo desde FTP");
		}

		//A cascala
		Debug("Cierre FTP");
		Serial1.flush();

		//Cerrar conexion,,,, paque??????
		//SendCommand("AT+KFTPCLOSE=0");
	} else
		Serial.println("No esta registrado a la red");

	//Si no estaba encendido al llamar al metodo lo apagamos


	ExitFtpMode();
}

//-2 No se pudo registrar
//-1 Modulo GSM no se pudo activar
//0 -si todo fue ok
char Llamar(const char *telefono) {
	char result = 0;
	bool encendido = EstaArrancado();
	if (ActivaModulo() == 0) {

		if (EstaRegistradoCheck(25)) {
			//--LLamaada
			sprintf(buffer, "ATD\"%s\";", telefono);
			SendCommand(buffer);

			//--Fin llamada
			Alarm.delay(15000);
			SendCommand("ATH");
		} else {
			result = -2;
			sprintf(buffer, "No se pudo realizar la llamada (Señal %d)",
					Cobertura());
			Debug(buffer);
		}

		//Apagamos modulo si el modulo no estaba encendido
		if (!encendido)
			ApagaModulo();
	} else
		result = -1;

	return result;
}

void LanzaRiego(uint8_t contador) {

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, programas[contador].Sector, 2);
	uint8_t sector = atoi(buffer);

	memset(riegos[contador].Sector, 0, sizeof(Riego));
	memcpy(riegos[contador].Sector, programas[contador].Sector, 2);
	sprintf(riegos[contador].FechaInicio, "%04i%02i%02i%02i%02i", year(),
			month(), day(), hour(), minute());

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, programas[contador].HoraRiego, 2);
	int horas = atoi(buffer);

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, programas[contador].MinutoRiego, 2);
	int minutos = atoi(buffer);

	time_t hasta = now() + minutesToTime_t(minutos) + hoursToTime_t(horas);
	hasta -= second(hasta);//Para que sea en el segundo 0


	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Parada programada ->  %04i/%02i/%02i/ %02i:%02i", year(
			hasta), month(hasta), day(hasta), hour(hasta), minute(hasta));
	Debug(buffer);

	if (hasta > riegos[contador].Hasta) {
		riegos[contador].Hasta = hasta;
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "[%i]->Hora parada %lu", contador,
				riegos[contador].Hasta);
		Debug(buffer);
	}

	//ACtivamos salida ;>
	digitalWrite(PIN_INIT_SECTORES + sector - 1, HIGH);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Regando-> Programa:%i Sector:%i Tiempo:%02i:%02i",
			contador, sector, horas, minutos);
	Debug(buffer);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Hasta - %04i/%02i/%02i %02i:%02i", year(hasta), month(
			hasta), day(hasta), hour(hasta), minute(hasta));
	Debug(buffer);

#ifdef SEND_SMS
	Sms(NUM_TELEFONO,buffer);
#endif
}

void PararRiego(uint8_t contador) {

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, riegos[contador].Sector, 2);
	Debug(riegos[contador].Sector);
	uint8_t sector = atoi(buffer);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Parando riego-> Programa:%i Sector:%i", contador, sector);
	Debug(buffer);

#ifdef SEND_SMS
	Sms(NUM_TELEFONO,buffer);
#endif

	//Desactiva la salida
	digitalWrite(PIN_INIT_SECTORES + sector - 1, LOW);

	//Miramos si hay algun programa que debemos lanzar. ;>
	for (uint8_t i = 0; i < PROGRAMAS_SIZE; i++) {
		if (i != contador) {
			if (strlen(programas[i].HoraInicio) > 0) {
				if (strncmp(programas[i].HoraInicio, "88", 2) == 0) {

					memset(buffer, 0, sizeof(buffer));
					memcpy(buffer, programas[i].MinutoInicio, 2);
					uint8_t programa = atoi(buffer);

					/*
					 memset(buffer,0,sizeof(buffer));
					 sprintf(buffer,"Iniciar programa %i al parar %i",programa,contador);
					 Debug(buffer);*/

					//EL contador empieza en 0
					if ((programa - 1) == contador) {
						LanzaRiego(i);
					}
				}
			}
		}
	}

	//Enviamos el fichero..

	//if (inactive)
	//{

	//guardar a fichero
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, riegos[contador].Sector, 14);
	sprintf(buffer + 14, "%04i%02i%02i%02i%02i", year(riegos[contador].Hasta),
			month(riegos[contador].Hasta), day(riegos[contador].Hasta), hour(
					riegos[contador].Hasta), minute(riegos[contador].Hasta));

	char filename[25];
	sprintf(filename, "rg_%lu", now());
	Debug(filename);
	Debug(buffer);
	FtpEnviamosDatos(filename, buffer);
	//}
	//else
	//{
	//Guardamos a flash?? Nein nein nein


	//}


	//Crea fichero de estadisticas
	riegos[contador].Hasta = 0;
}

void ChequearRiegos(time_t tiempo) {

	//Recorremos todos los programas miramos si debemos lanzar alguno
	//O parar alguno que este en ejecucion...

	int hora_actual = hour(tiempo);
	int minuto_actual = minute(tiempo);

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Chequeando riegos %02i:%02i", hora_actual, minuto_actual);
	Debug(buffer);

	for (uint8_t contador = 0; contador < PROGRAMAS_SIZE; contador++) {

		/*
		 memset(buffer, 0, sizeof(buffer));
		 sprintf(buffer, "PArar %lu %lu", riegos[contador].Hasta, now());
		 Debug(buffer);*/

		//Miramos en riegos
		if (riegos[contador].Hasta != 0 && riegos[contador].Hasta <= now())
			PararRiego(contador);
	}

	for (uint8_t contador = 0; contador < PROGRAMAS_SIZE; contador++) {

		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, &programas[contador], sizeof(Programa));

		if (strlen(buffer) > 0)
			Debug(buffer);

		if (strlen(buffer) == sizeof(Programa)) {
			//Miramos en programas
			memset(buffer, 0, sizeof(buffer));
			memcpy(buffer, programas[contador].HoraInicio, 2);
			int hora = atoi(buffer);

			memset(buffer, 0, sizeof(buffer));
			memcpy(buffer, programas[contador].MinutoInicio, 2);
			int minuto = atoi(buffer);

			memset(buffer, 0, sizeof(buffer));
			memcpy(buffer, programas[contador].DiasSemana, 3);
			DiasSemana dias = (DiasSemana) atoi(buffer);

			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "Programa. %i - hora %i minuto %i", contador, hora,
					minuto);
			Debug(buffer);

			if (hora_actual == hora && minuto_actual == minuto) {
				bool lanzar = false;

				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "Esta en hora de lanzar riego. %i", contador);
				Debug(buffer);

				//Tenemos que ver para que dias de la semana esta configurado que salte. ;>
				if ((dias & L) && weekday() == dowMonday)
					lanzar = true;

				if ((dias & M) && weekday() == dowTuesday)
					lanzar = true;

				if (dias & X && weekday() == dowWednesday)
					lanzar = true;

				if (dias & J && weekday() == dowThursday)
					lanzar = true;

				if (dias & V && weekday() == dowFriday)
					lanzar = true;

				if (dias & S && weekday() == dowSaturday)
					lanzar = true;

				if (dias & D && weekday() == dowSunday)
					lanzar = true;

				//Si la tenemos que lanzar
				if (lanzar)
					LanzaRiego(contador);

			}
		}

		//Miramos en riegos
		//if (riegos[contador].Hasta != 0 && riegos[contador].Hasta <= now())
		//	PararRiego(contador);

	}

}

void Inactivar() {
	lcd.noDisplay();
	lcd.noBacklight();
}

//Variable para controlar el ultimo minuto chequeado
time_t last_RiegosCheck = 0;
time_t last_FtpCheck = 0;
time_t last_Actividad = 0;

void FtpTask() {
	Debug("Chequeando ftp");
	//Si esta en modo inactivo...
	//Miramos si hay alguna programacion en el servidor FTP
	FtpObtenerArchivo(PROGRAMAS_FILE);
	last_FtpCheck = now();
}

char * FormatMensaje(const char* mensaje, const char* result) {
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, mensaje);
	strcat(buffer, T_POINTS);
	buffer[20] = '\0';
	strcpy(buffer + (strlen(buffer) - strlen(result)), result);
	return buffer;

}

void setup() {

	/*
	 TimeElements hora;
	 hora.Second=40;
	 hora.Minute=50;
	 hora.Hour=11;
	 hora.Day=30;
	 hora.Month=12;
	 hora.Year=2012-1970;


	 RTC.set(makeTime(hora));

	 */

	Serial1.begin(9600);
	Serial.begin(9600);

	//PIN de control del mnodulo GSM
	pinMode(onModulePin, OUTPUT);



	//Ponemos el pinout del teclado como entrada
	for (uint8_t i = 3; i < 11; i++) {
		digitalWrite(i, LOW);
		pinMode(i,INPUT);
	}

	//Ponemos el pinout de los reles de los sectores como salida
	for (uint8_t i = 0; i < SECTORES_COUNT; i++) {
		pinMode(PIN_INIT_SECTORES+i,OUTPUT);
		digitalWrite(PIN_INIT_SECTORES + i, LOW);

	}


	///Test de leds
/*
	//Ponemos el pinout de los reles de los sectores como salida
	for (uint8_t i = 0; i < SECTORES_COUNT; i++) {
		digitalWrite(PIN_INIT_SECTORES + i, HIGH);
		delay(1000);
	}
	for (uint8_t i = 0; i < SECTORES_COUNT; i++) {
		digitalWrite(PIN_INIT_SECTORES + i,LOW);
		delay(1000);
	}

*/
	//ApagaModulo();


	//time_t now=RTC.get();
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%04d%02d%02d%02d%02d", year(), month(), day(), hour(),
			minute());
	Debug("Hora RTC->");
	Debug(buffer);

	Wire.begin();
	setSyncProvider(RTC.get); // the function to get the time from the RTC
	Alarm.delay(500);

	//lcd inicializacion ;>
	lcd.init(); // initialize the lcd
	lcd.backlight();

	Alarm.delay(500);

	lcd.setCursor(0, 0);
	if (timeStatus() != timeSet) {
		lcd.print(FormatMensaje(T_RELOJ, T_ERROR_TEXT));
		Serial.println("Unable to sync with the RTC");
	} else {
		lcd.print(FormatMensaje(T_RELOJ, T_OK_TEXT));
		Serial.println("RTC has set the system time");
	}

	//setTime(8,29,0,1,1,10); // set time to 8:29:00am Jan 1 2010
	//setTime(hour(), minute(), second(), day(), month(), year());
	sprintf(buffer, "%04d%02d%02d%02d%02d", year(), month(), day(), hour(),
			minute());

	Serial.println(buffer);

	/*
	 // Print a message to the LCD.
	 lcd.setCursor(0, 0);
	 lcd.print("2-Line DISPLAY");
	 delay(1500);
	 lcd.setCursor(0, 1);
	 lcd.print("YourDuino: HI!");
	 */

	//#ifndef INICIO_RAPIDO


	programaActual = 0;

	lcd.setCursor(0, 1);

	if (ActivaModulo() == 0) {

		lcd.print(FormatMensaje(T_ACTIVAR_MOD, T_OK_TEXT));

		//#ifndef INICIO_RAPIDO


		lcd.setCursor(0, 2);
		//Cargamos archivo de configuracion
		memset(&config, 0, sizeof(Configuracion));
		if (!FlashReadFichero(CONFIG_FILE, config.FechaActualizacion)) {
			Debug("No se pudo cargar el archivo de configuracion");
			lcd.print(FormatMensaje(T_CONFIGURACION, T_ERROR_TEXT));

		} else {
			lcd.print(FormatMensaje(T_CONFIGURACION, T_OK_TEXT));
		}

		//#endif
		//Ponemos config
		PutConexionConfig();

		Debug("Cargando archivo Flash...");

		lcd.setCursor(0, 3);
		//Cargamos archivo de programas
		memset(programas, 0, sizeof(Programa) * PROGRAMAS_SIZE);
		if (FlashReadFichero(PROGRAMAS_FILE, programas[0].Sector)) {
#ifdef SEND_SMS
			SmsProgramas();
#endif
			lcd.print(FormatMensaje(T_PROGRAMAS, T_OK_TEXT));
		} else {
			Debug("No se pudo cargar el archivo de programas");
			lcd.print(FormatMensaje(T_PROGRAMAS, T_ERROR_TEXT));
			//Para que se vea el msj en el display
			Alarm.delay(2000);
		}

		//Llamaremos a FtpTask para comprobar si hay alguna nueva programacion de riego
		//FtpTask();


	} else {
		lcd.print(FormatMensaje(T_ACTIVAR_MOD, T_ERROR_TEXT));
		//Para que se vea el msj en el display
		Alarm.delay(2000);
	}

	//Para que se vea el msj en el display
	Alarm.delay(500);

	//  ObtenerArchivo("gtame.no-ip.org","anonymous","","40000",PROGRAMAS_FILE);

	//#ifndef INICIO_RAPIDO
	FtpTask();
	//#endif
	//Crearemos tb el trigger para ejecutar la peticion FTP cada X minutos en inactividad
	//Alarm.timerRepeat(60*5, FtpTask);// timer for every 5 minutos
	//Alarm.timerRepeat(20,ChequearRiegos);// timer for every 20 segundos


	//  DiasSemana dias=(DiasSemana) (L | M | X |J | V | S | D);

	//#endif


	//Inicializa la pantalla
	pActual.Initializate(&lcd, 20, 4);

	// Print our characters on the LCD
	//Screens del programa
	pantallas[MENU_PANTALLA] = (Frame**) &pMenu;
	pantallas[PROGRAMAS_PANTALLA] = (Frame**) &pProgramas;
	pantallas[MANUALMENU_PANTALLA] = (Frame**) &pManual;
	pantallas[CONSULTA_PANTALLA] = (Frame**) &pConsulta;
	pantallas[PARAMETROS_PANTALLA] = (Frame**) &pParametros;
	/*pantallas[FECHA_PANTALLA]=(Frame**)&pFecha;

	 pantallas[PARAMETROSMENU_PANTALLA]=(Frame**)&pParametrosMenu;
	 pantallas[TELEFONOMENU_PANTALLA]=(Frame**)&pTelefono;
	 pantallas[SIMPIN_PANTALLA]=(Frame**)&pPin;
	 pantallas[MENSAJE_PANTALLA]=(Frame**)&pMensaje;*/

	//Fijamos la pantalla actual..
	setPantallaActual(MENU_PANTALLA);
	//setPantallaActual(MENSAJE_PANTALLA);

	Alarm.delay(1500);

	last_Actividad = now();
}

void loop() //----( LOOP: RUNS CONSTANTLY )----
{

	time_t current_minute = now();

	if ((last_RiegosCheck + SECS_PER_MIN) <= current_minute) {

		//Si ha cambiado el minuto.. forzamos que se redibuje..
		pActual.needrefresh = true;

		Serial.println(pActual.fields[0].value);
		Serial.println(pActual.fields[1].value);

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%lu %lu", last_RiegosCheck, current_minute);
		Debug(buffer);

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "Hora actual %04i/%02i/%02i %02i:%02i", year(),
				month(), day(), hour(), minute());
		Debug(buffer);

		if (last_RiegosCheck == 0) {

			ChequearRiegos(current_minute);

		} else {
			while (last_RiegosCheck < current_minute) {
				last_RiegosCheck += SECS_PER_MIN;

				if (last_RiegosCheck > current_minute)
					last_RiegosCheck = current_minute;

				ChequearRiegos(last_RiegosCheck);
			}
		}

		TimeElements tiempo;
		breakTime(current_minute, tiempo); // break time_t into elements
		tiempo.Second = 0; //Quitamos los segundos para que sea hora exacta..
		last_RiegosCheck = makeTime(tiempo); // convert time elements into time_t
		//last_RiegosCheck = current_minute;

		if (inactive)
			FtpEnviamosEstados();

	}
	//Si pasaron más del tiempo estipulado de inactividad
	else if (current_minute > last_Actividad + 30) {
		inactive = true;
		//Pondremos flag de inactividad a true
		lcd.noDisplay();
		lcd.noBacklight();
	}
	//#ifndef INICIO_RAPIDO
	else if (((last_FtpCheck + (FREQUENCY_FTP * SECS_PER_MIN))
			<= (current_minute)) && inactive) {
		Debug("Comprobar nuevas programaciones...");
		FtpTask();
		last_FtpCheck = current_minute;
	}
	//Enviar programacion.. Cada 1 h para curarnos en salud de que esta bien subida la config. ;)
	//else if (last_Programas+)


	//#endif

	if (!inactive && IsLockedATCommand) {
		lcd.clear();
		lcd.setCursor(0, 1);
		lcd.print("Sincronizando...");
	} else {
		//Procesamos teclado
		char key = keypad.getKey();

		if (key != NO_KEY) {
			if (inactive) {
				lcd.display();
				lcd.backlight();

				inactive = false;
			} else
				pActual.SetKey(key);

			last_Actividad = now();
		}

		/*
		 sprintf(buffer, "%04d/%02d/%02d %02d:%02d", year(), month(), day(), hour(),	minute());
		 Debug(buffer);
		 */

		//Redibujamos pantalla
		if (!inactive && !IsLockedATCommand)
			pActual.Paint();
	}

}

/*Por hacer
 *Leer correctamente fichero desde Flash
 *Iniciar por sector no por programa
 En el loop que el bucle chequee todos los minutos que no han sido procesados e incluir cada 5 minutos un chequeo al ftp
 *Mandar sms o llamadas perdidas cuando se cargue configuracion o se  inicie el riego/pare riego.
 --FIN--
 */

