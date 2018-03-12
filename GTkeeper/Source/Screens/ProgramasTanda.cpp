/*
 * ProgramasTanda.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "ProgramasTanda.h"
#ifdef PANTALLA_TECLADO

ProgramasTandaScreen programastandaScreen;

ProgramasTandaScreen::ProgramasTandaScreen() {
	// TODO Auto-generated constructor stub

}

void ProgramasTandaScreen::OnEnter()
{
	screenManager.SetTextMode();

	//Addfields in screen
	//LINEA0
	screenManager.AddFields(13,0,2,TextEdit,1,MAX_PROGRAMAS);//Programa Inicial


	//LINEA1
	screenManager.AddFields(7,1,2,TextEdit,1,PORTS_NUM);//Sector desde
	screenManager.AddFields(16,1,2,TextEdit,1,PORTS_NUM);//Sector Hasta


	//LINEA2
	//Segunda Linea A Tiempo de reigo y Abono
	screenManager.AddFields(1,2,2,TextEdit,0,99);//Tiempo de riego en Horas
	screenManager.AddFields(4,2,2,TextEdit,0,59);//Tiempo de riego en Minutos

	screenManager.AddFields(9,2,2,TextEdit,0,99);//Tiempo de abono en Horas
	screenManager.AddFields(12,2,2,TextEdit,0,59);//Tiempo de abono en Minutos

	//LINEA3
	screenManager.AddFields(7,3,1,TextEdit,1,9);//Tandas

	screenManager.AddFields(10,3,2,Button,"OK");//Si
	screenManager.AddFields(14,3,6,Button,"CANCEL");//No

	//Inicializa campos de dia de la semana
	_tanda.abonohora=0;
	_tanda.abonomin=0;
	_tanda.program=1;
	_tanda.riegohora=0;
	_tanda.riegomin=0;
	_tanda.sector_desde=1;
	_tanda.sector_hasta=PORTS_NUM;
	_tanda.tanda=1;

	setData();

	//Pongo el cursor sobre el programa inicio
	screenManager.SetFocus(0,0);


}


void ProgramasTandaScreen::OnFieldChange(uint8_t field)
{

	  char* value=screenManager.GetValueField(field);
	  uint8_t valor_num=atoi(value);

	  LOG_DEBUG_ARGS("VAlor de campo %i->%s",field,value);

	  switch(field)
	  {
	   case 0://Programa
	   {

		   if (valor_num>0 && valor_num<=MAX_PROGRAMAS)
		   {
			   _tanda.program=valor_num;
		   }
		   else
			   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
			   LOG_DEBUG_ARGS("Valor de programa incorrecto %i",valor_num);
	   }
	   break;

	   case 1://Sector desde
	   {
	 	   if (valor_num>0 && valor_num<=PORTS_NUM)
	 	   {
	 		   _tanda.sector_desde=valor_num;
	 	   }
	 	   else
	 		   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
	 		   LOG_DEBUG_ARGS("Valor de programa incorrecto %i",valor_num);
	    }
	    break;
	   case 2://Sector desde
	   {
	 	   if (valor_num>0 && valor_num<=PORTS_NUM)
	 	   {
	 		   _tanda.sector_hasta=valor_num;
	 	   }
	 	   else
	 		   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
	 		   LOG_DEBUG_ARGS("Valor de programa incorrecto %i",valor_num);
	    }
	    break;
	    //LINEA2
	    	 	//Segunda Linea A Tiempo de reigo y Abono
	    	 	//Tiempo de riego en Horas
	    	   case 3:
	    	   {
	    		    _tanda.riegohora=valor_num;
	    	   }
	    	    break;
	    	 	//Tiempo de riego en Minutos
	    		    	   case 4:
	    		    	    {
	    		    	    	if (valor_num<60)
	    		    	    		_tanda.riegomin=valor_num;
	    		    	  	   else
	    		    	  		   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
	    		    	  		   LOG_DEBUG_ARGS("Valor de minriego incorrecto %i",valor_num);
	    		    	    }
	    		    	     break;

	    		    	 	//Tiempo de abono en Horas
	    		    	   case 5:
	    		    	     {
	    		    	  	    _tanda.abonohora=valor_num;
	    		    	     }
	    		    	      break;
	    		    	   	//Tiempo de abono en Minutos
	    		    	     case 6:
	    		    	      {
	    		    	    	   if (valor_num<60)
	    		    	    	  		  	  _tanda.abonomin=valor_num;
	    		    	    	   //else
	    		    	    		   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
	    		    	    		   //LOG_DEBUG_ARGS("Valor incorrecto %i",valor_num);
	    		    	      }
	    		    	       break;

	    		    	     case 7:
								   {
									   if (valor_num>0)
										_tanda.tanda=valor_num;
									   else
											   //Comprobar el lado del foco , coger ese caracter y ponerlo al inicio del campo ;)
											   LOG_DEBUG_ARGS("Valor de tanda incorrecto %i",valor_num);
								   }
									break;

	  }

	  setData();
}


void ProgramasTandaScreen::setData()
{
	char* buffer=screenManager.BufferScreen;
	memset(buffer,0,sizeof(SC_BUFFER_SIZE));
	sprintf_P(buffer,PSTR("%02i%02i%02i%02i%02i%02i%02i%01i"), _tanda.program, _tanda.sector_desde, _tanda.sector_hasta, _tanda.riegohora,_tanda.riegomin,_tanda.abonohora,_tanda.abonomin,_tanda.tanda);
 	strncpy(screenManager.strings,buffer,strlen(buffer));
}


void ProgramasTandaScreen::OnDrawFields()
{

	screenManager.m_lcd->setCursor(0,0);
	screenManager.m_lcd->print("PROGRAMA INI");

	screenManager.m_lcd->setCursor(0,1);
	screenManager.m_lcd->print("SECTOR");

	screenManager.m_lcd->setCursor(10,1);
	screenManager.m_lcd->print("HASTA");

	screenManager.m_lcd->setCursor(0,2);
	screenManager.m_lcd->print('R');

	screenManager.m_lcd->setCursor(3,2);
	screenManager.m_lcd->print(':');

	screenManager.m_lcd->setCursor(8,2);
	screenManager.m_lcd->print('A');

	screenManager.m_lcd->setCursor(11,2);
	screenManager.m_lcd->print(':');

	screenManager.m_lcd->setCursor(0,3);
	screenManager.m_lcd->print("TANDAS");


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
bool ProgramasTandaScreen::SetKey(unsigned char c)
{

	if (c=='C')
	  {
		if (screenManager.GetCursorPos()==9)
			GotoMenu(0,0);
		else
		  screenManager.SetFocus(9,0);

	  }
	else
		  return true;//Procesamiento por default

}

void  ProgramasTandaScreen::GotoMenu( uint8_t result,uint8_t codefunc )
{
	screenManager.SetPantallaActual((ScreenBase *)&programasmenuScreen);
}

void ProgramasTandaScreen::OnClickButton(uint8_t field)
{

	if (field==8) //Ok
	{
			//Hacemos el proceso
			LOG_DEBUG("Programando tanda");


			uint8_t program=_tanda.program;
			//Lo haremos tantas tandas como haya indicado
			for (uint8_t tanda=1;tanda<=_tanda.tanda;tanda ++)
			{
				for (uint8_t sector=_tanda.sector_desde;sector<=_tanda.sector_hasta;sector++ )
				{
					if(
					   (sector>0 && sector<=PORTS_NUM) &&
					   (program>0 && program<=MAX_PROGRAMAS)
					)
					{


						gtKeeper.ResetPrograma(&gtKeeper.programas[program-1]);

						gtKeeper.programas[program-1].Sector=sector;
						//Si no es el primer programa de la primera tanda ponemos el programa anterior
						if (program!=_tanda.program)
						{
							gtKeeper.programas[program-1].HoraInicio=88;
							gtKeeper.programas[program-1].MinutoInicio=program-1;
						}

						gtKeeper.programas[program-1].TiempoRiego= (_tanda.riegohora*SECS_PER_HOUR)+(_tanda.riegomin*SECS_PER_MIN);
						gtKeeper.programas[program-1].TiempoAbono=(_tanda.abonohora*SECS_PER_HOUR)+(_tanda.abonomin*SECS_PER_MIN);

						gtKeeper.GrabarProgramaAEEPROM(program-1,&gtKeeper.programas[program-1]);
						program++;
					}

				}
			}

			sprintf_P(screenManager.BufferScreen, PSTR("Programada tanda ->\n%i nuevos Riegos!"),program-_tanda.program);
			//Mostramos msg de info
			screenManager.ShowMsgBox(screenManager.BufferScreen,MsgOkButton,GotoMenu);
	}
	else if (field==9 )//Cancel
		GotoMenu(0,0);

}


#endif
