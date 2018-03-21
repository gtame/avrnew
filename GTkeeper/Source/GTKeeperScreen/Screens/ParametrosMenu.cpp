/*
 * ParametrosMenu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "ParametrosMenu.h"
#ifdef PANTALLA_TECLADO


ParametrosMenu::ParametrosMenu() {


	// TODO Auto-generated constructor stub
}



void ParametrosMenu::OnEnter()
{

	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,20,Option, MNU_PARAM_TEXT_GSM );
	screenManager.AddFields(0,1,20,Option, MNU_PARAM_TEXT_HORA);
	screenManager.AddFields(0,2,20,Option,MNU_PARAM_TEXT_SYS);


}


bool ParametrosMenu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return true;
}



void ParametrosMenu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //GSM
			screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
			break;
		case 1: //HORA
			screenManager.SetPantallaActual((ScreenBase*)&fechahoraScreen);
			break;
		case 2: //SISTEMA
			screenManager.SetPantallaActual((ScreenBase*)&sistemasScreen);
			break;


	}

}


//Constructor de menuScreen
ParametrosMenu parametrosMenuScreen;
#endif
