/*
 * Menu.h
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */



#ifndef PARAMETROS_H_
#define PARAMETROS_H_


#include <LiquidCrystal_I2C.h>
#include <DS1307RTC.h>
#include <WProgram.h>
#include <Pantalla.h>
#include "../main.h"
#include <Frame.h>

#define MODE_MENU 1
#define MODE_HORA 2
#define MODE_TELEFONO 3
#define MODE_AVISOS 4

#define TEXT_HORA "1-Hora"
#define TEXT_TELEFONO "2-Telefono"
#define TEXT_AVISOS "3-Avisos"


class Parametros:Frame {
public:
	Parametros();
	virtual void OnClickButton(uint8_t field);
	virtual void OnEnter();
	virtual bool SetKey(unsigned char c);
	virtual void OnDrawFields();
	virtual void OnFieldChange(uint8_t field);

private:
	void SetMode(uint8_t mode);
	uint8_t mode;
};

#endif /* MENU_H_ */
