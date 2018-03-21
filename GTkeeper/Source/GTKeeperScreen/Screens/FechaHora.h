/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */



#ifdef PANTALLA_TECLADO


#ifndef _FECHAHORA_H_
#define _FECHAHORA_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Time.h>
#include <gtkeeper.h>
#include <ScreenBase.h>
#include <ScreenManager.h>
#include "ProgramasMenu.h"
 
class FechaHoraScreen: public ScreenBase {
public:
	FechaHoraScreen();

	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);

	virtual bool ShowBlink() { return false;}
private:
	void setFecha();
	time_t fecha;
	TimeElements fechaElements;
};

extern FechaHoraScreen fechahoraScreen;
#endif


#endif
