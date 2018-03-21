/*
 * ProgramasMenu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "ProgramasMenu.h"
#ifdef PANTALLA_TECLADO


ProgramasMenu::ProgramasMenu() {


	// TODO Auto-generated constructor stub
	MsgboxCallBack=ResetearProgramas;
}



void ProgramasMenu::OnEnter()
{

	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,20,Option,MNU_PRG_TEXT_PROGRAMAR);
	screenManager.AddFields(0,1,20,Option,MNU_PRG_TEXT_PRG_TANDA);
	screenManager.AddFields(0,2,20,Option,MNU_PRG_TEXT_RESET);


	/*
	pActual.AddFields(0,2,15,Option);
	pActual.AddFields(0,3,15,Option);

	sprintf(pActual.fields[0].value,"1-%s",TEXT_MANUAL);
	Serial.println(pActual.fields[0].value);

	Serial.println(pActual.fields[1].value);

	sprintf(pActual.fields[2].value,"3-%s",TEXT_PARAMETROS);
	sprintf(pActual.fields[3].value,"4-%s",TEXT_CONSULTAS);*/

	screenManager.needrefresh=true;
	//MsgboxCallBack=(void (*)( uint8_t result,uint8_t codefunc ))PararRiegos;

}


bool ProgramasMenu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return true;
}

 void ProgramasMenu::ResetearProgramas( uint8_t result,uint8_t codefunc )
{
	 if (result==0)
	 {
		 Riego.ResetProgramas();

	 }
}


void ProgramasMenu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //Programas
		{
			screenManager.SetPantallaActual((ScreenBase *)&programasScreen);
		}
			break;
		case 1: //Programas
		{
			//
			//
			//screenManager.SetPantallaActual(PROGRAMAS_PANTALLA);
			screenManager.SetPantallaActual((ScreenBase *)&programastandaScreen);
		}
			break;
		case 2: //Parametros
			screenManager.ShowMsgBox("DESEA RESETEAR TODOS\n RIEGOS PROGRAMADOS?",MsgYesNoButton,MsgboxCallBack);
			break;
		case 3: //Consulta
			//
			//screenManager.SetPantallaActual(PROGRAMAS_PANTALLA);
			 break;

	}

}


//Constructor de menuScreen
ProgramasMenu programasmenuScreen;
#endif
