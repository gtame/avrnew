/*
 * LongTextBase.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "LongTextBase.h"
#ifdef PANTALLA_TECLADO


LongTextBaseScreen::LongTextBaseScreen() {
	// TODO Auto-generated constructor stub

}

void LongTextBaseScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen

	screenManager.AddFields(0,1,20,AlfaTextEdit);//Sector
	screenManager.AddFields(0,2,20,AlfaTextEdit);//Sector



	screenManager.AddFields(3,3,3,Button,"OK");//Sabado
	screenManager.AddFields(13,3,6,Button,"CANCEL");//Domingo

	//Inicializa campos de dia de la semana
	char * valor=GetValue();
	strcpy(screenManager.strings,valor);

	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);


}

void  LongTextBaseScreen::OnSave (){}
void  LongTextBaseScreen::OnGotoMenu(){}


void LongTextBaseScreen::OnFieldChange(uint8_t field)
{

}

void LongTextBaseScreen::OnDrawFields()
{
	//Dibujamos en la primera fila el Title
	screenManager.m_lcd->setCursor(0,0);
	screenManager.m_lcd->print(GetTitle());
}

char* LongTextBaseScreen::GetTitle()
{
	return NULL;
}

bool LongTextBaseScreen::SetKey(unsigned char c)
{

  //Tecla eliminar campo

  //Tecla cancel
  if (c=='C')
  {
	  if (screenManager.GetCursorPos()==3)
	  {

		  OnGotoMenu();

	  }
	  else
		  screenManager.SetFocus(3,0);
  }



  return true;

}



void LongTextBaseScreen::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field==2)//OK
	{
		OnSave();
	}
	else if (field==3) //CAncel
	{
		OnGotoMenu();
	}
}

#endif
