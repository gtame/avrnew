/*
 * Sistemas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "Sistema.h"
#ifdef PANTALLA_TECLADO
SistemasScreen sistemasScreen;

SistemasScreen::SistemasScreen() {
	// TODO Auto-generated constructor stub

}


/*


	bool GSMAvailable;
	//num de puertos - salidas
	uint8_t numpuertos;
	uint8_t numabono;

	bool motor_diesel;
 *
 * */
void SistemasScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen
	//LINEA0
	screenManager.AddFields(5,0,1,Button);//GSMAvailable
	screenManager.AddFields(17,0,1,Button);//Motor diesel

	screenManager.AddFields(15,1,2,TextEdit,1,PORTS_NUM);//SECTORES
	screenManager.AddFields(15,2,2,TextEdit,0,2);//Abonos

	//Inicializa campos de dia de la semana
	setData();

	//Pongo el cursor sobre el programa inicio
	screenManager.SetFocus(0,0);
}


void SistemasScreen::OnFieldChange(uint8_t field)
{

	  char* value=screenManager.GetValueField(field);
	  uint8_t valor_num=atoi(value);

	  LOG_DEBUG_ARGS("VAlor de campo %i->%s",field,value);

	  switch(field)
	  {


	   case 2://Sectores
	   {
	 	   if (valor_num>0 && valor_num<=PORTS_NUM)
	 	   {
	 		  gtKeeper.config.numpuertos=valor_num;
	 		  gtKeeper.EEPROMGuardaConfig();
	 	   }

	    }
	    break;
	   case 3://ABonos
	   {
	 	   if (valor_num>=0 && valor_num<=2)
	 	   {
	 		   gtKeeper.config.numabono=valor_num;
	 		  gtKeeper.EEPROMGuardaConfig();
	 	   }

	    }

	  }

	  setData();
}


void SistemasScreen::setData()
{
	char* buffer=screenManager.BufferScreen;
	memset(buffer,0,sizeof(SC_BUFFER_SIZE));
	sprintf_P(buffer,PSTR("%c%c%02i%02i"), (gtKeeper.config.GSMAvailable?'X':'_'),(gtKeeper.config.motor_diesel?'X':'_'),gtKeeper.config.numpuertos,gtKeeper.config.numabono);
 	strncpy(screenManager.strings,buffer,strlen(buffer));
}


void SistemasScreen::OnDrawFields()
{

	screenManager.m_lcd->setCursor(0,0);
	screenManager.m_lcd->print("GSM");

	screenManager.m_lcd->setCursor(11,0);
	screenManager.m_lcd->print("MOTOR");

	screenManager.m_lcd->setCursor(0,1);
	screenManager.m_lcd->print("NUM PROGRAMAS");

	screenManager.m_lcd->setCursor(0,2);
	screenManager.m_lcd->print("NUM ABONOS");


	setData();
}


/*

 	//Programa Inicial
	//Sector desde
	//Sector Hasta
	//Segunda Linea A Tiempo de reigo y Abono
	//Tiempo de riego en Horas
	//Tiempo de riego en Minutos
	//Tiempo de abono en Horas
	//Tiempo de abono en Minutos

	//LINEA3
	/Tandas
 * */
bool SistemasScreen::SetKey(unsigned char c)
{

	if (c=='C')
	  {
		screenManager.SetPantallaActual((ScreenBase *)&parametrosMenuScreen);

	  }
	else
		  return true;//Procesamiento por default

}



void SistemasScreen::OnClickButton(uint8_t field)
{

	if (field==0) //GSM
	{
		//Hacemos el proceso
		gtKeeper.config.GSMAvailable=!gtKeeper.config.GSMAvailable;

	}
	else if (field==1 )//MOTOR
	{
		gtKeeper.config.motor_diesel=!gtKeeper.config.motor_diesel;
	}

	setData();
	gtKeeper.EEPROMGuardaConfig();

}
#endif

