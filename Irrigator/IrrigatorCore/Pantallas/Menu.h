/*
 * Menu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */



#ifndef MENU_H_
#define MENU_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Pantalla.h>
#include <Frame.h>


#define TEXT_PROGRAMAS "1-Programas"
#define TEXT_MANUAL "2-Manual"
#define TEXT_PARAMETROS "3-Parametros"
#define TEXT_CONSULTAS "4-Consulta"

class Menu:ScreenBase {
public:
	Menu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
};

#endif /* MENU_H_ */
