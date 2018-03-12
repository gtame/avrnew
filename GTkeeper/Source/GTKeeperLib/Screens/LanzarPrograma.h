/*
 * LanzaProgramas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */
#ifdef PANTALLA_TECLADO

#ifndef LANZAPROGRAMASCREEN_H_
#define LANZAPROGRAMASCREEN_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>

#include "..\gtkeeper.h"
#include "LanzarBase.h"
#include "ManualMenu.h"

class LanzaProgramasScreen:public LanzarBaseScreen {
public:
	LanzaProgramasScreen();

	virtual void OnDrawFields();

	virtual void  OnSelection ();
	virtual uint8_t MaxValue () {return  MAX_PROGRAMAS;}
	virtual void  OnGotoMenu();
};

extern LanzaProgramasScreen lanzaprogramasScreen;

#endif /* LANZAPROGRAMAS_H_ */
#endif



