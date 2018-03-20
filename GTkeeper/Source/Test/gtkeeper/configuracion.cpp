/*
 * configuracion.cpp
 *
 * Created: 20/03/2018 7:53:52
 *  Author: gtame
 */ 
/*
 * sdlog.cpp
 *
 * Created: 19/03/2018 12:36:50
 *  Author: gtame
 */ 
#include "..\main.h"



//12765331679911111215
//3 para AvisosSMS
//6 siguientes para movil
//NUM 4 password sms
//BOOL 1 o 0 para motor diesel
//NUM 1 para abono
//NUM 2 para puertos

/*
	SMSNone =0,//
	SMSInicioSector = (1 << 0),//1
	SMSFinSector= (1 << 1),//2
	SMSProgramacionWeb= (1 << 2),//4
	SMSReset= (1 << 3),//8
	SMSReservado1= (1 << 4),//16
	SMSReservado2= (1 << 5),//32
	SMSReservado3= (1 << 6)//64

*/
char config[]="12765331679911111215"; 

test (config_ShowConfigInfo)
{
	gtKeeper.CargaConfigDesdeString(config);
	gtKeeper.ShowConfigInfo();
	pass();
}



test (config_ResetConfig)
{

	
	assertTrue(gtKeeper.CargaConfigDesdeString(config));
	assertTrue(strlen(gtKeeper.config.MovilAviso)>0);
	gtKeeper.ResetConfig();
	assertTrue(strlen(gtKeeper.config.MovilAviso)==0);
	pass();
}



test (config_CargaConfigDesdeSim)
{
	
	//Debe ser implementado
 	pass();

}


test  (config_CargaConfigDesdeString)
{

	assertFalse(gtKeeper.CargaConfigDesdeString("1asd679dfgd215"));
	gtKeeper.ResetConfig();
	assertTrue(gtKeeper.CargaConfigDesdeString(config));
	//CheckPhone
	assertTrue(strcmp(gtKeeper.config.MovilAviso ,"653316799")==0);
	
	//Check Avisos
	assertTrue(((SmsAviso)gtKeeper.config.AvisosSMS & SMSFinSector)>0);
	assertTrue(((SmsAviso)gtKeeper.config.AvisosSMS & SMSInicioSector)>0);
	assertTrue(((SmsAviso)gtKeeper.config.AvisosSMS & SMSProgramacionWeb)>0);
	assertTrue(((SmsAviso)gtKeeper.config.AvisosSMS & SMSReset)>0);
 
	//Check puertos
	assertTrue(gtKeeper.config.numabono == 2 );
	assertTrue(gtKeeper.config.motor_diesel == true );
	assertTrue(gtKeeper.config.numpuertos== 15);
	
 	pass();
}


test  (config_ConfiguracionToString)
{
	gtKeeper.ResetConfig();
	assertTrue(gtKeeper.CargaConfigDesdeString(config));
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	gtKeeper.ConfiguracionToString(buffer_test);
	assertTrue(strlen(buffer_test)==LEN_CONFIG_STRING);
	
	//LOG_DEBUG_ARGS("%s vs %s",buffer_test,config);
	assertTrue(strcmp(buffer_test,config)==0);
 	pass();

}

test  (config_EEPROMGuardaConfig)
{
	gtKeeper.ResetConfig();
	assertTrue(gtKeeper.CargaConfigDesdeString(config));
	
	gtKeeper.EEPROMGuardaConfig();
	gtKeeper.ResetConfig();
	assertTrue(gtKeeper.EEPROMCargaConfig());
	
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	gtKeeper.ConfiguracionToString(buffer_test);
	assertTrue(strcmp(buffer_test,config)==0);
	
	pass();
}

test  (config_EEPROMCargaConfig)
{	
	//Es el mismo test q  config_EEPROMGuardaConfig
	skip();
	
}
