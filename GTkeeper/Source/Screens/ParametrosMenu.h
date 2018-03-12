/*
 * ParametrosMenu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef PARAMETROSMENU_H_
#define PARAMETROSMENU_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>

#include "Programas.h"
#include "Sistema.h"
#include "FechaHora.h"
#include "GSMMenu.h"
#include "..\textos.h"




class ParametrosMenu:ScreenBase {
public:
	ParametrosMenu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);

};


extern ParametrosMenu parametrosMenuScreen;

#endif /* PARAMETROSMENU_H_ */
#endif
