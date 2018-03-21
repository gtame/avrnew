/*
 * Programas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */


#include "EstadisticasDetalle.h"
#ifdef PANTALLA_TECLADO
EstadisticasDetalleScreen estadisticadetalleScreen;


EstadisticasDetalleScreen::EstadisticasDetalleScreen() {

}


void EstadisticasDetalleScreen::OnEnter()
{
	// TODO Auto-generated constructor stub
	setSector(1);
}

void EstadisticasDetalleScreen::OnDrawFields()
{
	char *buffer=screenManager.BufferScreen;

	memset(buffer,0,SC_BUFFER_SIZE);
	screenManager.m_lcd->setCursor(0,0);
	sprintf_P(buffer,PSTR("Sector: %02i"),estadistica->sector);
	screenManager.m_lcd->print(buffer);


	memset(buffer,0,SC_BUFFER_SIZE);
	screenManager.m_lcd->setCursor(0,1);
	sprintf_P(buffer,PSTR("Riego:  %lu H %02i M"),NUMERO_HORAS(estadistica->tiempo_riego),NUMERO_MINUTOS(estadistica->tiempo_riego));
	screenManager.m_lcd->print(buffer);


	memset(buffer,0,SC_BUFFER_SIZE);
	screenManager.m_lcd->setCursor(0,2);
	sprintf_P(buffer,PSTR("Abono:  %lu H %02i M"),NUMERO_HORAS(estadistica->tiempo_abono),NUMERO_MINUTOS(estadistica->tiempo_abono));

	screenManager.m_lcd->print(buffer);


}

void EstadisticasDetalleScreen::setSector(uint8_t sector)
{
	if (sector==0)
		sector=1;

	estadistica = &Riego.estadisticas[sector-1];
	currentsector=sector;
	screenManager.needrefresh=true;

}


bool EstadisticasDetalleScreen::SetKey(unsigned char c)
{
	//Tecla subir
  if (c=='A')
  {
	  if (currentsector<PORTS_NUM)
		  currentsector++;
	  else
		  currentsector=1;

	  setSector(currentsector);
  }
  //Tecla cancel
  else if (c=='C')
  {
	  screenManager.SetPantallaActual((ScreenBase *)&estadisticasmenuScreen);
  }

  //Tecla baja
  else if (c=='D')
  {
	  if (currentsector==1)
		  currentsector=PORTS_NUM;
	  else
		  currentsector--;

	  setSector(currentsector);
  }

  return true;

}

#endif


