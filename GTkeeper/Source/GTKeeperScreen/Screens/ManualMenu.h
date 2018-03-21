/*
 * ManualMenu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef MANUALMENU_H_
#define MANUALMENU_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "Consulta.h"
#include "Programas.h"
#include "LanzarPrograma.h"
#include "LanzarSector.h"
#include "..\textos.h"




class ManualMenu:ScreenBase {
public:
	ManualMenu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	static void  PararRiegos( uint8_t result,uint8_t codefunc );

};


extern ManualMenu manualScreen;

#endif /* MANUALMENU_H_ */
#endif
