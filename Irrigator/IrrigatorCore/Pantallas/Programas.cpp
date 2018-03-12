/*
 * Programas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "Programas.h"

Programas::Programas() {
	// TODO Auto-generated constructor stub
}

void Programas::OnEnter()
{
	pActual.SetTextMode();

	//Addfields in screen
	pActual.AddFields(1,0,2,TextEdit);//ProgramId
	pActual.AddFields(5,0,2,TextEdit);//Sector

	pActual.AddFields(8,0,1,Button);//Lunes
	pActual.AddFields(9,0,1,Button);//Martes
	pActual.AddFields(10,0,1,Button);//Miercoles
	pActual.AddFields(11,0,1,Button);//Jueves
	pActual.AddFields(12,0,1,Button);//Viernes
	pActual.AddFields(13,0,1,Button);//Sabado
	pActual.AddFields(14,0,1,Button);//Domingo

	pActual.AddFields(1,1,2,TextEdit);//Hora
	pActual.AddFields(4,1,2,TextEdit);//Minuto

	pActual.AddFields(9,1,2,TextEdit);//Tiempo de riego en Horas
	pActual.AddFields(12,1,2,TextEdit);//Tiempo de riego en Minutos


	//pActual.AddFields(15,1,1,Button,OK_TEXT);//Salir


	//Inicializa campos de dia de la semana
	currentprograma=-1;
	setPrograma(0);

	//Pongo el cursor sobre el programa
	pActual.SetFocus(0,0);


}
void Programas::OnFieldChange(uint8_t field)
{

  char* value=pActual.GetValueField(field);
  switch(field)
  {
   case 0://Programa
   {

	   //Goto... otro programa
   }
   break;
   case 1://Sector
   {

	   memcpy(programas[currentprograma].Sector,value,2);
   }
   break;
   case 9://Sector
   {

	   memcpy(programas[currentprograma].HoraInicio,value,2);
   }
   break;
   case 10://Sector
   {

	   memcpy(programas[currentprograma].MinutoInicio,value,2);
   }
   break;
   case 11://Sector
   {

	   memcpy(programas[currentprograma].HoraRiego,value,2);
   }
   break;
   case 12://Sector
   {

	   memcpy(programas[currentprograma].MinutoRiego,value,2);
   }
   break;

  }
}


void Programas::setPrograma(int8_t programa)
{

	 if (programa==-1)
		 programa=PROGRAMAS_SIZE-1;
	 else if (programa==PROGRAMAS_SIZE)
		 programa=0;


	if (strlen(programas[programa].Sector)==0)
		strcpy(programas[programa].Sector,"0000000000000");


	//Solo cambiamos el programa.. cuando no coinciden..
	if (programa!=currentprograma)
	{


		currentprograma=programa;

		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"%02i",programa+1);
		pActual.SetTextField(0, buffer);
	}



	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].Sector,2);
	pActual.SetTextField(1,buffer);


	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].DiasSemana,3);
	DiasSemana dias = (DiasSemana) atoi(buffer);

	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"%c%c%c%c%c%c%c",((dias & L) ? 'L' : '_'),((dias & M) ? 'M' : '_'),
			((dias & X) ? 'X' : '_'),((dias & J) ? 'J' : '_'),((dias & V) ? 'V' : '_'),((dias & S) ? 'S' : '_'),((dias & D) ? 'D' : '_')
	);

	pActual.SetTextField(2,buffer);

	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].HoraInicio,2);
	pActual.SetTextField(9,buffer);


	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].MinutoInicio,2);
	pActual.SetTextField(10,buffer);




	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].HoraRiego,2);
	pActual.SetTextField(11,buffer);


	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,programas[programa].MinutoRiego,2);
	pActual.SetTextField(12,buffer);


}


void Programas::OnDrawFields()
{

	pActual.m_lcd->setCursor(0,0);
	pActual.m_lcd->print('P');



	pActual.m_lcd->setCursor(4,0);
	pActual.m_lcd->print('S');


	pActual.m_lcd->setCursor(0,1);
	pActual.m_lcd->print('H');

	pActual.m_lcd->setCursor(3,1);
	pActual.m_lcd->print(':');

	pActual.m_lcd->setCursor(8,1);
	pActual.m_lcd->print('R');

	pActual.m_lcd->setCursor(11,1);
	pActual.m_lcd->print(':');

	setPrograma(currentprograma);

}

bool Programas::SetKey(unsigned char c)
{
	//Tecla subir
  if (c=='A')
  {
	  if (pActual.GetCursorPos()==0)
	  {
		  int8_t programa= currentprograma+1;
		  setPrograma(programa);
	  }
	  else
		  pActual.SetFocus(0,1);
  }
  //Tecla cancel
  else if (c=='C')
  {
	  if (pActual.GetCursorPos()==0)
	  {
		  pActual.m_lcd->clear();
		  pActual.m_lcd->setCursor(0,1);
		  pActual.m_lcd->print("Guardando programas");

		  //Guardar fichero a flash ()??????
		  if (FlashGuardaFichero(PROGRAMAS_FILE, programas[0].Sector,strlen(programas[0].Sector)))
		  {
			  pActual.m_lcd->clear();
			  pActual.m_lcd->setCursor(0,1);
			  pActual.m_lcd->print("Sincronizando...");

			  FtpEnviamosProgramas();
		  }


		  setPantallaActual(MENU_PANTALLA);
	  }
	  else
		  pActual.SetFocus(0,1);
  }

  //Tecla baja
  else if (c=='D')
  {
	  if (pActual.GetCursorPos()==0)
	  {
		  int8_t  programa= currentprograma-1;
		  setPrograma(programa);
	  }
	  else
		  pActual.SetFocus(0,1);
  }

  return true;

}


void Programas::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field>=2 && field<=8)
	{
		memset(buffer,0,sizeof(buffer));
		memcpy(buffer,programas[currentprograma].DiasSemana,3);
		uint8_t dias =  atoi(buffer);

		switch (field)
		{
			case 2: //L
			{
				if ((dias & L))
					dias=(DiasSemana)dias & ~L;
				else
					dias|=L;
			}
			break;
			case 3: //L
			{
				if ((dias & M))
					dias&= ~M;
				else
					dias|=M;
			}
			break;
			case 4: //L
			{
				if ((dias & X))
					dias&= ~X;
				else
					dias|=X;
			}
			break;
			case 5: //L
			{
				if ((dias & J))
					dias&=~J;
				else
					dias|=J;
			}
			break;
			case 6: //L
			{
				if ((dias & V))
						dias&=~V;
					else
						dias|=V;
			}
			break;
			case 7: //L
			{
				if ((dias & S))
						dias&=~S;
					else
						dias|=S;
			}
			break;
			case 8: //L
			{
				if ((dias & D))
						dias&=~D;
					else
						dias|=D;
			}
			break;
		}

		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"%03i",(uint8_t)dias);
		memcpy(programas[currentprograma].DiasSemana,buffer,3);

	}

}


