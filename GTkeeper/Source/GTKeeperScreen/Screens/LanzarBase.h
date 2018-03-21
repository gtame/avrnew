/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#ifdef PANTALLA_TECLADO


#ifndef _LANZARBASESCREEN_H_
#define _LANZARBASESCREEN_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <ScreenBase.h>
#include <ScreenManager.h>


class LanzarBaseScreen: public ScreenBase {
public:
	LanzarBaseScreen();

	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
	virtual void  OnSelection ();
	virtual uint8_t MaxValue () {return 0;}
	virtual uint8_t MinValue () {return 0;}
	virtual uint8_t FieldLen() {return 2;}
	virtual void  OnGotoMenu();
	virtual bool ShowBlink() { return false;}
protected:
	void setValor(int8_t valor);
	uint8_t currentvalue;

};


#endif

#endif
