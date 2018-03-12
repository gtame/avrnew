/*
 * Consulta.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */



#ifndef CONSULTA_H_
#define CONSULTA_H_


#include <LiquidCrystal_I2C.h>
#include <WProgram.h>
#include <Pantalla.h>
#include <time.h>
#include "../main.h"
#include <Frame.h>


#define TEXT_PROGRAMAS "1-Programas"
#define TEXT_MANUAL "2-Manual"
#define TEXT_PARAMETROS "3-Parametros"
#define TEXT_CONSULTAS "4-Consulta"

class Consulta:Frame {
public:
	Consulta();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual void OnDrawFields();
	virtual bool SetKey(unsigned char c);
private:
	void writeMeter( int level );
	int current_cobertura;
};

#endif /* CONSULTA_H_ */
