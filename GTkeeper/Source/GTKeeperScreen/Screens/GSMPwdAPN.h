/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#ifdef PANTALLA_TECLADO
#ifndef GSMPWDAPN_H_
#define GSMPWDAPN_H_


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

class GSMPwdAPNScreen: public LongTextBaseScreen {
public:
	GSMPwdAPNScreen();


	virtual void  OnSave ();
	virtual void  OnGotoMenu();
	virtual char* GetTitle();
	virtual char* GetValue();



};
extern GSMPwdAPNScreen gsmpwdapnScreen;

#endif
#endif
