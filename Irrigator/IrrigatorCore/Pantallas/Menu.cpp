/*
 * Menu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Menu.h"

Menu::Menu() {


	// TODO Auto-generated constructor stub
}



void Menu::OnEnter()
{

	pActual.SetOptionMode();

	pActual.AddFields(0,0,15,Option,TEXT_PROGRAMAS);
	pActual.AddFields(0,1,15,Option,TEXT_MANUAL);
	pActual.AddFields(0,2,15,Option,TEXT_PARAMETROS);
	pActual.AddFields(0,3,15,Option,TEXT_CONSULTAS);

	/*
	pActual.AddFields(0,2,15,Option);
	pActual.AddFields(0,3,15,Option);

	sprintf(pActual.fields[0].value,"1-%s",TEXT_MANUAL);
	Serial.println(pActual.fields[0].value);

	Serial.println(pActual.fields[1].value);

	sprintf(pActual.fields[2].value,"3-%s",TEXT_PARAMETROS);
	sprintf(pActual.fields[3].value,"4-%s",TEXT_CONSULTAS);*/

	pActual.needrefresh=true;


}

void Menu::OnClickButton(uint8_t field)
{

	switch (field)
	{
		case 0: //Programas
			setPantallaActual(PROGRAMAS_PANTALLA);
			break;
		case 1: //Manual
			//
			setPantallaActual(MANUALMENU_PANTALLA);
			break;
		case 2: //Parametros
			setPantallaActual(PARAMETROS_PANTALLA);
			break;
		case 3: //Consulta
			//
				setPantallaActual(CONSULTA_PANTALLA);
				break;

	}

}




