/*
 * Consulta.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Consulta.h"

#ifdef PANTALLA_TECLADO
Consulta consultaScreen;

byte meter_soff[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B11011
};

byte meter_s0[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00011
};
byte meter_s1[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00011,
B00011
};
byte meter_s2[8] = {
B00000,
B00000,
B00000,
B00000,
B11000,
B11000,
B11000,
B11011,
};
byte meter_s3[8] = {
B00000,
B00000,
B00000,
B00011,
B11011,
B11011,
B11011,
B11011,
};


byte meter_s4[8] = {
B00000,
B00000,
B11000,
B11000,
B11000,
B11000,
B11000,
B11011,
};

byte meter_s5[8] = {
B00011,
B00011,
B11011,
B11011,
B11011,
B11011,
B11011,
B11011,
};


Consulta::Consulta() {


	_lastrefresh=0;
}



void Consulta::OnEnter()
{

}


bool Consulta::NeedRefresh()
{
	//Si desde el screenmanager se requiere
	if (screenManager.needrefresh)
		return true;
	//Si han pasado 10 segundos para mostrar cobertura
#ifndef PROTEUS
	else if (ELAPSED_SECONDS(_lastrefresh)>15)
		return true;
#endif
	//Si el minuto ha cambiado
	else if (minute(now())!=minute(_lastrefresh))
		return true;
	//El numero de riegos activos ha cambiado
	else if (num_activos!=gtKeeper.GetSalidasActivas())
		return true;
	else
		return false;

}

void Consulta::OnDrawFields()
{

		char* buffer=screenManager.BufferScreen;
		memset(buffer,0,SC_BUFFER_SIZE);

		_lastrefresh=now();

		//char *buffer=this->buffer;
		screenManager.m_lcd->setCursor(0,0);

		uint8_t day=weekday();
		//Domingo=1 ... Sabado=7
		switch(day)
			{
				case 1:
					strcpy_P(buffer,PSTR("DOMINGO"));
					break;
				case 2:
					strcpy_P(buffer,PSTR("LUNES"));
					break;
				case 3:
					strcpy_P(buffer,PSTR("MARTES"));
					break;
				case 4:
					strcpy_P(buffer,PSTR("MIERCOLES"));
					break;
				case 5:
					strcpy_P(buffer,PSTR("JUEVES"));
					break;
				case 6:
					strcpy_P(buffer,PSTR("VIERNES"));
					break;
				case 7:
					strcpy_P(buffer,PSTR("SABADO"));
					break;


			}

		sprintf(buffer+strlen(buffer)," %02d:%02d",hour(),minute());


		screenManager.m_lcd->print(buffer);

//
//		//uint8_t enejecucion=0;
//		uint8_t linea=1;
//		//Ahora chequeamos los riegos que estan activos y calculamos lo que les queda..
//		//Ordenados de menor a mayor.??
//		for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
//		{
//			if (gtKeeper.programas[i].Hasta>0)
//			{
//				//obtenemos el numero de segundos que faltan para apagarse
//				time_t hasta = gtKeeper.programas[i].Hasta -now();
//				if (hasta>0)
//				{
//
//					memset(buffer, 0, SC_MAX_TEXT_FIELDS);
//					//memcpy(screenManager.buffer,riego[i].Sector,2);
///*
//					enejecucion++;
//					if ((enejecucion % 2)==1)
//					{
//						screenManager.m_lcd->setCursor(linea,0);
//					}
//					else
//					{
//						linea++;
//						screenManager.m_lcd->setCursor(linea,0);
//
//					}*/
//					screenManager.m_lcd->setCursor(0,linea);
//					snprintf_P(buffer,SC_MAX_TEXT_FIELDS,PSTR("P%02i S%02i %02lu:%02lu \0"),i+1,gtKeeper.programas[i].Sector,numberOfHours(hasta), numberOfMinutes(hasta) );
//					screenManager.m_lcd->print(buffer);
//					linea++;
//					//Añadimos 1 minuto para que no aparezca que le quedan 0. Esto se hace sumando 60 segundos al hasta
//					//hasta+=SECS_PER_MIN;
//
//
//
//				}
//			}
//		}
//


		num_activos=gtKeeper.GetSalidasActivas();
		if(num_activos==0)
			screenManager.m_lcd->setCursor(0,2);
		else
			screenManager.m_lcd->setCursor(0,1);

		snprintf_P(buffer,21,PSTR("==== %i ACTIVOS ====="),num_activos);
		screenManager.m_lcd->print(buffer);

		if (num_activos>0)
		{
			memset(buffer,0,SC_BUFFER_SIZE);
			uint num_linea=2;
			//Dibujamos en las 2 lineas que nos quedan (2-3) aquello que esta en ejecución
			for (uint8_t i=0;i<num_activos;i++)
			{

				switch(gtKeeper.salidas[i].Tipo)
				{

				case	actPrograma:
					sprintf_P(buffer +strlen(buffer),PSTR("P%02i S%02i "), gtKeeper.salidas[i].Ident+1,gtKeeper.salidas[i].Sector);
					break;
				case 	actSector:
					sprintf_P(buffer +strlen(buffer),PSTR("S%02i "), gtKeeper.salidas[i].Sector);
					break;
				case actAbono:
					sprintf_P(buffer +strlen(buffer),PSTR("A%02i "), gtKeeper.salidas[i].Ident);
					break;
				case actMotor:
						sprintf_P(buffer +strlen(buffer),PSTR("M%02i "), gtKeeper.salidas[i].Ident);
						break;
				default:

					sprintf_P(buffer +strlen(buffer),PSTR("DEF%02i %i "), gtKeeper.salidas[i].Ident,gtKeeper.salidas[i].Tipo);
				}

				if (strlen(buffer)==20 && num_linea<4)
				{
					screenManager.m_lcd->setCursor(0,num_linea);
					screenManager.m_lcd->print(buffer);
					memset(buffer,0,SC_BUFFER_SIZE);
					num_linea++;
				}

			}
			//Ponemos lo que haya en el buffer.
			if (strlen(buffer)>0 && num_linea<4)
			{
				screenManager.m_lcd->setCursor(0,num_linea);
				screenManager.m_lcd->print(buffer);
			}
		}


		//Mostramos info de la cobertura!!
		if (gtKeeper.IsGSMEnable())
		{
			uint8_t current_cobertura=gtKeeper.GetCobertura();
			writeMeter(current_cobertura);
		}


		screenManager.m_lcd->noCursor();

}




void Consulta::writeMeter( int  level ) {


	screenManager.m_lcd->createChar(1, meter_soff);

	screenManager.m_lcd->createChar(2, meter_s0);

	screenManager.m_lcd->setCursor(19,0);
	screenManager.m_lcd->write( 1);
	screenManager.m_lcd->setCursor(18,0);
	screenManager.m_lcd->write( 1 );
	screenManager.m_lcd->setCursor(17,0);
	screenManager.m_lcd->write( 2);




	switch( level ){

	case 5:
		{
			screenManager.m_lcd->createChar(5, meter_s5);
		}
	case 4:
		{
			if( level == 4 ) screenManager.m_lcd->createChar(5, meter_s4);
			screenManager.m_lcd->setCursor(19,0);
			screenManager.m_lcd->write( 5);
		}
	case 3:
		{
			screenManager.m_lcd->createChar(4, meter_s3);

		}
	case 2:
		{
			if( level == 2 ) screenManager.m_lcd->createChar(4, meter_s2);
			screenManager.m_lcd->setCursor(18,0);
			screenManager.m_lcd->write( 4 );
		}
	case 1:
	{
			screenManager.m_lcd->createChar(3,  meter_s1);
			screenManager.m_lcd->setCursor(17,0);
			screenManager.m_lcd->write( 3 );
	}break;

	//En caso que no haya cobertura
	/*
	case 0:
	case -1:
	{
		screenManager.m_lcd->setCursor(15,0);
		screenManager.m_lcd->print("----");
	} break;
*/
	default: break;
	}
}

bool Consulta::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return false;
}

#endif

