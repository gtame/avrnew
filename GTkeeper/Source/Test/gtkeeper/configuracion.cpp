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
	Config.CargaConfigDesdeString(config);
	Config.ShowConfigInfo();
	pass();
}



test (config_ResetConfig)
{

	
	assertTrue(Config.CargaConfigDesdeString(config));
	assertTrue(strlen(Config.config.MovilAviso)>0);
	Config.ResetConfig();
	assertTrue(strlen(Config.config.MovilAviso)==0);
	pass();
}



test (config_CargaConfigDesdeSim)
{
	
	//Debe ser implementado
 	pass();

}


test  (config_CargaConfigDesdeString)
{

	assertFalse(Config.CargaConfigDesdeString("1asd679dfgd215"));
	Config.ResetConfig();
	assertTrue(Config.CargaConfigDesdeString(config));
	//CheckPhone
	assertTrue(strcmp(Config.config.MovilAviso ,"653316799")==0);
	
	//Check Avisos
	assertTrue(((SmsAviso)Config.config.AvisosSMS & SMSFinSector)>0);
	assertTrue(((SmsAviso)Config.config.AvisosSMS & SMSInicioSector)>0);
	assertTrue(((SmsAviso)Config.config.AvisosSMS & SMSProgramacionWeb)>0);
	assertTrue(((SmsAviso)Config.config.AvisosSMS & SMSReset)>0);
 
	//Check puertos
	assertTrue(Config.config.numabono == 2 );
	assertTrue(Config.config.motor_diesel == true );
	assertTrue(Config.config.numpuertos== 15);
	
 	pass();
}


test  (config_ConfiguracionToString)
{
	Config.ResetConfig();
	assertTrue(Config.CargaConfigDesdeString(config));
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Config.ConfiguracionToString(buffer_test);
	assertTrue(strlen(buffer_test)==LEN_CONFIG_STRING);
	
	//LOG_DEBUG_ARGS("%s vs %s",buffer_test,config);
	assertTrue(strcmp(buffer_test,config)==0);
 	pass();

}

test  (config_EEPROMGuardaConfig)
{
	Config.ResetConfig();
	assertTrue(Config.CargaConfigDesdeString(config));
	
	Config.EEPROMGuardaConfig();
	Config.ResetConfig();
	assertTrue(Config.EEPROMCargaConfig());
	
	memset(buffer_test,0,MAIN_BUFFER_SIZE);
	Config.ConfiguracionToString(buffer_test);
	assertTrue(strcmp(buffer_test,config)==0);
	
	pass();
}

test  (config_EEPROMCargaConfig)
{	
	//Es el mismo test q  config_EEPROMGuardaConfig
	skip();
	
}
