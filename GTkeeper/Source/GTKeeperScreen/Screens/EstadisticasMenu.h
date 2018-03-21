/*
 * EstadisticasMenu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO

#ifndef ESTADISTICASMENU_H_
#define ESTADISTICASMENU_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "Menu.h"
#include "..\textos.h"
#include "EstadisticasDetalle.h"



class EstadisticasMenu:ScreenBase {
public:
	EstadisticasMenu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	static void  ResetearEstadisticas( uint8_t result,uint8_t codefunc );

};


extern EstadisticasMenu estadisticasmenuScreen;

#endif /* ESTADISTICASMENU_H_ */

#endif
