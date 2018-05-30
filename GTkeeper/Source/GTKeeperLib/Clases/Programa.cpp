/* 
* Programa.cpp
*
* Created: 18/03/2018 15:58:30
* Author: Gabi
*/


#include "Programa.h"

tPrograma programas[MAX_PROGRAMAS];

// default constructor
Programa::Programa(  char * ibuffer,uint8_t isizebuffer)
{ 
	internalbuffer=ibuffer;
	sizebuffer=isizebuffer;
	changed=false;
} //Programa

//03101010120001200000
//03->Sector
//0101010-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Ejecucion * Hora que se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
bool Programa::CargaProgramaDesdeString(uint8_t progIndex,char *progstr)
{
	char buffnum[4];
	//Validaciones
	//if (progstr==internalbuffer)
		//return false;
	
	if (strlen(progstr)!=LEN_PROGRAMA_STRING)
		return false;
		
		
	if (!isValidNumber(progstr))
		return false;
		
		
	tPrograma* programa=&programas[progIndex];

	LOG_DEBUG_ARGS("CArga desde string %s",progstr);

	//01127125000010000
 
	memset(buffnum,0,4);
	strncpy (buffnum,progstr,2);
	programa->Sector=atoi(buffnum);


	//Ejecucion de progstr -- Hora de inicio o sector
	memset(buffnum,0,4);
	strncpy(buffnum,progstr+2,3);
	programa->Dias= atoi(buffnum);

	//Ejecucion de progstr -- Hora de inicio o sector
	memset(buffnum,0,4);
	strncpy(buffnum,progstr+5,2);
	programa->HoraInicio= atoi(buffnum);

	memset(buffnum,0,4);
	strncpy(buffnum,progstr+7,2);
	programa->MinutoInicio=atoi(buffnum);

		
	//Tiempo riego
	memset(buffnum,0,4);
	strncpy(buffnum,progstr+9,2);
	programa->TiempoRiego= atoi(buffnum) * 60;

	memset(buffnum,0,4);
	strncpy(buffnum,progstr+11,2);
	programa->TiempoRiego+=atoi(buffnum) ;


	//Tiempo abono
	memset(buffnum,0,4);
	strncpy(buffnum,progstr+13,2);
	programa->TiempoAbono=atoi(buffnum) * 60; //Tiempo de abono (horas a minutos)

	memset(buffnum,0,4);
	strncpy(buffnum,progstr+15,2);
	programa->TiempoAbono+=atoi(buffnum) ; //Tiempo de abono

	//Cambiamos el flag para saber que cambio el programa
	SetChangedProgramas(true);

	return true;
 

}

void Programa::GuardarProgramasEEPROM()
{
	for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
		GrabarProgramaAEEPROM(i);

	changed=false;
}


bool Programa::GrabarProgramaAEEPROM(uint8_t progIndex)
{
	
	if (progIndex>=MAX_PROGRAMAS)
		return false;
		
	
	 tPrograma progEeprom;
     CargarProgramaDesdeEEPROM (progIndex, &progEeprom);

	//La lectura de la eeprom es mucha mas rapida que la escritura, ademas el nº de escrituras en una eeprom es limitado
	//Por lo que comprobaremos antes de escribir que ha cambiado (memcmp)
	if (memcmp((void*)&programas[progIndex],(void *)&progEeprom,sizeof(tPrograma)) !=0)
	{

		uint16_t address=GET_ADDRES_PROGRAM(progIndex);
		//Cargamos la configuracion
		while (!eeprom_is_ready());
		eeprom_write_block((void*)&programas[progIndex], ( void*) address, sizeof(tPrograma));
	}
	 
	
	//Quiza hay que implementar un metodo para check - Ver metodo EPPROMCargaConfig
	return true;
	/*
	memset(internalbuffer,0, sizebuffer);
	ProgramaToString(progIndex,internalbuffer);
	
	if (strlen(internalbuffer)!=LEN_PROGRAMA_STRING)
	{
		LOG_CRITICAL("GTK-1");
		return false;
	}
	else
	{


		uint16_t ee_addr=GET_ADDRES_PROGRAM(progIndex);

		char *buffer_reader=internalbuffer+(LEN_PROGRAMA_STRING+1);
		//Ahora lo que vamos a hacer es comparar con lo que hay guardado en la eeprom
		eeprom_read_block((void *)buffer_reader,(void *)ee_addr,LEN_PROGRAMA_STRING);

		//Solo guardo cuando sean iguales
		if (strcmp(internalbuffer,buffer_reader)!=0)
		{
			LOG_DEBUG_ARGS("Guardando.. %s",internalbuffer);
			while (!eeprom_is_ready());

			eeprom_write_block((void *)internalbuffer,(void *)ee_addr,LEN_PROGRAMA_STRING);
		}

		return true;
	}*/
}


bool Programa::CargarProgramaDesdeEEPROM(uint8_t progIndex,tPrograma * programa)
{
	if (progIndex>=MAX_PROGRAMAS)
	return false;
	//config.flag_check='\0';//Lo ponemos a false para comprobar que carga bien la config
	ResetPrograma(programa);
	//Cargamos la configuracion
	eeprom_read_block((void*)programa, (const void*) GET_ADDRES_PROGRAM(progIndex), sizeof(tPrograma));
	//return (config.flag_check=='X');
	return true;
}

bool Programa::CargarProgramaDesdeEEPROM(uint8_t progIndex)
{
	return CargarProgramaDesdeEEPROM(progIndex,&programas[progIndex]);
}

void Programa::ResetPrograma(tPrograma *prog)
{
	
	prog->Dias=0;
	prog->Sector=0;
	prog->HoraInicio=0;
	prog->MinutoInicio=0;
	prog->TiempoAbono=0;
	prog->TiempoRiego=0;

}

void Programa::ResetPrograma(uint8_t progIndex)
{
	if (progIndex>=MAX_PROGRAMAS)
	return;
	
	tPrograma* programa=&programas[progIndex];
	ResetPrograma(programa);
	
}

//Podria ser eliminada - solo es usada a modo debug
void Programa::ProgramaToDisplay(uint8_t progIndex, char *text) {

	if (progIndex>=MAX_PROGRAMAS)
		return;
		
	tPrograma* programa=&programas[progIndex];

	if (programa->HoraInicio == 88) {
		sprintf_P(text, PSTR("S%02i SA:%02i R%02i:%02i A%02i:%02i"), programa->Sector,programa->MinutoInicio,NUMERO_HORAS(programa->TiempoRiego),NUMERO_MINUTOS(programa->TiempoRiego),NUMERO_HORAS(programa->TiempoAbono),NUMERO_MINUTOS(programa->TiempoAbono));

		} else {

		DiasSemana dias = (DiasSemana)programa->Dias;

		//Tenemos que ver para que dias de la semana esta configurado que salte. ;>
		char dias_select[8];
		dias_select[0] = ((dias & L) ? 'L' : '_');
		dias_select[1] = ((dias & M) ? 'M' : '_');
		dias_select[2] = ((dias & X) ? 'X' : '_');
		dias_select[3] = ((dias & J) ? 'J' : '_');
		dias_select[4] = ((dias & V) ? 'V' : '_');
		dias_select[5] = ((dias & S) ? 'S' : '_');
		dias_select[6] = ((dias & D) ? 'D' : '_');
		dias_select[7]=0;

		sprintf_P(text, PSTR("S%02i  %s H%02i:%02i R%02i:%02i A%02i:%02i"),
		programa->Sector, dias_select,programa->HoraInicio,programa->MinutoInicio,NUMERO_HORAS(programa->TiempoRiego),NUMERO_MINUTOS(programa->TiempoRiego),NUMERO_HORAS(programa->TiempoAbono),NUMERO_MINUTOS(programa->TiempoAbono));
	}


}

void Programa::ProgramaToString(uint8_t progIndex, char *text) {

	if (progIndex>=MAX_PROGRAMAS)
	return;
	
 tPrograma* programa=&programas[progIndex];

 sprintf_P(text,
 PSTR("%02i%03i%02i%02i%02i%02i%02i%02i"),
 programa->Sector,
 programa->Dias,
 programa->HoraInicio,
 programa->MinutoInicio,
 NUMERO_HORAS(programa->TiempoRiego),
 NUMERO_MINUTOS(programa->TiempoRiego),
 NUMERO_HORAS(programa->TiempoAbono),
 NUMERO_MINUTOS(programa->TiempoAbono));
}

///Adaptacion de la libreria de arduino TimeAlarms.h
time_t Programa::GetNextEjecucion(uint8_t progIndex)
{
		if (progIndex>=MAX_PROGRAMAS)
		return -1;
		
	tPrograma* programa=&programas[progIndex];

	//Si tiene dias programados
	if (programa->Dias!=0)
	{
		time_t ahora=now();
		TimeElements timeAhora;
		time_t proximaejecucion=0;
		breakTime(ahora,timeAhora);
		
		//La enum de time Sunday=1 si es domingo lo mandamos al final de la semana
		//NONE =0,
		//L = (1 << 0),//1
		//M= (1 << 1),//2
		//X= (1 << 2),//4
		//J= (1 << 3),//8
		//V= (1 << 4),//16
		//S= (1 << 5),//32
		//D= (1 << 6)//64
		int day= dayOfWeek2(ahora);
		
		DiasSemana dia=NONE;

		//Recorremos hacia adelante desde el dia actual hasta el domingo del 0(Lunes) al 6(Domingo)
		for (uint8_t i=day ;i<=7;i++)
		{
			dia= dayToDiasSemana(i) ;
			
			//Si tiene este dia miramos horas
			if (dia & programa->Dias)
			{
				if
				(
				//Si es el dia actual...
				//Y tiene programada una hora posterior
				(
				(i==day) &&
				  (
					(programa->HoraInicio>timeAhora.Hour) ||
					//Si es la misma hora el minuto programado tiene que ser mayor , si es el mismo busca la siguiente programacion
					(programa->HoraInicio==timeAhora.Hour && programa->MinutoInicio>timeAhora.Minute)
				  )
				)
				//O Tiene hora programada un dia posterior independientemente del dia
				|| (i>day)
				)
				{
					// Ya tenemos hora ;)
					//HAcemos con la combinacion Dia Hora el time;
					timeAhora.Hour=programa->HoraInicio;
					timeAhora.Minute=programa->MinutoInicio;
					// Y le sumamos los dias que han pasado desde hoy
					proximaejecucion=makeTime(timeAhora) + ((i-day) * SECS_PER_DAY);
					return proximaejecucion;
					
				}
				
			}

		}

		
		//Si no hay proxima ejecucion
		if (proximaejecucion==0)
		{
			//Recorremos hacia adelante... hasta el dia de hoy
			for (uint8_t i=1 ;i<=day;i++)
			{
				DiasSemana dia= dayToDiasSemana(i) ;
				//Si tiene este dia miramos horas
				if (dia & programa->Dias)
				{
					//El que tenga programacion es la primera no hace falta seguir buscando
					// Ya tenemos hora ;)

					//Sumaremos al tiempo obtenido lo que nos queda de semana hasta el domingo 23:59
					proximaejecucion =makeTime(timeAhora) + ((i-1)* SECS_PER_DAY) + (SECS_PER_WEEK - elapsedSecsThisWeek2(ahora))+
					//Ahora estamos en el dia a las 00:00 , le ponemos la hora //minuto correcto
					(programa->HoraInicio * SECS_PER_HOUR )+ (programa->MinutoInicio * SECS_PER_MIN);

					return proximaejecucion;
				}
			}
		}
		


		return proximaejecucion;
	}
	else
	return 0;
	
}

bool Programa::EEPROMCargaProgramas()
{
 
		
	bool bresult=true;
	for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
	{
		if (CargarProgramaDesdeEEPROM(program))
		{

			memset(internalbuffer,0,sizebuffer);
			ProgramaToDisplay(program,internalbuffer);
			LOG_DEBUG_ARGS("%s",internalbuffer);

		}
		else
		bresult=false;//LOG_ERROR_ARGS("Cargando programa %i",program);
	}


	return bresult;
}

void Programa::ResetProgramas()
{
	for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
	{
		ResetPrograma(program);
		GrabarProgramaAEEPROM(program);
	}
}

void Programa::ShowInfoProgramas()
{
	for (uint8_t program=0;program<MAX_PROGRAMAS;program++)
	{
		memset(internalbuffer,0,sizebuffer);
		ProgramaToDisplay(program,internalbuffer);
		LOG_DEBUG_ARGS("%i->%s",program,internalbuffer);
	}
}