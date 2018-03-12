/*
 * LanzaProgramas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "LanzarPrograma.h"
#ifdef PANTALLA_TECLADO
LanzaProgramasScreen lanzaprogramasScreen;




LanzaProgramasScreen::LanzaProgramasScreen() {
	// TODO Auto-generated constructor stub

}


void LanzaProgramasScreen::OnDrawFields()
{
	screenManager.m_lcd->setCursor(0,1);
	screenManager.m_lcd->print(F("PROGRAMA:"));
}


void  LanzaProgramasScreen::OnGotoMenu()
{
	screenManager.SetPantallaActual((ScreenBase *)&manualScreen);
}


void  LanzaProgramasScreen::OnSelection ()
{
	LOG_DEBUG_ARGS("LANZANDO PROGRAMA %i",currentvalue);
	gtKeeper.LanzaRiego(currentvalue-1,false);
	OnGotoMenu();

}


#endif

