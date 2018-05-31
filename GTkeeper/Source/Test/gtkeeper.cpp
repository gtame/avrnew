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


 test(web_OnWeb)
 {
   // GSMModem modem;
	//assertTrue( PostHttpResultCallback("http://loquesea",6)==LOAD_WEB_OK);

	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Config.ResetConfig();
	//assertTrue(Config.GetChangedConfig());
	

	
	Config.CargaConfigDesdeString(CONST_CONFIG_STRING);
	assertTrue(Config.GetChangedConfig());

	

	
	
	
	assertTrue(SDCard.ClearLogs());
	assertTrue(SDCard.WriteLog("claro que hace")!=-1);

	LOG_DEBUG_ARGS("COnfig %i",Config.GetChangedConfig());
	
	
	Config.config.lastupdateconfig=1;
	Config.config.lastupdateprog=1;
	GSMModem.GetIMEI(Config.config.Imei);
	LOG_DEBUG_ARGS("Imei %s",Config.config.Imei);

	gtKeeper.OnWeb();

	//Acaba perfect, no hay errores!!! :D
	assertTrue(gtKeeper.GetErroresWeb()==0);


	 Riego.EnciendePrograma(0);

	//Ahora deberia tragar
	//Adelantamos el tiempo
	setTime(now()+WEB_ERROR_SEND_TIME+1);
	assertTrue(gtKeeper.CheckWeb());



	
	gtKeeper.OnWeb();

	//Acaba perfect, no hay errores!!! :D
	assertTrue(gtKeeper.GetErroresWeb()==0);


 }