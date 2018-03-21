/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef PROGRAMASCREEN_H_
#define PROGRAMASCREEN_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "ProgramasMenu.h"

#include <gtkeeper.h>

class ProgramasScreen:ScreenBase {
public:
	ProgramasScreen();

	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnSetFocus(uint8_t* field,uint8_t* selpos);
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
	virtual void OnPaint();
	virtual bool ShowBlink() { return false;}
private:
	void ShowHoraInicio(bool show);
	void setPrograma(int8_t programa);
	void SetVisibleFields(bool visible);
	bool showed;
	uint8_t currentprograma;
	tPrograma *program;
};


extern ProgramasScreen programasScreen;


#endif
#endif
