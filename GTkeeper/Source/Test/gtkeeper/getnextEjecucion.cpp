////
/////*
 ////* getnextEjecucion.cpp
 ////*
 ////* Created: 17/03/2018 15:50:58
 ////*  Author: Gabi
 ////*/ 


#include <ArduinoUnit.h>
#include <main.h>



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
	
	gtKeeper.SetHora(hora);*/

	setTime(0,0,0,1,1,2018);
	printTime(now());
}

test (dayToDiasSemana)
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

test (dayOfWeek2)
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

test (elapsedSecsThisWeek2)
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


test(getnextEjecucion)
{
	TimeElements telements;
	//Ponemos hora 1/01/2018- LUNES
	PonerHora();
	//Ponemos programa
	gtKeeper.programas[0].Dias =  M;
	gtKeeper.programas[0].HoraInicio=8;
	
	time_t nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	//Ejecucion del prog deberia ser el 02-01 a las 8:00
	assertTrue(nextexecution== GetTime(8,0,0,2,1,2018));

	//Si estamos el martes a las 7:59 aun debemos tener como proxima ejecucion ese dia a las 08:00
	setTime(7,59,0,2,1,2018);
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(8,0,0,2,1,2018));


	//Ahora si hemos llegado al martes a las 8 su programacion se tiene que ir a la semana siguiente
	setTime(8,0,0,2,1,2018);
	printTime(now());
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(8,0,0,9,1,2018));


	//Ponemos el jueves y vemos que fecha propone
	gtKeeper.programas[0].Dias =  J;
	gtKeeper.programas[0].HoraInicio=23;
	gtKeeper.programas[0].MinutoInicio=59;	
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(23,59,0,4,1,2018));

	//Ponemos el jueves y vemos que fecha propone
	gtKeeper.programas[0].Dias =  J;
	gtKeeper.programas[0].HoraInicio=6;
	gtKeeper.programas[0].MinutoInicio=1;
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,4,1,2018));
		

		
	//Programamos dias de la semana y vemos que fecha propone
	gtKeeper.programas[0].Dias =  L | M  | X | J | V;
	gtKeeper.programas[0].HoraInicio=6;
	gtKeeper.programas[0].MinutoInicio=1;
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,3,1,2018));


	//NONE
	gtKeeper.programas[0].Dias =  NONE;
	gtKeeper.programas[0].HoraInicio=6;
	gtKeeper.programas[0].MinutoInicio=1;
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== 0);

	//Lunes domingo y sabado.. deberia ser el sabado (dia 6)
	gtKeeper.programas[0].Dias =  L | D | S;
	gtKeeper.programas[0].HoraInicio=6;
	gtKeeper.programas[0].MinutoInicio=1;
	nextexecution=gtKeeper.GetNextEjecucion(&gtKeeper.programas[0]);
	printTime(nextexecution);
	assertTrue(nextexecution== GetTime(6,1,0,6,1,2018));
	
}

 



