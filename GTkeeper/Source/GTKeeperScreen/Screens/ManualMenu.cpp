/*
 * ManualMenu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "ManualMenu.h"
#ifdef PANTALLA_TECLADO


ManualMenu::ManualMenu() {


	// TODO Auto-generated constructor stub
}



void ManualMenu::OnEnter()
{

	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,20,Option,MNU_MAN_TEXT_LANZAR_PROGRAMA);
	screenManager.AddFields(0,1,20,Option,MNU_MAN_TEXT_LANZAR_SECTOR);
	screenManager.AddFields(0,2,20,Option,MNU_MAN_TEXT_PARAR);
	//screenManager.AddFields(0,3,20,Option,MNU_MAN_TEXT_LIMPIAR);

}


bool ManualMenu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return true;
}

 void ManualMenu::PararRiegos( uint8_t result,uint8_t codefunc )
{
	 if (result==0)
	 {
		 LOG_DEBUG_ARGS("Parando riegos!! %i",result);
		 Riego.ApagarRiegos();
	 }
}


void ManualMenu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //Programas
		{
			screenManager.SetPantallaActual((ScreenBase *)&lanzaprogramasScreen);
		}
			break;
		case 1: //Manual
		{

			screenManager.SetPantallaActual((ScreenBase *)&lanzasectorScreen);
		}
			break;
		case 2: //Parametros
			screenManager.ShowMsgBox_P(PSTR("DESEA PARAR TODOS\n RIEGOS ACTIVOS?"),MsgOKCancelButton,PararRiegos);
			break;
		case 3: //Consulta
			//
			//screenManager.SetPantallaActual(PROGRAMAS_PANTALLA);
			 break;

	}

}


//Constructor de menuScreen
ManualMenu manualScreen;
#endif
