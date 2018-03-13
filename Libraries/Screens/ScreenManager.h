/*
 * Pantalla.h
 *
 *  Created on: 08/06/2010
 *      Author: gabi
 */

// include the library code:




#ifndef PANTALLA_H_
#define PANTALLA_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <Time.h>
#include "ScreenBase.h"
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <Logger.h>
#include <math.h>

#define OK_TEXT "O"
#define CANCEL_TEXT "C"
#define T_POINTS '.'

#define SC_MAX_FIELDS_SCREEN 16
#define SC_MAX_TEXT_FIELDS 100
#define INTERNAL_SC_BUFFER_SIZE 25
#define SC_BUFFER_SIZE 100

//Funcion usada para tiempo de inactividad
#define ELAPSED_SECONDS(X)  (now()-X)


typedef enum {AlfaTextEdit, TextEdit, Button, Option}  fieldstype_t ;

typedef enum {Estandar,Replace}  modeedition_t ;

typedef enum {MsgOkButton,MsgOKCancelButton,MsgYesNoButton}  msgboxmode_t,msgboxmode ;

typedef struct
{
	uint8_t posX;
	uint8_t posY;
	uint8_t length;
	char * value;
	fieldstype_t type;
	bool enabled;
	bool visible;
	modeedition_t editmode;
	uint8_t maxvalue;
	uint8_t minvalue;
} t_fields;


class ScreenManager {
public:
	ScreenManager();
	virtual void Initializate(LiquidCrystal_I2C* lcd,uint8_t cols,uint8_t rows,Keypad * keypad);
	virtual void OnEnter();
	virtual void OnExit();
	virtual void Paint();
	virtual void ClickButton(uint8_t field);
	virtual void OkButton();
	virtual void CancelButton();
	virtual void DrawFields();
	virtual void DrawFields(bool force);

	void SetPantallaActual(ScreenBase* screen);

	void AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type,uint8_t minvalue,uint8_t maxvalue);
	void AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type);
	void AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type,const char* txt);

	void AddFields(uint8_t posX, uint8_t posY, fieldstype_t type,const char* txt);
	void SetTextField(uint8_t field,const char* txt);
	char* GetValueField(uint8_t field);

	uint8_t GetCursorPos();
	void ClearFields();
	void SetFocus(uint8_t field,uint8_t sellength);
	virtual void SetKey(unsigned char c);
	virtual void SetCursorPos(uint8_t X,uint8_t Y);
	void SetOptionMode();
	void SetTextMode();
	//int SendToBuffer(const char *__fmt, ...);

	void ShowMsgBox(char *,msgboxmode,void (*MsgboxCallBack)( uint8_t result,uint8_t codefunc ));
	void ShowMsgBox_P(PGM_P ,msgboxmode,void (*MsgboxCallBack)( uint8_t result,uint8_t codefunc ));
	int8_t ShowMsgBox(char * ,msgboxmode,uint8_t delayseconds=0 ) ;
	int8_t ShowMsgBox_P(PGM_P ,msgboxmode,uint8_t delayseconds=0 ) ;

	void Loop();


	uint8_t ncols;
	uint8_t nrows;

	ScreenBase * frame;


//protected:
	//Indicador que necesita Repintarse
	bool needrefresh;

	//Variable del lcd
	LiquidCrystal_I2C* m_lcd;

	//Posicion x global en el lcd
	uint8_t posX;

	//Posicion y global en el lcd
	uint8_t posY;

	//Campo donde esta ubicado actualmente el cursor
	uint8_t fieldpos;

	//Dentro del campo ubicado donde se encuentra
	uint8_t sellength;

	//Cuantos campos hay
	uint8_t fieldscount;

	//longuitud ocupada
	uint8_t lenstring;

	//Contenedor de campos
	t_fields fields[SC_MAX_FIELDS_SCREEN];

	//Contenedor de strings
	char strings[SC_MAX_TEXT_FIELDS];


//Variables añadidas para las pantallas de opciones
	bool bOptionMode;
	uint8_t actualpage;

	void PrintTextLine(uint8_t row,const char* mensaje, const char* result);
	void PrintTextLine_P(uint8_t row,PGM_P mensaje,PGM_P result);


	void Apagar();//Apaga la pantalla
	void Encender();//Encender pantalla

	inline bool IsActive ()	{return !bisInactive;}

	inline uint8_t GetTimeInactivity(){ return inactivity_seconds;}
	inline void SetTimeInactivity(uint8_t value) { inactivity_seconds=value;}
	//
	char BufferScreen [SC_BUFFER_SIZE];
private:
	void PrintBufferToLine(uint8_t row,const char* result,bool isPGM);
	Keypad *m_keypad;
	time_t _lastActivity;
	uint8_t inactivity_seconds;
	void PutChar(unsigned char c);
	char buffer[INTERNAL_SC_BUFFER_SIZE];
	bool bisInactive;


//Msgbox VAr-Methods
	void _ExitMsgbox();
	bool isMessageBox;
	char * msgboxtxt;
	msgboxmode msgboxmod;
	uint8_t msgboxbtn;
	void (*MsgboxCallBack)( uint8_t result,uint8_t codefunc );

};



extern ScreenManager screenManager;



#endif /* PANTALLA_H_ */
