/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#ifdef PANTALLA_TECLADO
#ifndef PROGRAMASTANDA_H_
#define PROGRAMASTANDA_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#include "..\gtkeeper.h";
#include <ScreenBase.h>
#include <ScreenManager.h>
#include "ProgramasMenu.h"

typedef struct  {

	uint8_t program;
	uint8_t sector_desde;
	uint8_t sector_hasta;
	uint8_t riegohora;
	uint8_t riegomin;
	uint8_t abonohora;
	uint8_t abonomin;
	uint8_t tanda;

} 	tTandaProg, TandaProg;



class ProgramasTandaScreen: public ScreenBase {
public:
	ProgramasTandaScreen();

	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
	virtual bool ShowBlink() { return false;}
	static void  GotoMenu( uint8_t result,uint8_t codefunc );
private:
	void setData();
	TandaProg _tanda;


};

extern ProgramasTandaScreen programastandaScreen;
#endif
#endif
