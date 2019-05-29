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
 


//Test para comprobar que la solicitud se realiza correctamente
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
 
 
 
 
 //Test para comprobar que la solicitud se realiza correctamente
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

	 //Deberian
	 gtKeeper.OnWeb();

	 //Acaba perfect, no hay errores!!! :D
	 assertTrue(gtKeeper.GetErroresWeb()==0);


 }