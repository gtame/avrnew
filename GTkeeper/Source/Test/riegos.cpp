/*
 * CPPFile1.cpp
 *
 * Created: 22/03/2018 1:04:05
 *  Author: Gabi
 */ 
 #include "main.h"

time_t lastcheckok=0;
void FijarFecha(int year, uint month ,uint day)
{
	 //Fijamos la fecha a 1/1/2018 0:00 Lunes
	 TimeElements time;
	 time.Year=CalendarYrToTm(year);
	 time.Month=month;
	 time.Day=day;
	 time.Hour=0;
	 time.Minute=0;
	 time.Second=0;
	 time_t ahora=makeTime(time);
	 setTime(ahora);
	
}

//Cuando se produce un delay imprevisto tiene que chequear que no se colo ninguno por el medio.
test (riego_anade_simple)
{
	
	FijarFecha(2018,1,1);//Lunes 00:00
	/*
	time_t ahora=now();
	ahora+=60;
	Riego.programas[0].Dias=127;
	Riego.programas[0].Sector=1;

	Riego.programas[0].HoraInicio=hour(ahora);
	Riego.programas[0].MinutoInicio=minute(ahora);
	Riego.programas[0].TiempoAbono=200;
	Riego.programas[0].TiempoRiego=300;
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);*/
}


//Cuando se produce un delay imprevisto tiene que chequear que no se colo ninguno por el medio.
test (riego_delay_imprevisto)
{ 
	FijarFecha(2018,1,1);
	time_t ahora=now();
	ahora+=60;

	Riego.programas[0].Dias=127;
	Riego.programas[0].Sector=1;
	Riego.programas[0].HoraInicio=hour(ahora);
	Riego.programas[0].MinutoInicio=minute(ahora);
	Riego.programas[0].TiempoAbono=20;//Minutos
	Riego.programas[0].TiempoRiego=30;//Minutos
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
	
	
	Riego.programas[1].Dias=0;
	Riego.programas[1].Sector=2;
	Riego.programas[1].HoraInicio=88;
	Riego.programas[1].MinutoInicio=1;
	Riego.programas[1].TiempoAbono=20;//Minutos
	Riego.programas[1].TiempoRiego=30;//Minutos

	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(1,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
}


testing(riegos_check)
 {
	 
	if (checkTestDone(riego_anade_simple) && 
		checkTestDone(riego_delay_imprevisto) &&
		Riego.GetSalidasActivas()==0
		)
		{
			if (lastcheckok==0)
				lastcheckok=now();	
		}
	else
		lastcheckok=0;
	
		
	if (lastcheckok!=0 && ELAPSED_SECONDS(lastcheckok)>600)
	{
		//Cuando se han ejecutado todos finaliza
		pass();
	}
	else
	{
			time_t newtime=now()+10;//1' cada loop ;)
		setTime(newtime);

		//Ejecutamos un riego simple
		//LogTime(now());
		Riego.CheckRiegos();
		Riego.ShowInfoSalidas();
	}
 }
