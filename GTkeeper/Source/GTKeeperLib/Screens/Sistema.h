/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef SISTEMASSCREEN_H_
#define SISTEMASSCREEN_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#include "..\gtkeeper.h"
#include <ScreenBase.h>
#include <ScreenManager.h>
#include "ParametrosMenu.h"



class SistemasScreen: public ScreenBase {
public:
	SistemasScreen();

	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
	virtual bool ShowBlink() { return false;}
private:
	void setData();


};

extern SistemasScreen sistemasScreen;
#endif
#endif
