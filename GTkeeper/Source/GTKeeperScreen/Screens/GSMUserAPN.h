/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef GSMUSERAPN_H_
#define GSMUSERAPN_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <gtkeeper.h>
#include "LongTextBase.h"
#include <ScreenBase.h>
#include <ScreenManager.h>

#include "GSMMenu.h"

class GSMUserAPNScreen: public LongTextBaseScreen {
public:
	GSMUserAPNScreen();


	virtual void  OnSave ();
	virtual void  OnGotoMenu();
	virtual char* GetTitle();
	virtual char* GetValue();



};
extern GSMUserAPNScreen gsmuserapnScreen;

#endif
#endif
