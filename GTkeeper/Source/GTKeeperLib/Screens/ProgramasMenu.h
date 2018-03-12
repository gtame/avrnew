/*
 * ProgramasMenu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef PROGRAMASMENU_H_
#define PROGRAMASMENU_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "Programas.h"
#include "Menu.h"
#include "ProgramasTanda.h"
#include "..\textos.h"




class ProgramasMenu:ScreenBase {
public:
	ProgramasMenu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	static void  ResetearProgramas( uint8_t result,uint8_t codefunc );
private:
	void (*MsgboxCallBack)( uint8_t result,uint8_t codefunc );
};


extern ProgramasMenu programasmenuScreen;

#endif /* PROGRAMASMENU_H_ */
#endif
