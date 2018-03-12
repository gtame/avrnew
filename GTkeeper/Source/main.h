/*
 * main.h
 *
 *  Created on: 18 de abr. de 2017
 *      Author: Gabi
 */

#ifndef MAIN_H_
#define MAIN_H_


#ifdef PANTALLA_TECLADO
//LCD y pantallas

#include <Wire.h>
#include <ScreenManager.h>

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
//Screens
#include "Screens/Menu.h"
#include "Screens/FechaHora.h"
#include "Screens/GSMMenu.h"

#define SCREEN_ACTIVE() screenManager.IsActive()

#else

#define SCREEN_ACTIVE() false
//definida en ScreenManager.h
#define ELAPSED_SECONDS(X)  (now()-X)

#endif




//Funciones
void setLed(uint8_t led );


extern Stream *sserial;


#endif /* MAIN_H_ */
