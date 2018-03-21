/*
 * Programas.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "Programas.h"
#ifdef PANTALLA_TECLADO
ProgramasScreen programasScreen;



#include "Programas.h"

ProgramasScreen::ProgramasScreen() {
	// TODO Auto-generated constructor stub
	program=NULL;
	showed=false;
}

void ProgramasScreen::OnEnter()
{
	screenManager.SetTextMode();
	showed=false;
	//Addfields in screen

	//Primera linea RElativo a programa - Sector
	screenManager.AddFields(1,0,2,TextEdit);//ProgramId
	screenManager.fields[0].editmode=Replace;

	screenManager.AddFields(5,0,2,TextEdit);//Sector
	screenManager.AddFields(10,0,2,TextEdit);//Programa Anterior


	//Segunda Linea A Tiempo de reigo y Abono
	screenManager.AddFields(1,1,2,TextEdit);//Tiempo de riego en Horas
	screenManager.AddFields(4,1,2,TextEdit);//Tiempo de riego en Minutos

	screenManager.AddFields(9,1,2,TextEdit);//Tiempo de abono en Horas
	screenManager.AddFields(12,1,2,TextEdit);//Tiempo de abono en Minutos


	//Tercera Linea Relativo a Hora Inicio & Minuto Inicio & Dia
	screenManager.AddFields(1,2,2,TextEdit);//Hora Inicio
	screenManager.AddFields(4,2,2,TextEdit);//Minuto Inicio


	screenManager.AddFields(8,2,1,Button);//Lunes
	screenManager.AddFields(9,2,1,Button);//Martes
	screenManager.AddFields(10,2,1,Button);//Miercoles
	screenManager.AddFields(11,2,1,Button);//Jueves
	screenManager.AddFields(12,2,1,Button);//Viernes
	screenManager.AddFields(13,2,1,Button);//Sabado
	screenManager.AddFields(14,2,1,Button);//Domingo





	//screenManager.AddFields(15,1,1,Button,OK_TEXT);//Salir


	//Inicializa campos de dia de la semana
	currentprograma=-1;
	setPrograma(0);

	//Pongo el cursor sobre el programa
	screenManager.SetFocus(0,0);


}


void ProgramasScreen::SetVisibleFields(bool visible)
{
	screenManager.fields[7].visible=visible;
	screenManager.fields[8].visible=visible;
	screenManager.fields[9].visible=visible;
	screenManager.fields[10].visible=visible;
	screenManager.fields[11].visible=visible;
	screenManager.fields[12].visible=visible;
	screenManager.fields[13].visible=visible;
	screenManager.fields[14].visible=visible;
	screenManager.fields[15].visible=visible;
	ShowHoraInicio(visible);
}


void ProgramasScreen::OnSetFocus(uint8_t* field,uint8_t* selpos)
{
	if (program!=NULL)
	{

		LOG_DEBUG_ARGS("Cmbio foco %i",*field);
		if (program->HoraInicio==88 && (*field>6))
		{
			//Pasamos el foco al campo 0
			*field=0;
		}
		LOG_DEBUG_ARGS("Cmbio foco %i",*field);
	}
}

void ProgramasScreen::OnFieldChange(uint8_t field)
{
  uint8_t valor_num=0;
  char* value=screenManager.GetValueField(field);
  program=&Riego.programas[currentprograma];


  LOG_DEBUG_ARGS("VAlor de campo %i->%s",field,value);

  switch(field)
  {
   case 0://Programa
   {

	   //Goto... otro programa
	   valor_num=atoi(value);
	   if (valor_num>0 && valor_num<=MAX_PROGRAMAS)
	   {
		   setPrograma(valor_num-1);
		   screenManager.SetFocus(0,0);
	   }
	   else
	   {
		   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
		   LOG_DEBUG_ARGS("Valor de programa incorrecto %i",valor_num);
	   }
   }
   break;
   case 1://Sector
   {

	   valor_num=atoi(value);
	   if (valor_num>=0 && valor_num<=PORTS_NUM)
	   {
		   program->Sector=valor_num;
	   }
	   else
		   LOG_DEBUG_ARGS("Valor de sector incorrecto %i",valor_num);




   }
   break;

   case 2://Sector Anterior
   {


	   //Si hora de inicio es 88,, Es que hay sector anterior
	    valor_num=atoi(value);
	    if (valor_num!=0)
	    {

	    	if (valor_num<MAX_PROGRAMAS)
	    	{
		    	program->HoraInicio=88;
		    	program->MinutoInicio=valor_num;
		    	program->Dias=0;

	    	}
	    	else
	    	    LOG_DEBUG_ARGS("Valor de programa incorrecto %i",valor_num);
	    }
	    else
	    	program->HoraInicio=0;
   }
   break;

   case 7://Hora de inicio
   {
	   valor_num=atoi(value);
	   if (valor_num<24)
	   {
		   if(program->HoraInicio==88)
			   	  program->MinutoInicio=0;
		   program->HoraInicio=atoi(value);
	   }
	   else
		   LOG_DEBUG_ARGS("No procede hora %i",valor_num);
   }
   break;
   case 8://Minuto inicio
   {
	   valor_num=atoi(value);
  	   if (valor_num<60)
  	   {
  		   if (program->HoraInicio==88)
  			   program->HoraInicio=0;

  		   program->MinutoInicio=valor_num;
  	   }
  	   else
  		   LOG_DEBUG_ARGS("No procede minutos %i",valor_num);
   }
   break;
   case 3://Tiempo Riego
   case 4:
   {
	   //Horas + minutos, el  valor es en seg. ;)

	   value=screenManager.GetValueField(4);
	   valor_num=atoi(value);

	   if (valor_num<60)
	   {

		   program->TiempoRiego =(valor_num*SECS_PER_MIN);

		   value=screenManager.GetValueField(3);
		   program->TiempoRiego+=(atoi(value) *SECS_PER_HOUR);


	   }
	   else
		   LOG_DEBUG_ARGS("No procede minutos %i",valor_num);

		   LOG_DEBUG_ARGS("Abono ->%lu",program->TiempoRiego);
   }
   break;
   case 5://Tiempo abono
   case 6:
   {
	   //Horas + minutos, el  valor es en seg. ;)

	   	   value=screenManager.GetValueField(6);
	   	valor_num=atoi(value);

	  	   if (valor_num<60)
	  	   {
	  		   program->TiempoAbono =(valor_num*SECS_PER_MIN);
	  		   value=screenManager.GetValueField(5);
	  		   program->TiempoAbono +=(atoi(value) *SECS_PER_HOUR);

		   }
		   else
			   LOG_DEBUG_ARGS("No procede minutos %i",valor_num);

	   LOG_DEBUG_ARGS("Abono ->%lu",program->TiempoAbono);
   }
   break;

  }


  Riego.GrabarProgramaAEEPROM(currentprograma);
}



void ProgramasScreen::setPrograma(int8_t programa)
{

	 if (programa==-1)
		 programa=MAX_PROGRAMAS-1;
	 else if (programa==MAX_PROGRAMAS)
		 programa=0;



	 char* buffer=screenManager.BufferScreen;

	//Solo cambiamos el programa.. cuando no coinciden..
	if (programa!=currentprograma)
		currentprograma=programa;

	//Apuntamos al programa actual
	program=&Riego.programas[currentprograma];


	//Pasamos al buffer de la screen los datos ;)
	DiasSemana dias = (DiasSemana) program->Dias;


	memset(buffer,0,sizeof(SC_BUFFER_SIZE));
	Riego.ProgramaToDisplay(currentprograma,buffer);
	LOG_DEBUG_ARGS("Programa-> %s",buffer);


	memset(buffer,0,sizeof(SC_BUFFER_SIZE));

	if (program->HoraInicio==88)
	{
		sprintf_P(buffer,PSTR("%02i%02i%02i%02lu%02lu%02lu%02lu____*******"),
				currentprograma+1,
				program->Sector,
				(program->HoraInicio==88?program->MinutoInicio:0),
				NUMERO_HORAS(program->TiempoRiego),
				NUMERO_MINUTOS(program->TiempoRiego),
				NUMERO_HORAS(program->TiempoAbono),
				NUMERO_MINUTOS(program->TiempoAbono)
				);
	}
	else
	{
		sprintf_P(buffer,PSTR("%02i%02i%02i%02lu%02lu%02lu%02lu%02i%02i%c%c%c%c%c%c%c"),
		currentprograma+1,
		program->Sector,
		(program->HoraInicio==88?program->MinutoInicio:0),
		NUMERO_HORAS(program->TiempoRiego),
		NUMERO_MINUTOS(program->TiempoRiego),
		NUMERO_HORAS(program->TiempoAbono),
		NUMERO_MINUTOS(program->TiempoAbono),
		(program->HoraInicio==88?0:program->HoraInicio),
		(program->HoraInicio==88?0:program->MinutoInicio),
		((dias & L) ? 'L' : '_'),
		((dias & M) ? 'M' : '_'),
		((dias & X) ? 'X' : '_'),
		((dias & J) ? 'J' : '_'),
		((dias & V) ? 'V' : '_'),
		((dias & S) ? 'S' : '_'),
		((dias & D) ? 'D' : '_')

		);
	}
	LOG_DEBUG_ARGS("Buffer->%s",buffer);

	SetVisibleFields(program->HoraInicio!=88);

	strcpy(screenManager.strings,buffer);


}




void ProgramasScreen::OnDrawFields()
{


	screenManager.m_lcd->setCursor(0,0);
	screenManager.m_lcd->print('P');



	screenManager.m_lcd->setCursor(4,0);
	screenManager.m_lcd->print('S');

	screenManager.m_lcd->setCursor(8,0);
	screenManager.m_lcd->print('P');
	screenManager.m_lcd->setCursor(9,0);
	screenManager.m_lcd->print('A');

	screenManager.m_lcd->setCursor(0,1);
	screenManager.m_lcd->print('R');

	screenManager.m_lcd->setCursor(3,1);
	screenManager.m_lcd->print(':');

	screenManager.m_lcd->setCursor(8,1);
	screenManager.m_lcd->print('A');

	screenManager.m_lcd->setCursor(11,1);
	screenManager.m_lcd->print(':');


	ShowHoraInicio(program->HoraInicio!=88);

	setPrograma(currentprograma);

}


void ProgramasScreen::ShowHoraInicio(bool show)
{
		if (show)
		{

			if(!showed)
			{
			screenManager.m_lcd->setCursor(0,2);
			screenManager.m_lcd->print('H');

			screenManager.m_lcd->setCursor(3,2);
			screenManager.m_lcd->print(':');
			showed=true;
			}
		}
		else
		{
			if (showed)
			{
			screenManager.m_lcd->setCursor(0,2);
			screenManager.m_lcd->print(' ');

			screenManager.m_lcd->setCursor(3,2);
			screenManager.m_lcd->print(' ');
			showed=false;
			}
		}
}

bool ProgramasScreen::SetKey(unsigned char c)
{
	//Tecla subir
  if (c=='A')
  {
	  if (screenManager.GetCursorPos()==0)
	  {
		  int8_t programa= currentprograma+1;
		  setPrograma(programa);
		  screenManager.SetFocus(0,0);
	  }
	  else
		  screenManager.SetFocus(0,0);
  }
  //Tecla eliminar campo
  else if (c=='B')
  {
	  LOG_DEBUG("Tecla B!!");
	  uint8_t field=screenManager.GetCursorPos();
	  switch (field)
	  {

	  	  	  case 1://Sector
			  {
				  	  program->Sector=0;
			  }
			  break;
			  case 2://Sector anterior
			  {
				  	  program->HoraInicio=0;
				  	  program->MinutoInicio=0;

			  }
			  break;


		  case 3:
		  {
			  	  //Dejamos solo el numero de minutos
			  	  program->TiempoRiego=NUMERO_MINUTOS(program->TiempoRiego)*SECS_PER_MIN;
		  }
		  break;
		  case 4:
		  {
			  	  	 //Dejamos solo el numero de horas
			  		program->TiempoRiego=NUMERO_HORAS(program->TiempoRiego)*SECS_PER_HOUR;
		  }
		  break;
		  case 5:
		  {
				  //Dejamos solo el numero de minutos
				  program->TiempoAbono=NUMERO_MINUTOS(program->TiempoAbono)*SECS_PER_MIN;
		  }
		  break;
		  case 6:
		  {
					 //Dejamos solo el numero de horas
					program->TiempoAbono=NUMERO_HORAS(program->TiempoAbono)*SECS_PER_HOUR;
		  }
		  break;
		  case 7:
		  {
			  	  program->HoraInicio=0;
		  }
		  break;
		  case 8:
			  {
				  program->MinutoInicio=0;
			  }
			 break;
		  	  //Si lo hacemos sobre un dia de la semana reseteamos todos
			  case 9:case 10:case 11:case 12:case 13:case 14:case 15:
			  {
				  	  program->Dias=0;
			  }
		  	break;

	  }

  }
  //Tecla cancel
  else if (c=='C')
  {
	  if (screenManager.GetCursorPos()==0)
	  {
		  /*
		  screenManager.m_lcd->clear();
		  screenManager.m_lcd->setCursor(0,1);
		  screenManager.m_lcd->print("Guardando programas");

		  //Guardar fichero a flash ()??????
		  if (FlashGuardaFichero(PROGRAMAS_FILE, programas[0].Sector,strlen(programas[0].Sector)))
		  {
			  screenManager.m_lcd->clear();
			  screenManager.m_lcd->setCursor(0,1);
			  screenManager.m_lcd->print("Sincronizando...");

			  FtpEnviamosProgramas();
		  }
		  */

		  screenManager.SetPantallaActual((ScreenBase *)&programasmenuScreen);
	  }
	  else
		  screenManager.SetFocus(0,0);
  }

  //Tecla baja
  else if (c=='D')
  {
	  if (screenManager.GetCursorPos()==0)
	  {
		  int8_t  programa= currentprograma-1;
		  setPrograma(programa);
		  screenManager.SetFocus(0,0);
	  }
	  else
		  screenManager.SetFocus(0,0);
  }

  return true;

}


void ProgramasScreen::OnPaint()
{
	showed=false;
}

void ProgramasScreen::OnClickButton(uint8_t field)
{


	//Campos correspondientes a los dias de la semana
	if (field>=9 && field<=15 && program->HoraInicio!=88)
	{

		uint8_t dias =  Riego.programas[currentprograma].Dias;
		switch (field)
		{


			case 9: //L
			{
				if ((dias & L))
					dias=(DiasSemana)dias & ~L;
				else
					dias|=L;
			}
			break;
			case 10: //L
			{
				if ((dias & M))
					dias&= ~M;
				else
					dias|=M;
			}
			break;
			case 11: //L
			{
				if ((dias & X))
					dias&= ~X;
				else
					dias|=X;
			}
			break;
			case 12: //L
			{
				if ((dias & J))
					dias&=~J;
				else
					dias|=J;
			}
			break;
			case 13: //L
			{
				if ((dias & V))
						dias&=~V;
					else
						dias|=V;
			}
			break;
			case 14: //L
			{
				if ((dias & S))
						dias&=~S;
					else
						dias|=S;
			}
			break;
			case 15: //L
			{
				if ((dias & D))
						dias&=~D;
					else
						dias|=D;
			}
			break;
		}


		//ACtualiza el valor
		Riego.programas[currentprograma].Dias=dias;

		Riego.GrabarProgramaAEEPROM(currentprograma);

		setPrograma(currentprograma);
	}

}

#endif
