/*
 * estadistica.cpp
 *
 * Created: 20/03/2018 20:00:48
 *  Author: Gabi
 */ 
 #include "..\main.h"


 test(esta_EEPROMLeerEstadistica)
 {
 		Riego.estadisticas[0].tiempo_riego=60;
 		Riego.estadisticas[0].tiempo_abono=60;
 		Riego.EEPROMGuardarEstadistica(&Riego.estadisticas[0]);

		Riego.ResetearEstadistica(0);
		assertTrue(	Riego.estadisticas[0].tiempo_riego==0);
		assertTrue(	Riego.estadisticas[0].tiempo_abono==0);

		Riego.EEPROMLeerEstadistica(0,&Riego.estadisticas[0]);
	

		assertTrue(	Riego.estadisticas[0].sector==1);
		assertTrue(	Riego.estadisticas[0].tiempo_riego==60);
		assertTrue(	Riego.estadisticas[0].tiempo_abono==60);
		

 }

 test(esta_EEPROMGuardarEstadistica)
 {
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
 
		assertTrue(Riego.estadisticas[i-1].sector==i);
		Riego.estadisticas[i-1].tiempo_riego=i*60;
		Riego.estadisticas[i-1].tiempo_abono=i*60;
		Riego.EEPROMGuardarEstadistica(&Riego.estadisticas[i-1]);
		assertTrue(Riego.estadisticas[i-1].sector==i);
	}

	

	//No se llama a ResetearEstadisticas() pq ese metodo guarda a EEPROM	
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
		Riego.ResetearEstadistica(i-1);
		assertTrue(Riego.estadisticas[i-1].sector==i);
		assertTrue(Riego.estadisticas[i-1].tiempo_abono==0);
		assertTrue(Riego.estadisticas[i-1].tiempo_riego==0);
	}
	
	Riego.CargarEstadisticasEEPROM();

	
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
		assertTrue(Riego.estadisticas[i-1].sector==i);
		assertTrue(Riego.estadisticas[i-1].tiempo_riego==i*60);
		assertTrue(Riego.estadisticas[i-1].tiempo_abono==i*60);
	}

	
 }

 test(esta_RegistrarEstadisticas)
 {

 //To-DO

 }

 test(esta_ResetearEstadisticas)
 {
	 for(uint8_t i=1;i<=MAX_PORTS;i++)
	 {
		 //assertTrue(Riego.estadisticas[i-1].sector==i);
		 Riego.estadisticas[i-1].sector=i;
		 Riego.estadisticas[i-1].tiempo_riego=i*60;
		 Riego.estadisticas[i-1].tiempo_abono=i*60;
		 Riego.EEPROMGuardarEstadistica(&Riego.estadisticas[i-1]);

	 }

	 Riego.ResetearEstadisticas();
	 

		for(uint8_t i=1;i<=MAX_PORTS;i++)
		{
			assertTrue(Riego.estadisticas[i-1].sector==i);
			Riego.estadisticas[i-1].tiempo_riego=i*60;
			Riego.estadisticas[i-1].tiempo_abono=i*60;
		
		}

	  Riego.CargarEstadisticasEEPROM();

	  for(uint8_t i=1;i<=MAX_PORTS;i++)
	  {
		  assertTrue(Riego.estadisticas[i-1].sector==i);
		  assertTrue(Riego.estadisticas[i-1].tiempo_riego==0);
		  assertTrue(Riego.estadisticas[i-1].tiempo_abono==0);
	  }
 }


 test(esta_CargarEstadisticasEEPROM)
 {
	skip(); //Ya ejecutado en  GuardarEstadisticasEEPROM ..EEPROMGuardarEstadistica
 }
 

 test(esta_GuardarEstadisticasEEPROM)
 {
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
		assertTrue(Riego.estadisticas[i-1].sector==i);
		Riego.estadisticas[i-1].tiempo_riego=i*60;
		Riego.estadisticas[i-1].tiempo_abono=i*60;
	

	}

 Riego.GuardarEstadisticasEEPROM();

 //No se llama a ResetearEstadisticas() pq ese metodo guarda a EEPROM
 for(uint8_t i=1;i<=MAX_PORTS;i++)
 {
	 Riego.ResetearEstadistica(i-1);
	 assertTrue(Riego.estadisticas[i-1].tiempo_abono==0);
	 assertTrue(Riego.estadisticas[i-1].tiempo_riego==0);
 }

 Riego.CargarEstadisticasEEPROM();

 for(uint8_t i=1;i<=MAX_PORTS;i++)
 {
	 assertTrue(Riego.estadisticas[i-1].sector==i);
	 assertTrue(Riego.estadisticas[i-1].tiempo_riego==i*60);
	 assertTrue(Riego.estadisticas[i-1].tiempo_abono==i*60);
 }

 }