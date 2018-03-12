/*
 * EstadisticasMenu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "EstadisticasMenu.h"
#ifdef PANTALLA_TECLADO


EstadisticasMenu::EstadisticasMenu() {
	// TODO Auto-generated constructor stub
}



void EstadisticasMenu::OnEnter()
{

	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,20,Option,MNU_STA_TEXT_ESTADISTICA);
	screenManager.AddFields(0,1,20,Option,MNU_STA_TEXT_RESET);
}


bool EstadisticasMenu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return true;
}

 void EstadisticasMenu::ResetearEstadisticas( uint8_t result,uint8_t codefunc )
{
	 if (result==0)
	 {
		 LOG_DEBUG_ARGS("REseteando estadisticas!! %i",result);
		 gtKeeper.ResetearEstadisticas();
		 //gtKeeper.ApagarRiegos();

	 }
}


void EstadisticasMenu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //Programas
				screenManager.SetPantallaActual((ScreenBase *)&estadisticadetalleScreen);
			break;
		case 1: //Parametros
				screenManager.ShowMsgBox("DESEA RESETEAR LAS\n ESTADISTICAS?",MsgYesNoButton,ResetearEstadisticas);
			break;
	}

}


//Constructor de menuScreen
EstadisticasMenu estadisticasmenuScreen;
#endif
