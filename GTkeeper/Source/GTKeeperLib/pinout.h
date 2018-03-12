//Definicion de pines
#ifndef PINOUT_H_
#define PINOUT_H_


//Define de constantes  depuertos
#define PORTS_NUM 1//15 Numero max de puertos:)
#define PORTS_ABONO 2 //Numero de puertos de abono
#define PORT_MOTOR 40  //Numero donde se conecta el motor de bombeo


//LEDS 
#define LED_WORKING_PIN 13
#define LED_ERROR_MODULE_PIN 13
#define LED_ERROR_SIM_PIN 13
#define LED_ERROR_NETWORK_PIN 13
#define	LED_ERROR_MODULE_PIN 13 //LED INDICADOR DE ERRORES

//Pines para el Modulo  GSM 
#define GSM_ONOFF_PIN 9
#define GSM_READ_PIN
#define GSM_WRITE_PIN

//Pines de sectores

#define SECTORES

//El pin out del teclado las 4 primeros pines empezando por el * son las filas y los otras 4 las columnas
//http://www.learningaboutelectronics.com/Articles/Arduino-keypad-circuit.php

#define KEYBOARD_ROW1_PIN 28
#define KEYBOARD_ROW2_PIN 29
#define KEYBOARD_ROW3_PIN 26
#define KEYBOARD_ROW4_PIN 27

#define KEYBOARD_COL1_PIN 24
#define KEYBOARD_COL2_PIN 25
#define KEYBOARD_COL3_PIN 22
#define KEYBOARD_COL4_PIN 23

//Pin I2C


//PIN COMUN
#define PORT_COMUN_PIN 4

//PORT_PUENTE: Es el pin donde va al rele que maneja el puente de la valvula
//Si el rele esta abierto(sin encender) le envia -(negativo) a la salida de la valvula , en el caso que se active el rele de la valvula
//Cuando esta cerrado (encendido) le envia + (positivo)  a la salida de la valvula , en el caso que se active el rele de la valvula
#define PORT_PUENTEH1_PIN 2
#define PORT_PUENTEH2_PIN 3

//Pin SCLA

//Pin Abono
#define  PORT_ABONO1_PIN 39 
#define  PORT_ABONO2_PIN 40
//Pin Motor

//Pin sectores
#define PORT_SECTOR1_PIN 5
//#define PORT_SECTOR2_PIN 6
//#define PORT_SECTOR3_PIN 10


//Pin sensores


#endif /* PINOUT_H_ */
