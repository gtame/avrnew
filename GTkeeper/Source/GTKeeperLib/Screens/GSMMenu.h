/*
 * GSMMenu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#ifdef PANTALLA_TECLADO


#ifndef GSMMENU_H_
#define GSMMENU_H_



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ScreenBase.h>
#include <ScreenManager.h>
#include "GSMAPN.h"
#include "Programas.h"
#include "Sistema.h"
#include "FechaHora.h"
#include "..\textos.h"
#include "GSMTelefono.h"
#include "GSMSMSAviso.h"
#include "GSMUserAPN.h"
#include "GSMPwdAPN.h"

class GSMMenu:ScreenBase {
public:
	GSMMenu();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	bool isReady();
};


extern GSMMenu gsmMenuScreen;

#endif /* GSMMENU_H_ */
#endif
