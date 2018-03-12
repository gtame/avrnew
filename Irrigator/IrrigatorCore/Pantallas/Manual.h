/*
 * Menu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */



#ifndef MANUAL_H_
#define MANUAL_H_


#include <LiquidCrystal_I2C.h>
#include <WProgram.h>
#include <Pantalla.h>
#include "../main.h"
#include <Frame.h>
#include "../constants.h"


#define TEXT_LANZAR "1-Lanzar Programa"
#define TEXT_PARAR "2-Parar Riegos"
#define TEXT_BORRAR "3-Borrar Programas"


#define MODE_MENU 0
#define MODE_LANZAR 1
#define MODE_PARAR 2
#define MODE_BORRAR 3


class Manual:Frame {
public:
	Manual();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	virtual void OnDrawFields();
private:
	void SetMode(uint8_t mode);
	uint8_t mode;
};

#endif /* MENU_H_ */
