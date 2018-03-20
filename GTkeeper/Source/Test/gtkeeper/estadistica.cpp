/*
 * estadistica.cpp
 *
 * Created: 20/03/2018 20:00:48
 *  Author: Gabi
 */ 
 #include "..\main.h"


 test(esta_EEPROMLeerEstadistica)
 {
 		gtKeeper.estadisticas[0].tiempo_riego=60;
 		gtKeeper.estadisticas[0].tiempo_abono=60;
 		gtKeeper.EEPROMGuardarEstadistica(&gtKeeper.estadisticas[0]);

		gtKeeper.ResetearEstadistica(0);
		assertTrue(	gtKeeper.estadisticas[0].tiempo_riego==0);
		assertTrue(	gtKeeper.estadisticas[0].tiempo_abono==0);

		gtKeeper.EEPROMLeerEstadistica(0,&gtKeeper.estadisticas[0]);
	

		assertTrue(	gtKeeper.estadisticas[0].sector==1);
		assertTrue(	gtKeeper.estadisticas[0].tiempo_riego==60);
		assertTrue(	gtKeeper.estadisticas[0].tiempo_abono==60);
		

 }

 test(esta_EEPROMGuardarEstadistica)
 {
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
 
		assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		gtKeeper.estadisticas[i-1].tiempo_riego=i*60;
		gtKeeper.estadisticas[i-1].tiempo_abono=i*60;
		gtKeeper.EEPROMGuardarEstadistica(&gtKeeper.estadisticas[i-1]);
		assertTrue(gtKeeper.estadisticas[i-1].sector==i);
	}

	

	//No se llama a ResetearEstadisticas() pq ese metodo guarda a EEPROM	
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
		gtKeeper.ResetearEstadistica(i-1);
		assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		assertTrue(gtKeeper.estadisticas[i-1].tiempo_abono==0);
		assertTrue(gtKeeper.estadisticas[i-1].tiempo_riego==0);
	}
	
	gtKeeper.CargarEstadisticasEEPROM();

	
	for(uint8_t i=1;i<=MAX_PORTS;i++)
	{
		assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		assertTrue(gtKeeper.estadisticas[i-1].tiempo_riego==i*60);
		assertTrue(gtKeeper.estadisticas[i-1].tiempo_abono==i*60);
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
		 //assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		 gtKeeper.estadisticas[i-1].sector=i;
		 gtKeeper.estadisticas[i-1].tiempo_riego=i*60;
		 gtKeeper.estadisticas[i-1].tiempo_abono=i*60;
		 gtKeeper.EEPROMGuardarEstadistica(&gtKeeper.estadisticas[i-1]);

	 }

	 gtKeeper.ResetearEstadisticas();
	 

		for(uint8_t i=1;i<=MAX_PORTS;i++)
		{
			assertTrue(gtKeeper.estadisticas[i-1].sector==i);
			gtKeeper.estadisticas[i-1].tiempo_riego=i*60;
			gtKeeper.estadisticas[i-1].tiempo_abono=i*60;
		
		}

	  gtKeeper.CargarEstadisticasEEPROM();

	  for(uint8_t i=1;i<=MAX_PORTS;i++)
	  {
		  assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		  assertTrue(gtKeeper.estadisticas[i-1].tiempo_riego==0);
		  assertTrue(gtKeeper.estadisticas[i-1].tiempo_abono==0);
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
		assertTrue(gtKeeper.estadisticas[i-1].sector==i);
		gtKeeper.estadisticas[i-1].tiempo_riego=i*60;
		gtKeeper.estadisticas[i-1].tiempo_abono=i*60;
	

	}

 gtKeeper.GuardarEstadisticasEEPROM();

 //No se llama a ResetearEstadisticas() pq ese metodo guarda a EEPROM
 for(uint8_t i=1;i<=MAX_PORTS;i++)
 {
	 gtKeeper.ResetearEstadistica(i-1);
	 assertTrue(gtKeeper.estadisticas[i-1].tiempo_abono==0);
	 assertTrue(gtKeeper.estadisticas[i-1].tiempo_riego==0);
 }

 gtKeeper.CargarEstadisticasEEPROM();

 for(uint8_t i=1;i<=MAX_PORTS;i++)
 {
	 assertTrue(gtKeeper.estadisticas[i-1].sector==i);
	 assertTrue(gtKeeper.estadisticas[i-1].tiempo_riego==i*60);
	 assertTrue(gtKeeper.estadisticas[i-1].tiempo_abono==i*60);
 }

 }