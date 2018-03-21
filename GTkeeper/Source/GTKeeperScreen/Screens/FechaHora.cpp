/*
 * FechaHora.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "FechaHora.h"
#ifdef PANTALLA_TECLADO
FechaHoraScreen fechahoraScreen;

FechaHoraScreen::FechaHoraScreen() {
	// TODO Auto-generated constructor stub

}

void FechaHoraScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen

	//Fecha
	screenManager.AddFields(8,1,2,TextEdit,1,31); //Dia
	screenManager.AddFields(11,1,2,TextEdit,1,12); //Mes
	screenManager.AddFields(14,1,2,TextEdit,1,99);// Año

	//Hora

	//LINEA2
	//Hora
	screenManager.AddFields(8,2,2,TextEdit,0,23);//Tiempo de riego en Horas
	screenManager.AddFields(11,2,2,TextEdit,0,59);//Tiempo de riego en Minutos







	if (gtKeeper.EstaEnHora())
	{
		screenManager.AddFields(3,3,2,Button,"OK");//Sabado
		screenManager.AddFields(13,3,6,Button,"CANCEL");//Domingo
		fecha=now();
	}
	else
	{

		//Fecha actual
		fecha=0;
		screenManager.AddFields(10,3,2,Button,"OK");//Sabado
	}

	breakTime(fecha,fechaElements);

	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);

	setFecha();
}




void FechaHoraScreen::OnFieldChange(uint8_t field)
{
  uint8_t valor_num=0;
  char* value=screenManager.GetValueField(field);
  valor_num=atoi(value);


   if (field<5)
   {
	   //Chequeamos si esta entre valores MIN Y MAX :)
	   if (screenManager.fields[field].minvalue<=valor_num && screenManager.fields[field].maxvalue>=valor_num )
	   {
		   switch(field)
		   {
			case 0://Dia
				{
					fechaElements.Day=valor_num;
				}
				break;
			case 1://Mes
				{
					fechaElements.Month=valor_num;
				}
				break;
			case 2://año
				{
					fechaElements.Year=y2kYearToTm(valor_num);//Año 2000
				}
				break;
			case 3://hora
				{
					fechaElements.Hour=valor_num;
				}
				break;
			case 4://minuto
				{
					fechaElements.Minute=valor_num;
				}
				break;
		   }
	   }

	   fecha= makeTime(fechaElements);
   }


  setFecha();
}



void FechaHoraScreen::setFecha()
{

	    char* buffer=screenManager.BufferScreen;
	    memset(buffer,0,sizeof(SC_BUFFER_SIZE));
	    buffer=itoa( year(fecha),buffer, 10); // 10 - decimal;
		sprintf_P(buffer+5,PSTR("%02i%02i%s%02i%02i"),day(fecha),month(fecha),buffer+2,hour(fecha),minute(fecha));
		strncpy(screenManager.strings,buffer+5,strlen(buffer+5));
}




void FechaHoraScreen::OnDrawFields()
{

	screenManager.m_lcd->setCursor(1,1);
	 screenManager.m_lcd->print("Fecha:");
	 screenManager.m_lcd->setCursor(10,1);
	 screenManager.m_lcd->print('/');
	 screenManager.m_lcd->setCursor(13,1);
	 screenManager.m_lcd->print('/');

		screenManager.m_lcd->setCursor(1,2);
		 screenManager.m_lcd->print("Hora:");
		 screenManager.m_lcd->setCursor(10,2);
		 screenManager.m_lcd->print(':');

}


bool FechaHoraScreen::SetKey(unsigned char c)
{

  //Tecla eliminar campo

  //Tecla cancel
	if (gtKeeper.EstaEnHora())
	{
		  if (c=='C')
		  {
			  if (screenManager.GetCursorPos()==6)
			  {
				  screenManager.SetPantallaActual((ScreenBase *)&parametrosMenuScreen);

			  }
			  else
				  screenManager.SetFocus(6,0);
		  }
	}

  return true;

}



void FechaHoraScreen::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field==5)//OK
	{

		if (fecha==0)
			screenManager.ShowMsgBox("Debe fijar una \n fecha y hora",MsgOkButton);
		else
		{
			//Fija la hora en arduino
			gtKeeper.SetHora(fecha);
			screenManager.SetPantallaActual((ScreenBase *)&parametrosMenuScreen);
		}
	}
	else if (field==6) //CAncel
	{
		//Al menu
		screenManager.SetPantallaActual((ScreenBase *)&parametrosMenuScreen);
	}
}
#endif