/*
 * LanzaProgramas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "LanzarSector.h"
#ifdef PANTALLA_TECLADO
LanzaSectorScreen lanzasectorScreen;


LanzaSectorScreen::LanzaSectorScreen() {
	// TODO Auto-generated constructor stub

}


void  LanzaSectorScreen::OnGotoMenu()
{

	screenManager.SetPantallaActual((ScreenBase *)&manualScreen);
}

void LanzaSectorScreen::OnDrawFields()
{


	screenManager.m_lcd->setCursor(1,1);
	screenManager.m_lcd->print(F("SECTOR:"));



}

void  LanzaSectorScreen::OnSelection ( )
{
	LOG_DEBUG_ARGS("LANZANDO SECTOR %i",currentvalue);
	gtKeeper.EnciendeSector(currentvalue);
	OnGotoMenu();

}


#endif
