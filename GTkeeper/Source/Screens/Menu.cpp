/*
 * Menu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Menu.h"
#ifdef PANTALLA_TECLADO


Menu::Menu() {


	// TODO Auto-generated constructor stub
}



void Menu::OnEnter()
{

	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,Option,TEXT_PROGRAMAS);
	screenManager.AddFields(0,1,Option,TEXT_MANUAL);
	screenManager.AddFields(0,2,Option,TEXT_PARAMETROS);
	screenManager.AddFields(0,3,Option,TEXT_ESTADISTICAS);

}


bool Menu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&consultaScreen);
		  return false;
	  }

	  return true;
}



void Menu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //Programas
		{
			screenManager.SetPantallaActual((ScreenBase *)&programasmenuScreen);

		}
			break;
		case 1: //Manual
		{
			screenManager.SetPantallaActual((ScreenBase *)&manualScreen);

			//screenManager.ShowMsgBox("HOLA ",MsgOKCancelButton,MsgboxCallBack);
			//screenManager.SetPantallaActual(PROGRAMAS_PANTALLA);
		}
			break;
		case 2: //Parametros
			screenManager.SetPantallaActual((ScreenBase *)&parametrosMenuScreen);
			break;
		case 3: //Consulta
			//
			screenManager.SetPantallaActual((ScreenBase *)&estadisticasmenuScreen);
			 break;

	}

}


//Constructor de menuScreen
Menu menuScreen;
#endif
