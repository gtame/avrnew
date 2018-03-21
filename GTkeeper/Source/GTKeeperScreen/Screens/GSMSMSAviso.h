/*
 * GSMSMSAviso.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#ifdef PANTALLA_TECLADO


#ifndef GSMSMSAVISO_H_
#define GSMSMSAVISO_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include <gtkeeper.h>

#include "GSMMenu.h"

class GsmSmsAvisoScreen:ScreenBase {
public:
	GsmSmsAvisoScreen();

	virtual void OnDrawFields();
	virtual void  OnEnter ();
	void  OnClickButton(uint8_t field);
	bool SetKey(unsigned char c);
};

extern GsmSmsAvisoScreen gsmSmsAvisoScreen;

#endif /* LANZAPROGRAMAS_H_ */

#endif


