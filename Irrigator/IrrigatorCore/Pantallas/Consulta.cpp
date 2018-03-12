/*
 * Consulta.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "Consulta.h"

Consulta::Consulta() {


	// TODO Auto-generated constructor stub
}



void Consulta::OnEnter()
{

	pActual.SetOptionMode();
	/*
	pActual.AddFields(0,0,15,Option,TEXT_PROGRAMAS);
	pActual.AddFields(0,1,15,Option,TEXT_MANUAL);
	pActual.AddFields(0,2,15,Option,TEXT_PARAMETROS);
	pActual.AddFields(0,3,15,Option,TEXT_CONSULTAS);
*/

	pActual.needrefresh=true;


}

void Consulta::OnDrawFields()
{


		//char *buffer=this->buffer;
		pActual.m_lcd->setCursor(0,0);

		uint8_t day=weekday();
		//Domingo=1 ... Sabado=7
		switch(day)
			{
				case 1:
					strcpy(buffer,"DOMINGO");
					break;
				case 2:
					strcpy(buffer,"LUNES");
					break;
				case 3:
					strcpy(buffer,"MARTES");
					break;
				case 4:
					strcpy(buffer,"MIERCOLES");
					break;
				case 5:
					strcpy(buffer,"JUEVES");
					break;
				case 6:
					strcpy(buffer,"VIERNES");
					break;
				case 7:
					strcpy(buffer,"SABADO");
					break;


			}
		sprintf(buffer+strlen(buffer), " %02d:%02d",hour(),minute());
		pActual.m_lcd->print(buffer);


		uint8_t linea=1;
		//Ahora chequeamos los riegos que estan activos y calculamos lo que les queda..
		//Ordenados de menor a mayor.??
		for(uint8_t i=0;i<PROGRAMAS_SIZE;i++)
		{
			if (riegos[i].Hasta>0)
			{
				//obtenemos el numero de segundos que faltan para apagarse
				time_t hasta = riegos[i].Hasta -now();
				if (hasta>0)
				{

					//memset(pActual.buffer, 0, sizeof(pActual.buffer));
					//memcpy(pActual.buffer,riego[i].Sector,2);

					memset(buffer, 0, sizeof(buffer));
					pActual.m_lcd->setCursor(0,linea);
					sprintf(buffer, "P%02i ",i+1);
					pActual.m_lcd->print(buffer);

					memset(buffer, 0, sizeof(buffer));
					memcpy(buffer,riegos[i].Sector,2);
					pActual.m_lcd->setCursor(4,linea);
					pActual.m_lcd->print("S");
					pActual.m_lcd->setCursor(5,linea);
					pActual.m_lcd->print(buffer);


					//Añadimos 1 minuto para que no aparezca que le quedan 0. Esto se hace sumando 60 segundos al hasta
					hasta+=SECS_PER_MIN;
					memset(buffer, 0, sizeof(buffer));
					sprintf(buffer,"%02lu:%02lu",numberOfHours(hasta), numberOfMinutes(hasta) );
					pActual.m_lcd->setCursor(8,linea);
					pActual.m_lcd->print(buffer);

					linea++;
				}
			}
		}

		//Si no hay riegos. mostramos info!!
		if (!IsLockedATCommand)
			current_cobertura=Cobertura();

			writeMeter(current_cobertura);
/*
		 pActual.m_lcd->setCursor(19,0);

		 // registro el nuevo caracter
		 pActual.m_lcd->createChar(0, customChar);

		  // escribo el nuevo caracter en pantalla
		 pActual.m_lcd->write(0);*/
}


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

void Consulta::writeMeter( int  level ) {

	Serial.print("Cobertura->");
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"Cobertura->%i",level);
	Serial.println(buffer);

	pActual.m_lcd->createChar(1, meter_soff);

	pActual.m_lcd->createChar(2, meter_s0);

	pActual.m_lcd->setCursor(19,0);
	pActual.m_lcd->write( 1);
	pActual.m_lcd->setCursor(18,0);
	pActual.m_lcd->write( 1 );
	pActual.m_lcd->setCursor(17,0);
	pActual.m_lcd->write( 2);





	switch( level ){

	case 5:
		{
			pActual.m_lcd->createChar(5, meter_s5);
		}
	case 4:
		{
			if( level == 4 ) pActual.m_lcd->createChar(5, meter_s4);
			pActual.m_lcd->setCursor(19,0);
			pActual.m_lcd->write( 5);
		}
	case 3:
		{
			pActual.m_lcd->createChar(4, meter_s3);

		}
	case 2:
		{
			if( level == 2 ) pActual.m_lcd->createChar(4, meter_s2);
			pActual.m_lcd->setCursor(18,0);
			pActual.m_lcd->write( 4 );
		}
	case 1:
	{
			pActual.m_lcd->createChar(3,  meter_s1);
			pActual.m_lcd->setCursor(17,0);
			pActual.m_lcd->write( 3 );
	}break;

	//En caso que no haya cobertura
	/*
	case 0:
	case -1:
	{
		pActual.m_lcd->setCursor(15,0);
		pActual.m_lcd->print("----");
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
		  setPantallaActual(MENU_PANTALLA);
		  return false;
	  }

	  return true;
}

void Consulta::OnClickButton(uint8_t field)
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
			//setPantallaActual(PARAMETROSMENU_PANTALLA);
			break;
		case 3: //Consulta
			//
				//Gprs.AT();
				break;

	}

}




