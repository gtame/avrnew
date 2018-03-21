/*
 * Menu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef MENU_H_
#define MENU_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "Consulta.h"
#include "ProgramasMenu.h"
#include "ParametrosMenu.h"
#include "ManualMenu.h"
#include "EstadisticasMenu.h"


class Menu:ScreenBase {
public:
	Menu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
};


extern Menu menuScreen;

#endif /* MENU_H_ */
#endif
