/*
 * Programas.h
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#include <LiquidCrystal_I2C.h>
#include <WProgram.h>
#include <Pantalla.h>
#include "../main.h"
#include "Frame.h"


#ifndef PROGRAMAS_H_
#define PROGRAMAS_H_



class Programas:Frame {
public:
	Programas();
protected:
	virtual void OnDrawFields();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();

	virtual void OnFieldChange(uint8_t field);
	virtual bool SetKey(unsigned char c);
private:
	void setPrograma(int8_t programa);
	uint8_t currentprograma;
};

#endif /* PROGRAMAS_H_ */
