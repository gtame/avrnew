/*
 * Frame.h
 *
 *  Created on: 18/09/2010
 *      Author: Admin
 */



#ifndef FRAME_H_
#define FRAME_H_

#include <LiquidCrystal_I2C.h>
#include <string.h>
// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <math.h>



class ScreenBase {
public:
	ScreenBase();

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnPaint();
	virtual void OnClickButton(uint8_t field);
	virtual void OnOkButton();
	virtual void OnCancelButton();
	virtual void OnDrawFields();
	virtual void OnChangePage(uint8_t newpage);
	virtual void OnFieldChange(uint8_t field);
	virtual bool NeedRefresh();//Funcion llamada para preguntar al frame si necesita refrescar
	virtual bool SetKey(unsigned char c);
	virtual void OnSetFocus(uint8_t* field,uint8_t* selpos);

	virtual bool ShowCursor() { return true;}
	virtual bool ShowBlink() { return true;}

};

#endif /* FRAME_H_ */
