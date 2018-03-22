/*
 * CPPFile1.cpp
 *
 * Created: 22/03/2018 1:04:05
 *  Author: Gabi
 */ 
 #include "main.h"



 //Struct definido para realizar las pruebas.
 //En ella se definiran la fecha inicio & fin de riego/Abono para cada programa que se ejecute. ;)
 typedef struct
 {
	tPrograma *programa;
	time_t	  InicioRiego;
	time_t	  FinRiego;
	time_t	  InicioAbono;
	time_t	  FinAbono;
 } tResults;

 //Array de results
 tResults results [MAX_PROGRAMAS];

 //Inicializacion de results;
 void InitResults()
 {
	for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		results[i].InicioRiego=0;
		results[i].FinRiego=0;
		results[i].InicioAbono=0;
		results[i].FinAbono=0;
	}
 }

 
 uint8_t lanzados=0;
 uint8_t parados=0;
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
test (riego_EnciendeSector_ApagarRiegos)
{
	
	FijarFecha(2018,1,1);//Lunes 00:00

	for(uint8_t i=0;i<PORTS_NUM;i++)
		Riego.EnciendeSector(i+1);

	delay(100);
	assertTrue(Riego.GetSalidasActivas()==PORTS_NUM);

	Riego.ApagarRiegos();
	assertTrue(Riego.GetSalidasActivas()==0);
	pass();

}

test (riego_Calcular_FechaHasta)
{
	
	FijarFecha(2018,1,1);//Lunes 00:00
	//Limpiamos salidas
	Riego.ApagarRiegos();

	//Programamos para 2018/01/01 00:01
	Riego.programas[0].Dias= L | M | X;
	Riego.programas[0].Sector=1;
	Riego.programas[0].HoraInicio=hour(now());
	Riego.programas[0].MinutoInicio=1;
	Riego.programas[0].TiempoAbono=2;//Minutos
	Riego.programas[0].TiempoRiego=3;//Minutos
	Riego.programas[0].Motor=true;
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);

	Riego.LanzaRiego(0,false);
	time_t ahora=now();

	//2 salidas activas correspondientes al abono y al programa
	assertTrue(Riego.GetSalidasActivas()==2);

	int posicion= Riego.GetPosicion(1,actAbono);
	if (posicion!=-1)
	{
	//LOG_DEBUG_ARGS("Tiempo %lu %lu",Riego.salidas[posicion].Hasta,ahora+(Riego.programas[0].TiempoAbono*SECS_PER_MIN));
	assertTrue(Riego.salidas[posicion].Hasta==ahora+(Riego.programas[0].TiempoAbono*SECS_PER_MIN));

	}
	

	posicion=Riego.GetPosicion(0,actPrograma);
		if (posicion!=-1)
		{
	//LOG_DEBUG_ARGS("Tiempo prog %lu %lu",Riego.salidas[posicion].Hasta,ahora+(Riego.programas[0].TiempoRiego*SECS_PER_MIN));
	assertTrue(Riego.salidas[posicion].Hasta==ahora+(Riego.programas[0].TiempoRiego*SECS_PER_MIN));
	}

	Riego.ShowInfoSalidas();
}



test (riego_multiples)
{ 
 
	FijarFecha(2018,1,1);//Lunes
	Riego.ApagarRiegos();

	//Programamos para 2018/01/01 00:01	 
	Riego.programas[0].Dias= L | M | X;
	Riego.programas[0].Sector=1;
	Riego.programas[0].HoraInicio=hour(now());
	Riego.programas[0].MinutoInicio=1;
	Riego.programas[0].TiempoAbono=2;//Minutos
	Riego.programas[0].TiempoRiego=3;//Minutos
	Riego.programas[0].Motor=true;
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(0,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);

	//Acontinuacion Sector2
	Riego.programas[1].Dias= L | M | X;
	Riego.programas[1].Sector=2;
	Riego.programas[1].HoraInicio=hour(now());
	Riego.programas[1].MinutoInicio=3;
	Riego.programas[1].TiempoAbono=1;//Minutos
	Riego.programas[1].TiempoRiego=3;//Minutos
	Riego.programas[1].Motor=false;
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(1,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
	
	//Chequea riegos por 1ª vez para que se actualice la fecha de actualizacion
	Riego.CheckRiegos();

	//setTime(now()+ (5*SECS_PER_MIN));//Lo adelantamos 5 minutos , deberian saltar los 2 riegos
	time_t ahora=now();

			
	Riego.LanzaCallBack([](SalidasActivas *salida,tPrograma *programa){ 
	
		if (salida->Tipo== actAbono)
			LOG_DEBUG("LANZADO ABONO"); 
		else if (salida->Tipo ==actPrograma)
			LOG_DEBUG_ARGS("LANZADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector ); 

		Riego.ShowInfoSalidas();
		lanzados++;
	
	});
	Riego.ParaCallBack([](SalidasActivas *salida,tPrograma *programa){ 
	
			if (salida->Tipo== actAbono)
			LOG_DEBUG("PARADO ABONO");
			else if (salida->Tipo ==actPrograma)
			{
				LOG_DEBUG_ARGS("PARADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector ); 
				Riego.ShowInfoSalidas();	
			}
			
	
	 parados++;});

	while(ELAPSED_SECONDS(ahora)<(SECS_PER_MIN*8))
	{
			Riego.CheckRiegos();
			time_t newtime=now()+10;//1' cada loop ;)
			setTime(newtime);
			Riego.ShowInfoSalidas();
	}

	
	Riego.ShowInfoSalidas();
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(lanzados>3);
	assertTrue(parados==lanzados);
	pass();
}


//Cuando se produce un delay imprevisto tiene que chequear que no se colo ninguno por el medio.
test (riego_delay_imprevistos)
{ 
 
	FijarFecha(2018,1,1);
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		tPrograma* programa=&Riego.programas[i];
			uint8_t port=(i % MAX_PORTS);
			programa->Sector =port;


			bool dias=(bool)(random(0,15)>=12);
			if (dias)
			{
		

				int veces=random(1,7);
				for (uint8_t j=0;j<veces;j++)
					programa->Dias|= dayToDiasSemana(j);

							programa->HoraInicio=random(0,23);
							programa->MinutoInicio=random(0,59);
			}
			else
			{

					programa->HoraInicio=88;
					do
					{
						programa->MinutoInicio=random(1,MAX_PROGRAMAS);
					} while (programa->MinutoInicio==i);
 
		
			}

			programa->Motor=(bool)random(0,1);

			programa->TiempoAbono=random(0,2);//Minutos
			programa->TiempoRiego=random(3,5);//Minutos
		
	}

	Riego.ShowInfoProgramas();
	//setTime(now()+ (5*SECS_PER_MIN));//Lo adelantamos 5 minutos , deberian saltar los 2 riegos
	time_t ahora=now();

	
	Riego.LanzaCallBack([](SalidasActivas *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		LOG_DEBUG("LANZADO ABONO");
		else if (salida->Tipo ==actPrograma)
		LOG_DEBUG_ARGS("LANZADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );

		Riego.ShowInfoSalidas();
		lanzados++;
		
	});
	Riego.ParaCallBack([](SalidasActivas *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		LOG_DEBUG("PARADO ABONO");
		else if (salida->Tipo ==actPrograma)
		{
			LOG_DEBUG_ARGS("PARADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );
			Riego.ShowInfoSalidas();
		}
		
		
	parados++;});

	while(ELAPSED_SECONDS(ahora)<(SECS_PER_HOUR*24))
	{
		Riego.CheckRiegos();
		time_t newtime=now()+100;//1' cada loop ;)
		setTime(newtime);
		Riego.ShowInfoSalidas();
	}

	
	Riego.ShowInfoSalidas();
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(lanzados>3);
	assertTrue(parados==lanzados);
	pass();
}




//Esto es para acelearar el bucle y el tiempo corra más liguero
testing(riegos_check)
 {
	 
	if (checkTestDone(riego_EnciendeSector_ApagarRiegos) && 
		checkTestDone(riego_multiples) &&
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
