/*
 * Menu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Parametros.h"

Parametros::Parametros() {


	// TODO Auto-generated constructor stub
}



void Parametros::SetMode(uint8_t newmode)
{
    //char* buffer=pActual.buffer;

	if (mode!=newmode)
	{
		mode=newmode;
		pActual.ClearFields();

		switch (mode)
		{
			case MODE_MENU: //Menu parametros
			{
				pActual.SetOptionMode();
				pActual.AddFields(0,0,15,Option,TEXT_HORA);
				pActual.AddFields(0,1,15,Option,TEXT_TELEFONO);
				pActual.AddFields(0,2,15,Option,TEXT_AVISOS);


			}
			break;
			case MODE_HORA://Hora
			{

				pActual.SetTextMode();

				//Addfields in screen
				pActual.AddFields(0,1,2,TextEdit);//Dia
				pActual.AddFields(3,1,2,TextEdit);//Mes
				pActual.AddFields(6,1,4,TextEdit);//Año
				pActual.AddFields(11,1,2,TextEdit);//Hora
				pActual.AddFields(14,1,2,TextEdit);//Minuto


				//memset(pActual.buffer, 0, sizeof(pActual.buffer));
				sprintf(buffer, "%02d%02d%04d%02d%02d",   day(),month(),year(), hour(),minute());
				pActual.SetTextField(0,buffer);

				/*
				memset(pActual.buffer, 0, sizeof(pActual.buffer));
				sprintf(pActual.buffer, "Gola mundo crueal %i", mode);
				pActual.SetTextField(0,pActual.buffer);


				pActual.AddFields(0,3,2,Button,"OK");//Aceptar
				pActual.AddFields(10,3,4,Button,"CANCEL");//Cancelar


*/

			}
			break;
			case MODE_TELEFONO://Telefono
			{

				pActual.SetTextMode();
				//Addfields in screen
				pActual.AddFields(8,0,4,TextEdit,config.Pin);
				//pActual.AddFields(4,1,16,AlfaTextEdit,config.ServerAPN);
				pActual.AddFields(4,1,25,AlfaTextEdit,config.ServerAPN);
				pActual.AddFields(5,2,9,AlfaTextEdit,config.Login);
				pActual.AddFields(9,3,9,AlfaTextEdit,config.Password);


			}
			break;
			case MODE_AVISOS:
			{

				pActual.SetTextMode();
				//Addfields in screen
				pActual.AddFields(10,0,9,TextEdit,config.Telefono1);//Telefono
				pActual.AddFields(7,1,12,AlfaTextEdit);//Email primera linea
				pActual.AddFields(0,2,20,AlfaTextEdit);//Email segunda lineas



			}
			break;
		}

		pActual.needrefresh=true;
	}
}


void Parametros::OnFieldChange(uint8_t field)
{

  char* value=pActual.GetValueField(field);

  switch (mode)
  {
	  case MODE_HORA:
	  {


			 TimeElements hora;
			 hora.Second=0;
			 hora.Minute=atoi(pActual.GetValueField(4));;
			 hora.Hour=atoi(pActual.GetValueField(3));;
			 hora.Day=atoi(pActual.GetValueField(0));
			 hora.Month=atoi(pActual.GetValueField(1));
			 hora.Year=atoi(pActual.GetValueField(2))-1970;


			 RTC.set(makeTime(hora));
			 delay(100);
			 setSyncProvider(RTC.get);



	  }
	  break;
  }
}


void Parametros::OnEnter()
{

	mode=0;
	SetMode(MODE_MENU);

}

void Parametros::OnDrawFields()
{
	switch(mode)
	{
		case MODE_HORA:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("Introduzca la hora");
			pActual.m_lcd->setCursor(0,1);
			pActual.m_lcd->print("  -  -       :");
		} break;
		case MODE_TELEFONO:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("SIM PIN:");
			pActual.m_lcd->setCursor(0,1);
			pActual.m_lcd->print("APN:");
			pActual.m_lcd->setCursor(0,2);
			pActual.m_lcd->print("USER:");
			pActual.m_lcd->setCursor(0,3);
			pActual.m_lcd->print("PASSWORD:");

		} break;
		case MODE_AVISOS:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("TELEFONO:");
			pActual.m_lcd->setCursor(0,1);
			pActual.m_lcd->print("EMAIL:");
		}break;
	}
}

void Parametros::OnClickButton(uint8_t field)
{

	switch (mode)
	{
		case MODE_MENU: //Lanzar
		{

			switch (field)
			{
				case 0: //Hora
					SetMode(MODE_HORA);
					break;
				case 1: //Telefono
					SetMode(MODE_TELEFONO);
					break;
				case 2: //Avisos
					SetMode(MODE_AVISOS);
					break;
			}
		}
		break;
	}

}

bool Parametros::SetKey(unsigned char c)
{

  //Tecla cancel
  if (c=='C')
  {
		switch (mode)
		{
			case MODE_MENU: //Lanzar
				setPantallaActual(MENU_PANTALLA);

				//PAra lanzar tenemos que pedirle que especifique el nº de programa... ;)
				break;
			case MODE_TELEFONO:
			case MODE_AVISOS:
			case MODE_HORA: //PArar
				//
				SetMode(MODE_MENU);
				break;


		}
  }
  /*else if (c=='#' && mode==)
  {
	  //Lanza el programa. ;>
	  //Vuelve al menu ;)
	  setPantallaActual(MENU_PANTALLA);
	  return false;
  }
*/

  return true;
}


