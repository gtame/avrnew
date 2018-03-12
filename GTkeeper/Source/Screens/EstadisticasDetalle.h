/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef ESTADISTICASDETCREEN_H_
#define ESTADISTICASDETCREEN_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Programas.h"
#include <ScreenBase.h>
#include <ScreenManager.h>
#include "EstadisticasMenu.h"

#include "..\gtkeeper.h"

class EstadisticasDetalleScreen:ScreenBase {
public:
	EstadisticasDetalleScreen();
	virtual void OnEnter();
	virtual void OnDrawFields();
	virtual bool SetKey(unsigned char c);
	virtual bool ShowBlink() { return false;}
	virtual bool ShowCursor() { return false;}
private:
	void setSector(uint8_t sector);
	uint8_t currentsector;
	Estadistica estadistica;
};


extern EstadisticasDetalleScreen estadisticadetalleScreen;


#endif
#endif
