////
////*
////* getnextEjecucion.cpp
////*
////* Created: 17/03/2018 15:50:58
////*  Author: Gabi
////*
////*/ 


#include <ArduinoUnit.h>
#include <main.h>

//03101010120001200000
//03->Sector
//127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Ejecucion * Hora que se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
char programa[]="03127120001200000";

void printTime(time_t hora)
{
	TimeElements timeEl;
	breakTime(hora,timeEl);
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	LOG_INFO_ARGS("HORA->%02i/%s/%04i %02i:%02i:%02i" ,timeEl.Day, monthStr(timeEl.Month),tmYearToCalendar(timeEl.Year), timeEl.Hour,timeEl.Minute,timeEl.Second);

}
time_t GetTime(int hr,int min,int sec,int dy, int mnth, int yr)
{
 
	//Fijamos hora a 01-01-2018 -LUNES
	TimeElements elements;
	elements.Year=CalendarYrToTm(2018);
	elements.Month=mnth;
	elements.Day=dy;
	elements.Hour=hr;
	elements.Minute=min;
	elements.Second=sec;
	return	makeTime(elements);
}
void PonerHora()
{
	//Fijamos hora a 01-01-2018 -LUNES
	/*TimeElements elements;
	elements.Year=CalendarYrToTm(2018);
	elements.Month=1;
	elements.Day=1;
	elements.Hour=0;
	elements.Minute=0;
	time_t hora=makeTime(elements);
	
	Riego.SetHora(hora);*/

	setTime(0,0,0,1,1,2018);
	printTime(now());
}


///TEST para macros
test (prog_dayToDiasSemana)
{
	PonerHora();
	assertTrue(dayToDiasSemana(1)==L);
	assertTrue(dayToDiasSemana(2)==M);
	assertTrue(dayToDiasSemana(3)==X);
	assertTrue(dayToDiasSemana(4)==J);
	assertTrue(dayToDiasSemana(5)==V);
	assertTrue(dayToDiasSemana(6)==S);
	assertTrue(dayToDiasSemana(7)==D);

}

test (prog_dayOfWeek2)
{
	//Fijamos hora a 01-01-2018 -LUNES
	PonerHora();
	time_t ahora=now();
	printTime(ahora);
	for(uint8_t i=0;i<7;i++)
	{
		 uint8_t dia= dayOfWeek2(ahora + (i*SECS_PER_DAY));
		//uint8_t dia2= dayOfWeek(ahora + (i*SECS_PER_DAY));
		//LOG_DEBUG_ARGS("dayOfWeek %i- dayOfWeek2 %i- contador %i",dia2,dia,i);	
		 assertTrue(dayToDiasSemana(dia)==dayToDiasSemana(i+1));
	}
}

test (prog_elapsedSecsThisWeek2)
{
	//Fijamos hora a 01-01-2018 -LUNES
	PonerHora();
	time_t ahora=now();
	time_t segundos= elapsedSecsThisWeek2(ahora);
	assertTrue(segundos==0);
	
	for(uint8_t i=0;i<24*7;i++)
	{
		time_t seg=(i*SECS_PER_HOUR);
		segundos= elapsedSecsThisWeek2((ahora+seg));
		assertTrue(segundos==SECS_PER_HOUR*i);
	}
	
	//NExt Week
	segundos=elapsedSecsThisWeek2 ((ahora+(SECS_PER_DAY*7)));
	assertTrue(segundos==0);

	//NExt Week + 10
	segundos=elapsedSecsThisWeek2 ((ahora+(SECS_PER_DAY*7)+10));
	assertTrue(segundos==10);

}

///ENDTESTS

///TEST para metodos

test(prog_CargaProgramaDesdeString)
{
	
	Riego.ResetPrograma((uint8_t)0);
	assertTrue(Riego.CargaProgramaDesdeString(0,programa));
	
	memset(buffer_test,0, MAIN_BUFFER_SIZE);
	Riego.ProgramaToString(0,buffer_test);
	
	LOG_DEBUG_ARGS("%s vs %s",programa, buffer_test);
	assertTrue(strcmp(buffer_test,programa)==0);

	pass();	
}

test(prog_GrabarProgramaAEEPROM)
{
	assertTrue(Riego.CargaProgramaDesdeString(0,programa));
	assertTrue(Riego.GrabarProgramaAEEPROM(0));
	Riego.ResetPrograma((uint8_t)0);
	assertTrue(Riego.programas[0].Sector==0);
	assertTrue(Riego.CargarProgramaDesdeEEPROM(0));
	assertTrue(Riego.programas[0].Sector==3);
}

test (prog_CargarProgramaDesdeEEPROM)
{
	//Ya se cubre en  prog_GrabarProgramaAEEPROM
	skip();
}

test(prog_ResetPrograma)
{
	assertTrue(Riego.CargaProgramaDesdeString(0,programa));
	assertTrue(Riego.programas[0].Sector==3);
	Riego.ResetPrograma((uint8_t) 0);
	assertTrue(Riego.programas[0].Sector==0);
}

test(prog_ProgramaToDisplay)
{
	assertTrue(Riego.CargaProgramaDesdeString(0,programa));
	memset(buffer_test,0, MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	assertTrue(strlen(buffer_test)>0);
	LOG_DEBUG_B(buffer_test);
}

test(prog_ProgramaToString)
{
	assertTrue(Riego.CargaProgramaDesdeString(0,programa));

	memset(buffer_test,0, MAIN_BUFFER_SIZE);
	Riego.ProgramaToString(0,buffer_test);
		
	LOG_DEBUG_ARGS("%s vs %s",programa, buffer_test);
	assertTrue(strcmp(buffer_test,programa)==0);
}


test(prog_GetNextEjecucion)
{
	TimeElements telements;
	//Ponemos hora 1/01/2018- LUNES
	PonerHora();

	//Ponemos programa
	Riego.programas[0].Dias =  M;
	Riego.programas[0].HoraInicio=8;
	
	time_t nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	//Ejecucion del prog deberia ser el 02-01 a las 8:00
	assertTrue(nextexecution== GetTime(8,0,0,2,1,2018));

	//Si estamos el martes a las 7:59 aun debemos tener como proxima ejecucion ese dia a las 08:00
	setTime(7,59,0,2,1,2018);
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(8,0,0,2,1,2018));


	//Ahora si hemos llegado al martes a las 8 su programacion se tiene que ir a la semana siguiente
	setTime(8,0,0,2,1,2018);
	printTime(now());
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(8,0,0,9,1,2018));


	//Ponemos el jueves y vemos que fecha propone
	Riego.programas[0].Dias =  J;
	Riego.programas[0].HoraInicio=23;
	Riego.programas[0].MinutoInicio=59;
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(23,59,0,4,1,2018));

	//Ponemos el jueves y vemos que fecha propone
	Riego.programas[0].Dias =  J;
	Riego.programas[0].HoraInicio=6;
	Riego.programas[0].MinutoInicio=1;
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,4,1,2018));
	

	
	//Programamos dias de la semana y vemos que fecha propone
	Riego.programas[0].Dias =  L | M  | X | J | V;
	Riego.programas[0].HoraInicio=6;
	Riego.programas[0].MinutoInicio=1;
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,3,1,2018));


	//NONE
	Riego.programas[0].Dias =  NONE;
	Riego.programas[0].HoraInicio=6;
	Riego.programas[0].MinutoInicio=1;
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== 0);

	//Lunes domingo y sabado.. deberia ser el sabado (dia 6)
	Riego.programas[0].Dias =  L | D | S;
	Riego.programas[0].HoraInicio=6;
	Riego.programas[0].MinutoInicio=1;
	nextexecution=Riego.GetNextEjecucion(0);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,6,1,2018));
	
}

test(prog_EEPROMCargaProgramas)
{

	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.CargaProgramaDesdeString(i,programa));
		Riego.programas[i].Sector= i;
		assertTrue(Riego.GrabarProgramaAEEPROM(i));
		assertTrue(Riego.CargarProgramaDesdeEEPROM(i));
		assertTrue(Riego.programas[i].Sector== i);

	}

	//Si llamamos a Riego.ResetPRogramas(); //soon guardados en eeprom
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{	
		Riego.ResetPrograma(i);
	}
 

	assertTrue(Riego.EEPROMCargaProgramas());

	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.programas[i].Sector== i);
	}
	
}


//REsetea todos los programas y los graba en la eeprom
test(prog_ResetProgramas)
{
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.CargaProgramaDesdeString(i,programa));
		Riego.programas[i].Sector= i;
		assertTrue(Riego.GrabarProgramaAEEPROM(i));
		assertTrue(Riego.CargarProgramaDesdeEEPROM(i));
		assertTrue(Riego.programas[i].Sector== i);

	}

	//Si llamamos a Riego.ResetPRogramas(); //soon guardados en eeprom
	Riego.ResetProgramas();

	assertTrue(Riego.EEPROMCargaProgramas());

	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.programas[i].Sector== 0);
	}
	
}

test(prog_ShowInfoProgramas)
{
		for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
		{
			assertTrue(Riego.CargaProgramaDesdeString(i,programa));
			Riego.programas[i].Sector= i;
			Riego.programas[i].TiempoRiego=i*60;//Incrementamos 1' a cada sector
			}
	
		Riego.ShowInfoProgramas();
		pass();

}
/*
	Programa(char * internalbuffer,uint8_t sizebuffer);
	
	bool CargaProgramaDesdeString(uint8_t progIndex,char *programa);
	bool GrabarProgramaAEEPROM(uint8_t progIndex);
	bool CargarProgramaDesdeEEPROM(uint8_t progIndex);
	void ResetPrograma(uint8_t progIndex);
	void ProgramaToDisplay(uint8_t progIndex,char *text);
	void ProgramaToString(uint8_t progIndex,char *text);
	time_t GetNextEjecucion(uint8_t progIndex);//Calcula la siguiente fecha de ejecucion para un programa dado
	
	
	//Metodos para toda la coleccion
	inline time_t GetNextAction() { return nexaction;}//Devuelve la fecha de la proxima accion programada a ejecutar CalculateNextAction
	bool EEPROMCargaProgramas();
	void ResetProgramas();
	void ShowInfoProgramas();
	inline bool GetChangedProgramas() { return changed;}
	protected:
	inline void SetChangedProgramas(bool value) { changed=value;}
	void CalculateNextAction ();//Calcula la fecha de la proxima accion programada a ejecutar (Programas & Salidas) <-> (Parar - arrancar) devuelta por GetNextAction
	private:
*/


//ENDTESTS



