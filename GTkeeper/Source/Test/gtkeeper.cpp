/*
 * gtkeeper.cpp
 *
 * Created: 24/03/2018 7:12:26
 *  Author: Gabi
 */ 
 #include "main.h"

 void InitializeWeb()
 {
	FijarFecha(2018,1,1);
	 Riego.ApagarRiegos();//Limpia salidas
	Riego.ResetProgramas();//Resetea programas
	gtKeeper.UpdateWebSuccess();//Esto es para inicializar vars de control de errores etc..
 }

 test(web_CheckWeb_Changed)
 {
	InitializeWeb();
	
	assertFalse(gtKeeper.CheckWeb()); //Por defecto false;

	

	 //Cargamos configuracion 
	 Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	 assertTrue(Config.GetChangedConfig());
	  //Deberia ser true ;)
	 assertTrue(gtKeeper.CheckWeb());
	  Config.EEPROMGuardaConfig();
	 //Ya no deberia estar pte por haber guardado
	 assertFalse(gtKeeper.CheckWeb()); 


	 //Hacemos lo mismo con programas
	 Riego.CargaProgramaDesdeString(1,"03127120001200000");
 	//Deberia ser true ;)
 	assertTrue(gtKeeper.CheckWeb());
	Riego.GuardarProgramasEEPROM();
	 //Ya no deberia estar pte por haber guardado
	assertFalse(gtKeeper.CheckWeb());


	 
 }

 test(web_CheckWeb_errores)
 {
	 InitializeWeb();
	 
	 assertFalse(gtKeeper.CheckWeb()); //Por defecto false;

	 //Cargamos configuracion
	 Config.CargaConfigDesdeString("12765331679911111215");
	 assertTrue(Config.GetChangedConfig());
	 //Deberia ser true ;)
	 assertTrue(gtKeeper.CheckWeb());

	 
	 //Si añadimos un error web
	 gtKeeper.AddErrorWeb();
	 assertFalse(gtKeeper.CheckWeb());

	 //Ahora deberia tragar
	 //Adelantamos el tiempo
	 setTime(now()+WEB_ERROR_SEND_TIME+1);
	 assertTrue(gtKeeper.CheckWeb());

 }

//Test para comprobar que la solicitud se realiza correctamente enviando solo logs
 test(web_OnWeb_logs)
 {
   
	//Cargamos configuracion
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Config.ResetConfig();
	Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	assertTrue(Config.GetChangedConfig());

	//Escribimos Log
	assertTrue(SDCard.ClearLogs());
	assertTrue(SDCard.WriteLog("Prueba escritura")!=-1);

	Config.config.lastupdateconfig=1;
	Config.config.lastupdateprog=1;
	GSMModem.GetIMEI(Config.config.Imei);
	LOG_DEBUG_ARGS("Imei %s",Config.config.Imei);

	//Realizamos solicitud
	gtKeeper.OnWeb();

	//Check! - Acaba perfect, no hay errores!!! :D
	assertTrue(gtKeeper.GetErroresWeb()==0);
	
	//No deberia tragar
	assertFalse(gtKeeper.CheckWeb());



 }
 
 //Test para comprobar que la solicitud se realiza correctamente enviando salidas y logs
 test(web_OnWeb_salidas_logs)
 {
	 
	 //Cargamos configuracion
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 Config.ResetConfig();
	 Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	 assertTrue(Config.GetChangedConfig());

	 //Escribimos Log
	 assertTrue(SDCard.ClearLogs());
	 assertTrue(SDCard.WriteLog("Prueba escritura")!=-1);

	 Config.config.lastupdateconfig=1;
	 Config.config.lastupdateprog=1;
	 GSMModem.GetIMEI(Config.config.Imei);
	 LOG_DEBUG_ARGS("Imei %s",Config.config.Imei);

	 //Arrancamos un riego
	 Riego.EnciendePrograma(0);
	 // Riego.ApagaPrograma(0);

	 //Adelantamos el tiempo , para que CheckWeb si que trague
	 setTime(now()+WEB_ERROR_SEND_TIME+1);
	 assertTrue(gtKeeper.CheckWeb());

	 //Deberia realizarse la solicitud correctamente
	 gtKeeper.OnWeb();

	 //Acaba perfect, no hay errores!!! :D
	 assertTrue(gtKeeper.GetErroresWeb()==0);


 }
 
   
  //Test para comprobar que procesa correctamente la respuesta del servidor utilizando peticion GET
  test(web_OnWeb_programas_read)
  {
	  //03101010120001200000
	  //03->Sector
	  //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
	  //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
	  //0120 -> Tiempo de riego
	  //0000 -> Tiempo de abono
	  char programa[]="03127120001200000";
	  //Cargamos configuracion
	  memset(buffer_test,0,MAIN_BUFFER_SIZE);
	  Config.ResetConfig();
	  Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	  assertTrue(Config.GetChangedConfig());
	  
	  /*
		for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
		{
			assertTrue(Riego.CargaProgramaDesdeString(i,programa));
			Riego.programas[i].Sector= i;
			Riego.programas[i].TiempoRiego=i*60;//Incrementamos 1' a cada sector
		}*/
		
		//Riego.ShowInfoProgramas();


	  //Limpiamos Log
	  assertTrue(SDCard.ClearLogs());
	 // assertTrue(SDCard.WriteLog("Prueba escritura")!=-1);

	  //Config.config.lastupdateconfig=1;
	 // Config.config.lastupdateprog=1;
	  GSMModem.GetIMEI(Config.config.Imei);
	  LOG_DEBUG_ARGS("Imei %s",Config.config.Imei);

	  //Arrancamos un riego
	  //Riego.EnciendePrograma(0);
	  // Riego.ApagaPrograma(0);

	  //Adelantamos el tiempo , para que CheckWeb si que trague
	  setTime(now()+WEB_ERROR_SEND_TIME+1);
	  assertTrue(gtKeeper.CheckWeb());

	  //Deberia realizarse la solicitud correctamente
	  gtKeeper.OnWebGet();

	  //Acaba perfect, no hay errores!!! :D
	  assertTrue(gtKeeper.GetErroresWeb()==0);


  }
  
  
    //Test para comprobar que procesa correctamente la solicitud POST al servidor enviando los programas
  test(web_OnWeb_programas_read_write)
  {
	  //03101010120001200000
	  //03->Sector
	  //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
	  //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
	  //0120 -> Tiempo de riego
	  //0000 -> Tiempo de abono
	  char programa[]="03127120001200000";
	  //Cargamos configuracion
	  memset(buffer_test,0,MAIN_BUFFER_SIZE);
	  Config.ResetConfig();
	  Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	  assertTrue(Config.GetChangedConfig());
	  
	  
		for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
		{
			assertTrue(Riego.CargaProgramaDesdeString(i,programa));
			Riego.programas[i].Sector= i;
			Riego.programas[i].TiempoRiego=i*60;//Incrementamos 1' a cada sector
			Riego.programas[i].HoraInicio= i;
			Riego.programas[i].MinutoInicio= i;
			Riego.programas[i].TiempoAbono=i*60;//Incrementamos 1' a cada sector
		}
		
		
		
	   Riego.ShowInfoProgramas();


	  //Limpiamos Log
	  assertTrue(SDCard.ClearLogs());
	 // assertTrue(SDCard.WriteLog("Prueba escritura")!=-1);

	  //Config.config.lastupdateconfig=1;
	 // Config.config.lastupdateprog=1;
	  GSMModem.GetIMEI(Config.config.Imei);
	  LOG_DEBUG_ARGS("Imei %s",Config.config.Imei);

	  //Arrancamos un riego
	  //Riego.EnciendePrograma(0);
	  // Riego.ApagaPrograma(0);

	  //Adelantamos el tiempo , para que CheckWeb si que trague
	  setTime(now()+WEB_ERROR_SEND_TIME+1);
	  assertTrue(gtKeeper.CheckWeb());

	  //Deberia realizarse la solicitud correctamente
	  gtKeeper.OnWeb();

	  //Acaba perfect, no hay errores!!! :D
	  assertTrue(gtKeeper.GetErroresWeb()==0);


	//REseteamos los programas
	Riego.ResetProgramas();
	//Verificamos que estan reseteados
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.programas[i].Sector==0);
	}
	//Refrescamos desde el servidor
	gtKeeper.OnWebGet();
	//Comparamos que los datos esperados son correctos
	for(uint8_t i=0;i<MAX_PROGRAMAS;i++)
	{
		assertTrue(Riego.programas[i].Sector== i);
		assertTrue(Riego.programas[i].TiempoRiego== i*60);//Incrementamos 1' a cada sector
		assertTrue(Riego.programas[i].HoraInicio== i);
		assertTrue(Riego.programas[i].MinutoInicio== i);
		assertTrue(Riego.programas[i].TiempoAbono==i*60);//Incrementamos 1' a cada sector
	}
	
	
  }
  
  