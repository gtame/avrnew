/*
 * LanzaProgramas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "GSMTelefono.h"
#ifdef PANTALLA_TECLADO
GsmTelefonoScreen gsmTelefonoScreen;


GsmTelefonoScreen::GsmTelefonoScreen() {
	// TODO Auto-generated constructor stub

}



void GsmTelefonoScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen

	screenManager.AddFields(10,1,10,TextEdit);//Sector



	screenManager.AddFields(3,3,3,Button,"YES");//Sabado
	screenManager.AddFields(13,3,2,Button,"NO");//Domingo

	//Inicializa campos de dia de la semana

 	strncpy(screenManager.strings,Config.config.MovilAviso,strlen(Config.config.MovilAviso));


	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);



}



void GsmTelefonoScreen::OnDrawFields()
{


	screenManager.m_lcd->setCursor(1,1);
	screenManager.m_lcd->print(F("TELEFONO:"));



}





bool GsmTelefonoScreen::SetKey(unsigned char c)
{

  //Tecla eliminar campo

  //Tecla cancel
  if (c=='C')
  {
	  if (screenManager.GetCursorPos()==2)
	  {
			screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);


	  }
	  else
		  screenManager.SetFocus(2,0);

	  return false;
  }



  return true;

}



void GsmTelefonoScreen::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field==1)//OK
	{
		char* phone=screenManager.GetValueField(0);
			LOG_DEBUG_ARGS("N %s",phone);
			if (strlen(phone)==9)
			{
				strcpy(Config.config.MovilAviso,phone);

				GSMModem.SetDefaultSMSNum(Config.config.MovilAviso);
				Config.EEPROMGuardaConfig();
				screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
			}
			else
				screenManager.ShowMsgBox("Numero Incorrecto",MsgOkButton);
	}
	else if (field==2) //CAncel
	{
		screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
	}
}

#endif


