/*
 * Consulta.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */
#ifdef PANTALLA_TECLADO
#ifndef CONSULTA_H_
#define CONSULTA_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "Menu.h"
#include <gtkeeper.h>

class Consulta:ScreenBase {
public:
	Consulta();
	virtual void OnEnter();
	virtual void OnDrawFields();
	virtual bool SetKey(unsigned char c);

	virtual bool ShowCursor() { return false;}
	virtual bool ShowBlink() { return false;}


	virtual bool NeedRefresh();

private:
	void writeMeter( int level );
	uint8_t current_cobertura;
	time_t _lastrefresh;
	uint8_t num_activos;
};

extern Consulta consultaScreen;

#endif /* CONSULTA_H_ */
#endif
