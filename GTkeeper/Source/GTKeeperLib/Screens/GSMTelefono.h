/*
 * LanzaProgramas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO

#ifndef GSMTELEFONO_H_
#define GSMTELEFONO_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "..\gtkeeper.h"

#include "GSMMenu.h"

class GsmTelefonoScreen:ScreenBase {
public:
	GsmTelefonoScreen();

	virtual void OnDrawFields();
	virtual void  OnEnter ();
	void  OnClickButton(uint8_t field);
	bool SetKey(unsigned char c);
};

extern GsmTelefonoScreen gsmTelefonoScreen;

#endif /* LANZAPROGRAMAS_H_ */


#endif

