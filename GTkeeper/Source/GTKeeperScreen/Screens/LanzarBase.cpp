/*
 * LanzarBase.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "LanzarBase.h"
#ifdef PANTALLA_TECLADO


LanzarBaseScreen::LanzarBaseScreen() {
	// TODO Auto-generated constructor stub

}

void LanzarBaseScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen

	screenManager.AddFields(10,1,FieldLen(),TextEdit,MinValue(),MaxValue());//Sector



	screenManager.AddFields(3,3,3,Button,"YES");//Sabado
	screenManager.AddFields(13,3,2,Button,"NO");//Domingo

	//Inicializa campos de dia de la semana
	currentvalue=1;
	setValor(currentvalue);

	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);


}



void  LanzarBaseScreen::OnSelection (){}
void  LanzarBaseScreen::OnGotoMenu(){}


void LanzarBaseScreen::OnFieldChange(uint8_t field)
{
  uint8_t valor_num=0;
  char* value=screenManager.GetValueField(field);



  LOG_DEBUG_ARGS("VAlor de campo %i->%s",field,value);

  switch(field)
  {
   case 0://Programa
   {

	   //Goto... otro programa
	   valor_num=atoi(value);
	   setValor(valor_num);
   }
   break;
  }
}



void LanzarBaseScreen::setValor(int8_t valor)
{

	 if (valor>0 && valor<=MaxValue())
	 {
		 currentvalue=valor;



	 }
	 else
		 LOG_DEBUG_ARGS("Valor no permitido %i",valor);

		//Solo cambiamos el programa.. cuando no coinciden..
			char* buffer=screenManager.BufferScreen;
			memset(buffer,0,sizeof(SC_BUFFER_SIZE));
			sprintf_P(buffer,PSTR("%02i"),currentvalue);
			strncpy(screenManager.strings,buffer,2);

}





bool LanzarBaseScreen::SetKey(unsigned char c)
{

  //Tecla eliminar campo

  //Tecla cancel
  if (c=='C')
  {
	  if (screenManager.GetCursorPos()==2)
	  {

		  OnGotoMenu();

	  }
	  else
		  screenManager.SetFocus(2,0);
  }



  return true;

}



void LanzarBaseScreen::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field==1)//OK
	{
		OnSelection();
	}
	else if (field==2) //CAncel
	{
		OnGotoMenu();
	}
}

#endif

