/*
 * LanzaProgramas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "GSMSMSAviso.h"
#ifdef PANTALLA_TECLADO
GsmSmsAvisoScreen gsmSmsAvisoScreen;


GsmSmsAvisoScreen::GsmSmsAvisoScreen() {
	// TODO Auto-generated constructor stub

}



void GsmSmsAvisoScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen
	//PASS SMS: #____#
	//_ INICIAR   _ PARAR
	//_ REINICIAR _ WEB

	screenManager.AddFields(11,0,4,TextEdit);//Pass SMS

	screenManager.AddFields(0,1,1,Button);//INICIAR
	screenManager.AddFields(13,1,1,Button);//PARAR
	screenManager.AddFields(0,2,1,Button);//REINICIAR
	screenManager.AddFields(13,2,1,Button);//WEB


	screenManager.AddFields(3,3,3,Button,"YES");//Sabado
	screenManager.AddFields(13,3,2,Button,"NO");//Domingo

	//Inicializa campos de dia de la semana

 	char* buffer=screenManager.BufferScreen;
	memset(buffer,0,sizeof(SC_BUFFER_SIZE));

	strncpy(buffer,gtKeeper.config.PasswordSMS,4);

	sprintf_P(buffer+4,PSTR("%c%c%c%c"),
			(gtKeeper.config.AvisosSMS & SMSInicioSector?'X':'_'),
			(gtKeeper.config.AvisosSMS & SMSFinSector?'X':'_'),
			(gtKeeper.config.AvisosSMS & SMSProgramacionWeb?'X':'_'),
			(gtKeeper.config.AvisosSMS & SMSReset?'X':'_'));

	strncpy(screenManager.strings,buffer,strlen(buffer));

	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);


}



void GsmSmsAvisoScreen::OnDrawFields()
{


	screenManager.m_lcd->setCursor(0,0);
	screenManager.m_lcd->print(F("PASS SMS: #"));

	screenManager.m_lcd->setCursor(15,0);
	screenManager.m_lcd->print('#');

	screenManager.m_lcd->setCursor(2,1);
	screenManager.m_lcd->print(F("INICIAR"));
	screenManager.m_lcd->setCursor(15,1);
	screenManager.m_lcd->print(F("PARAR"));

	screenManager.m_lcd->setCursor(2,2);
	screenManager.m_lcd->print(F("REINICIAR"));
	screenManager.m_lcd->setCursor(15,2);
	screenManager.m_lcd->print(F("WEB"));
}





bool GsmSmsAvisoScreen::SetKey(unsigned char c)
{

  //Tecla eliminar campo

  //Tecla cancel
  if (c=='C')
  {
	  if (screenManager.GetCursorPos()==6)
			screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
	  else
		  screenManager.SetFocus(6,0);

	  return false;
  }



  return true;

}



void GsmSmsAvisoScreen::OnClickButton(uint8_t field)
{
	char* value=screenManager.GetValueField(field);
	if (field>=1 && field<=4)//OK
	{
		if (strncmp(value,"X",1)==0)
			screenManager.fields[field].value[0]=' ';
		else
			screenManager.fields[field].value[0]='X';
	}

	//Campos correspondientes a los dias de la semana
	else if (field==5)//OK
	{
			char* pass=screenManager.GetValueField(0);
			LOG_DEBUG_ARGS("N %s",pass);
			if (strlen(pass)==4)
			{
				strcpy(gtKeeper.config.PasswordSMS,pass);

				/*Ahora el AvisoSMS*/
				gtKeeper.config.AvisosSMS=0;

				//iniciar
				 value=screenManager.GetValueField(1);
				 if (strncmp(value,"X",1)==0)
					 gtKeeper.config.AvisosSMS |= SMSInicioSector;
				 //parar
				 value=screenManager.GetValueField(2);
				 if (strncmp(value,"X",1)==0)
					 gtKeeper.config.AvisosSMS |= SMSFinSector;
				 //reiniciar
				 value=screenManager.GetValueField(3);
				 if (strncmp(value,"X",1)==0)
					 gtKeeper.config.AvisosSMS |= SMSProgramacionWeb;

				 //web
				 value=screenManager.GetValueField(4);
				 if (strncmp(value,"X",1)==0)
					 gtKeeper.config.AvisosSMS |= SMSReset;

				gtKeeper.EEPROMGuardaConfig();
				screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
			}
			else
				screenManager.ShowMsgBox("Pass Incorrecta",MsgOkButton,NULL);
	}
	else if (field==6) //CAncel
	{
		screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
	}
}


#endif

