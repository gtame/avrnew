/*
 * LanzaProgramas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO

#ifndef LANZASECTORSCREEN_H_
#define LANZASECTORSCREEN_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "ManualMenu.h"
#include <gtkeeper.h>
#include "LanzarBase.h"


class LanzaSectorScreen:LanzarBaseScreen {
public:
	LanzaSectorScreen();

	virtual void OnDrawFields();
	virtual void  OnSelection ();
	virtual uint8_t MaxValue () {return  PORTS_NUM;}

	virtual void  OnGotoMenu();
private:
	void  SetValor (uint8_t valor);
	uint8_t _currentValor;
};

extern LanzaSectorScreen lanzasectorScreen;

#endif /* LANZAPROGRAMAS_H_ */
#endif

