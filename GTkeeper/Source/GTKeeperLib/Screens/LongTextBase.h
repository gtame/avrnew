/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef LONGTEXTBASE_H_
#define LONGTEXTBASE_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <ScreenBase.h>
#include <ScreenManager.h>


class LongTextBaseScreen: public ScreenBase {
public:
	LongTextBaseScreen();

	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
	virtual void  OnSave ();
	virtual char* MaxValue () {return 0;}
	virtual void  OnGotoMenu();
	virtual bool ShowBlink() { return false;}
	virtual char* GetTitle();
	virtual char * GetValue() { return NULL;}


};


#endif
#endif
