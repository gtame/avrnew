/*
 * CPPFile1.cpp
 *
 * Created: 22/03/2018 1:04:05
 *  Author: Gabi
 */ 
 #include "..\main.h"

 uint8_t lastprograma=0;
 uint8_t lanzados=0;
 uint8_t parados=0;
 uint8_t lanzados_abonos=0;
 uint8_t parados_abonos=0;
 time_t lastcheckok=0;
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
	 	lanzados=0;
	 	lanzados_abonos=0;
	 	parados=0;
	 	parados_abonos=0;
		//Limpiamos salidas
		Riego.ApagarRiegos();
		
		Riego.ResetProgramas();
		 
	for (uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		results[i].InicioRiego=0;
		results[i].FinRiego=0;
		results[i].InicioAbono=0;
		results[i].FinAbono=0;
	}
 }

void LoadProgramas(uint8_t desde, uint8_t hasta)
{
	
	
	Riego.programas[desde].Dias= L | M | X;
	Riego.programas[desde].Sector=1;
	Riego.programas[desde].HoraInicio=hour(now());
	Riego.programas[desde].MinutoInicio=1;
	Riego.programas[desde].TiempoAbono=random(2,3);//Minutos
	Riego.programas[desde].TiempoRiego=random(3,5);//Minutos
	Riego.programas[desde].Motor=true;
	
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Riego.ProgramaToDisplay(desde,buffer_test);
	LOG_DEBUG_ARGS("PROG-> %s",buffer_test);
	
	results[desde].programa=&Riego.programas[desde];
	results[desde].InicioAbono=getTime(0,1,0,1,1,2018) ;
	results[desde].FinAbono=results[desde].InicioAbono + (Riego.programas[desde].TiempoAbono * SECS_PER_MIN) ;
	results[desde].InicioRiego=getTime(0,1,0,1,1,2018) ;;
	results[desde].FinRiego= results[desde].InicioRiego+ (Riego.programas[desde].TiempoRiego * SECS_PER_MIN);
	
	for(uint8_t i=desde+1;i<hasta;i++)
	{
		tPrograma* programa=&Riego.programas[i];
		tResults* resultanterior=&results[i-1];
		
		programa->Sector =(i % PORTS_NUM)+1;//No puede haber sector 0 por eso +1
		programa->HoraInicio=88;
		programa->MinutoInicio=i;//programa anterior
		programa->Motor=(bool)random(0,1);
		programa->TiempoAbono=random(0,2);//Minutos
		programa->TiempoRiego=random(3,5);//Minutos
		
		
		results[i].programa=programa;
		
		//La fecha de inicio sera la fecha fin del anterior
		results[i].InicioAbono=resultanterior->FinRiego;//El inicio del abono se dara con el inicio del anterior
		results[i].InicioRiego=resultanterior->FinRiego;
		
		//La fecha fin, sera la fecha inicio + tiempo indicado
		results[i].FinAbono=results[i].InicioAbono + (programa->TiempoAbono* SECS_PER_MIN) ;
		results[i].FinRiego= results[i].InicioRiego+ (programa->TiempoRiego * SECS_PER_MIN);
	}

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

//Calculo de fecha hasta
test (riego_Calcular_FechaHasta)
{
	
	FijarFecha(2018,1,1);//Lunes 00:00
	InitResults();

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

//Riego con delay ... para comprobar que saltan los sectores
test (riego_delay_imprevistos)
{ 
	FijarFecha(2018,1,1);
	
	InitResults();
	
	LoadProgramas(0,3);
	
	

			
	Riego.LanzaCallBack([](tSalida *salida,tPrograma *programa){ 
	
		if (salida->Tipo== actAbono)
			LOG_DEBUG("LANZADO ABONO"); 
		else if (salida->Tipo ==actPrograma)
		{
			LOG_DEBUG_ARGS("LANZADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector ); 
					lanzados++;
		}

		Riego.ShowInfoSalidas();

	
	});
	Riego.ParaCallBack([](tSalida *salida,tPrograma *programa){ 
	
			if (salida->Tipo== actAbono)
			LOG_DEBUG("PARADO ABONO");
			else if (salida->Tipo ==actPrograma)
			{
				 parados++;
				LOG_DEBUG_ARGS("PARADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector ); 
				Riego.ShowInfoSalidas();	
			}
			
	
	});
	 
	 
	 Riego.ShowInfoProgramas();
	 
	 
	 
	 //Lo adelantamos 1' para que saltee .. el riego 1
	 setTime(now()+61);
	 
	 //Chequea riegos por 1ª vez para que se actualice la fecha de actualizacion
	 Riego.CheckRiegos();


	//Ahora lo adelanatamos 5' deberian saltar los atrasados.
	setTime(now()+300);
	time_t ahora= now();

	while(ELAPSED_SECONDS(ahora)<(SECS_PER_MIN*15))
	{
			Riego.CheckRiegos();
			time_t newtime=now()+10;//1' cada loop ;)
			setTime(newtime);
			Riego.ShowInfoSalidas();
	}

	
	Riego.ShowInfoSalidas();
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(lanzados==3);
	assertTrue(parados==lanzados);
	pass();
}


//Riego multiple secuencial , Empieza-> siguiente sector -> siguiente sector -> siguiente sector.....N veces
test (riego_multiple_secuencial)
{
	
  	FijarFecha(2018,1,1);
	InitResults();

	LoadProgramas(0,20);
	 

	Riego.ShowInfoProgramas();

	//setTime(now()+ (5*SECS_PER_MIN));//Lo adelantamos 5 minutos , deberian saltar los 2 riegos
	time_t ahora=now();

	Riego.LanzaCallBack([](tSalida *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		{
			LOG_DEBUG_ARGS("LANZADO ABONO %i %i ",minute(results[lastprograma].InicioAbono),minute());
			assertTrue(minute(results[lastprograma].InicioAbono)==minute());
			lanzados_abonos++;
			
		}
		else if (salida->Tipo ==actPrograma)
		{
			lastprograma	=salida->Ident;
			LOG_DEBUG_ARGS("LANZADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );
			LOG_DEBUG_ARGS("LANZADO PROGRAMA %i %i ",minute(results[salida->Ident].InicioRiego),minute());
			assertTrue(minute(results[salida->Ident].InicioRiego)==minute());
			lanzados++;
		}
		
		
	});

	Riego.ParaCallBack([](tSalida *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		{
			//Como no se mez
 			LOG_DEBUG_ARGS("PARADO ABONO %i %i ",minute(results[lastprograma].FinAbono),minute());
			assertTrue(minute(results[lastprograma].FinAbono)==minute());
			parados_abonos++;
		}
		else if (salida->Tipo ==actPrograma)
		{
			
			uint8_t lastprogramaParado=salida->Ident;
			
			LOG_DEBUG_ARGS("PARADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );
			LOG_DEBUG_ARGS("PARADO PROGRAMA %i %i ",minute(results[lastprogramaParado].FinAbono),minute());
			assertTrue(minute(results[lastprogramaParado].FinRiego)==minute());
			//Riego.ShowInfoSalidas();
			parados++;
		}
		
	});

	while(ELAPSED_SECONDS(ahora)<(SECS_PER_HOUR*2))
	{
		Riego.CheckRiegos();
		time_t newtime=now()+55;//1' cada loop ;)
		setTime(newtime);
	}

	
	Riego.ShowInfoSalidas();
	 
	
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(lanzados>3);
	assertTrue(parados==lanzados);
	//Abonos
	assertTrue(lanzados_abonos==parados_abonos);
	
}


//Riego multiple secuencial , Empieza-> siguiente sector -> siguiente sector -> siguiente sector.....N veces
test (riego_multiple_cruzado)
{
	
	FijarFecha(2018,1,1);
	InitResults();
	

	
	LoadProgramas(0,10);
	LoadProgramas(10,MAX_PROGRAMAS);
	
	Riego.ShowInfoProgramas();

	
	//setTime(now()+ (5*SECS_PER_MIN));//Lo adelantamos 5 minutos , deberian saltar los 2 riegos
	time_t ahora=now();

	
	Riego.LanzaCallBack([](tSalida *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		{
			LOG_DEBUG_ARGS("LANZADO ABONO %i %i ",minute(results[lastprograma].InicioAbono),minute());
			assertTrue(minute(results[lastprograma].InicioAbono)==minute());
			lanzados_abonos++;
			
		}
		else if (salida->Tipo ==actPrograma)
		{
			lastprograma	=salida->Ident;
			LOG_DEBUG_ARGS("LANZADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );
			LOG_DEBUG_ARGS("LANZADO PROGRAMA %i %i ",minute(results[salida->Ident].InicioRiego),minute());
			assertTrue(minute(results[salida->Ident].InicioRiego)==minute());
			lanzados++;
		}
		
		
		
	});

	Riego.ParaCallBack([](tSalida *salida,tPrograma *programa){
		
		if (salida->Tipo== actAbono)
		{
			//Como no se mez
			LOG_DEBUG_ARGS("PARADO ABONO %i %i ",minute(results[lastprograma].FinAbono),minute());
			assertTrue(minute(results[lastprograma].FinAbono)==minute());
			parados_abonos++;
		}
		else if (salida->Tipo ==actPrograma)
		{
			
			uint8_t lastprogramaParado=salida->Ident;
			
			LOG_DEBUG_ARGS("PARADO PROGRAMA P%i S%i ",salida->Ident,programa->Sector );
			LOG_DEBUG_ARGS("PARADO PROGRAMA %i %i ",minute(results[lastprogramaParado].FinAbono),minute());
			assertTrue(minute(results[lastprogramaParado].FinRiego)==minute());
			//Riego.ShowInfoSalidas();
			parados++;
		}
		
	});

	while(ELAPSED_SECONDS(ahora)<(SECS_PER_HOUR*1))
	{
		Riego.CheckRiegos();
		time_t newtime=now()+55;//1' cada loop ;)
		setTime(newtime);
	}

	
	Riego.ShowInfoSalidas();
	
	LOG_DEBUG_ARGS("Lanzados %i - Parados %i",lanzados,parados);
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(lanzados==MAX_PROGRAMAS);
	assertTrue(lanzados==parados);
	
	//Abonos
	assertTrue(lanzados_abonos==parados_abonos);
	
	
}


//Calcula cuanto le queda para la siguiente accion a procesar
test (riego_CalculateNextAction_reset)
{
		
	FijarFecha(2018,1,1);
	InitResults();
	//Resetear programas
	Riego.ResetProgramas();
	time_t result=Riego.CalculateNextAction();
	assertTrue(result==0);
	
}


//Calcula cuanto le queda para la siguiente accion a procesar
test (riego_CalculateNextAction_dia)
{
	FijarFecha(2018,1,1);
	InitResults();

	LoadProgramas(0,20);
	//Riego.ShowInfoProgramas();


	time_t result=Riego.CalculateNextAction();
	LOG_DEBUG_ARGS("Result %i",result-now());
	assertTrue(result-now()==SECS_PER_MIN);
}

 
//Calcula cuanto le queda para la siguiente accion a procesar
test (riego_CalculateNextAction_conSalida)
{
	FijarFecha(2018,1,1);
	InitResults();
	LoadProgramas(0,3);
	Riego.programas[0].HoraInicio=6; //Adelante 1 hora el prog1 - index0

	//Obtengo del prog2(index1) y cogo el minimo entre tiempo abono y riego y le sumo la fecha actual , esa sera su fecha de paro
	time_t resultmin=(Riego.programas[1].TiempoAbono==0?Riego.programas[1].TiempoRiego: min(Riego.programas[1].TiempoAbono,Riego.programas[1].TiempoRiego))*SECS_PER_MIN+now();
	//LanzamosPrograma Index 1 ;)
	Riego.LanzaRiego(1,false);
	
	//La siguiente ejecucion sería la parada de ese riego 
	time_t result=Riego.CalculateNextAction();
	//LogTime(resultmin);
	//LogTime(result);
	//LOG_DEBUG_ARGS("Resuxlt %lu %lu",result,resultmin);
	assertTrue(result==resultmin);
}


