/*
 * Menu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Manual.h"

Manual::Manual() {


	// TODO Auto-generated constructor stub
}



void Manual::OnEnter()
{
	mode=-1;
	SetMode(MODE_MENU);

}

void Manual::OnDrawFields()
{
	switch (mode)
	{
		case MODE_LANZAR:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("Num de programa:");
		}break;
		case MODE_PARAR:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("Parar riegos");
			pActual.m_lcd->setCursor(0,1);
			pActual.m_lcd->print(T_PREGUNTA_TEXT);
		}break;

		case MODE_BORRAR:
		{
			pActual.m_lcd->setCursor(0,0);
			pActual.m_lcd->print("Eliminar programas");
			pActual.m_lcd->setCursor(0,1);
			pActual.m_lcd->print(T_PREGUNTA_TEXT);
		}break;
	}
}


void Manual::SetMode(uint8_t newmode)
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
				pActual.AddFields(0,0,20,Option,TEXT_LANZAR);
				pActual.AddFields(0,1,20,Option,TEXT_PARAR);
				pActual.AddFields(0,2,20,Option,TEXT_BORRAR);



			}
			break;
			case MODE_LANZAR://Hora
			{



				pActual.SetTextMode();
				pActual.AddFields(0,3,strlen(T_ACEPTAR_TEXT),Button,T_ACEPTAR_TEXT);//ProgramId
				pActual.AddFields(20-strlen(T_CANCELAR_TEXT),3,strlen(T_CANCELAR_TEXT),Button,T_CANCELAR_TEXT);//ProgramId
				//Addfields in screen
				pActual.AddFields(16,0,2,TextEdit,"00");//ProgramId
				pActual.SetFocus(2,0);



			}
			break;
			case MODE_PARAR://Telefono
			{
				pActual.SetTextMode();
				pActual.AddFields(0,3,strlen(T_ACEPTAR_TEXT),Button,T_ACEPTAR_TEXT);//ProgramId
				pActual.AddFields(20-strlen(T_CANCELAR_TEXT),3,strlen(T_CANCELAR_TEXT),Button,T_CANCELAR_TEXT);//ProgramId
				pActual.SetFocus(0,0);

			}
			break;
			case MODE_BORRAR:
			{


				pActual.SetTextMode();
				pActual.AddFields(0,3,strlen(T_ACEPTAR_TEXT),Button,T_ACEPTAR_TEXT);//ProgramId
				pActual.AddFields(20-strlen(T_CANCELAR_TEXT),3,strlen(T_CANCELAR_TEXT),Button,T_CANCELAR_TEXT);//ProgramId

				pActual.SetFocus(0,0);
			}
			break;
		}

		pActual.needrefresh=true;
	}
}

void Manual::OnClickButton(uint8_t field)
{

	switch (mode)
	{
		case MODE_MENU: //Lanzar
		{

			switch (field)
			{
				case 0: //Hora
					SetMode(MODE_LANZAR);
					break;
				case 1: //Telefono
					SetMode(MODE_PARAR);
					break;
				case 2: //Avisos
					SetMode(MODE_BORRAR);
					break;
			}
		}break;
		case MODE_LANZAR:
		{
			if (field==0)//Aceptar
			{
				char* value=pActual.GetValueField(2);
				uint8_t programa= atoi(value);
				if (programa>0 && programa<PROGRAMAS_SIZE)
				{
					//Lanzar programa...
					LanzaRiego(programa-1);
				}
			}
			setPantallaActual(CONSULTA_PANTALLA);
		}break;
		case MODE_PARAR:
		{
			if (field==0)//Aceptar
			{
				//Limpiamos los riegos
				memset(riegos,0,sizeof(Riego)*PROGRAMAS_SIZE);
				//Deshabilitamos todas salidas
				for(uint8_t i=0;i<SECTORES_COUNT;i++)
				{
					digitalWrite(PIN_INIT_SECTORES+i, LOW);
				}
			}

			SetMode(MODE_MENU);
		}break;
		case MODE_BORRAR:
		{
			if (field==0)//Aceptar
				memset(programas,0,sizeof(Programa)*PROGRAMAS_SIZE);
			SetMode(MODE_MENU);
		}break;
	}

}

bool Manual::SetKey(unsigned char c)
{

  //Tecla cancel
  if (c=='C')
  {
	  if (mode==MODE_MENU)
	  {
		  setPantallaActual(MENU_PANTALLA);
	  }
	  else
		  SetMode(MODE_MENU);

	  return false;
  }
  /*else if (c=='#' && mode==MODE_MENU)
  {
	  //Lanza el programa. ;>
	  //Vuelve al menu ;)
	  setPantallaActual(MENU_PANTALLA);
	  return false;
  }
*/

  return true;
}


