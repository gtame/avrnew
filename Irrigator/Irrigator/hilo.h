/*
 * hilo.h
 *
 *  Created on: 15/10/2015
 *      Author: Admin
 */
/*
 * hilo.h
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */


#ifndef HILO_H_
#define HILO_H_

#include <string.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

typedef unsigned int uint32;

extern SoftwareSerial mySerial; // RX, TX

#define PIN_RESET A3


typedef enum
{
	SIM,
	Phone
} callType_t;

int ReadSerialData(char *respuesta, int size);
bool SendCommandTimeout(const char * comando, char * respuesta, int size, uint32 wait);
bool SendCommand(const char * comando, char * respuesta, int size);
void SendCommandAsync(const char * comando);

void SwitchModule();
bool EstaArrancado();
void DesconectaModulo();
void ApagaModulo();
uint8_t ActivaModulo();
bool EstaRegistrado();
bool SIMEstaLista();
bool ExisteContactoSIM(uint8_t entrySim);
//uint8_t Llamar(callType_t tipo,const char *telefono);

bool LlamarTelefono(const char *telefono);
bool LlamarDesdeSim(uint8_t entrySim);
bool Llamar(callType_t tipo,const char *telefono);
bool TieneLlamadas();

/**
 *
 *  Log functions
 *  Usando SoftwareSerial
 */
void LogAtCommands (char tipo,const char *comando);
void LogResponse (const  char*comando);
void LogSend(const char *comando);
void LogError(const char *comando);
void LogInfo(const char *comando);


#endif /* HILO_H_ */
