 
#include "main.h"
 #include "EEPROM.h"
//Variables
 char bufferapp[MAIN_BUFFER_SIZE];

uint8_t contador;
char buffercommand[25];
char buffer_test[MAIN_BUFFER_SIZE];

 


 /*
 EEMEM tConfiguracion eevars = {
	 "653316799",
	 "1111",
	 127,
	 "xxxxx",
	 "gprs.utebo.com",
	 "gprs.utebo.com",
	 "gprs.utebo.com",
	 1,
	 15,
	 1,
	 1,
	 0,
	 0,
	 0,
	 'X'
 };*/

//Creamos FakeStreams
FakeStream FakeGSMStream; //Fake Stream para GSM
FakeStream FakeloggerStream; //Fake Stream para Logger

 //Creamos objetos
LogSD SDCard(bufferapp,MAIN_BUFFER_SIZE );
Configuracion Config(bufferapp,MAIN_BUFFER_SIZE);
Riegos Riego(&Config.config,bufferapp,MAIN_BUFFER_SIZE );
GSM GSMModem (&Config.config,&Serial1,bufferapp,MAIN_BUFFER_SIZE );
GTKeeper gtKeeper(&Config,&GSMModem, &Riego,&SDCard, bufferapp,MAIN_BUFFER_SIZE);  

///puertos
const uint8_t ports[PORTS_NUM]= {PORT_SECTOR1_PIN };//,PORT_SECTOR2_PIN,PORT_SECTOR3_PIN};//{224,25,26,27,28,29,30,31,32,33,34,35,36,37,38 } ;
const uint8_t ports_abono[PORTS_ABONO]= {PORT_ABONO1_PIN,PORT_ABONO2_PIN } ;

//Interrupciones variables (Volatile)
volatile bool int_input_user=false; //User interaccion
volatile bool int_input_gsm=false;//GSM interaccion


//Creamos Logger ;)
//Stream *streamLog=&FakeloggerStream;
Stream *streamLog=&Serial;
Logger Log(streamLog);
 
//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}


int main() {
	//Necesario para la inicializacion de arduino
	init();
	//Setup
	setup();
	//Loop
	while (true)
	loop();
}

void setup()
{
	//Inicializamos puerto serie
	Serial.begin(9600);
	while (!Serial);
	
	Serial1.begin(9600);
	while (!Serial1);


	Test::out = streamLog;
	Test::exclude("*");
 	/*
	 while (!eeprom_is_ready());	
	 for (int i = 0 ; i < EEPROM.length() ; i++) {
		 EEPROM.write(i, 0);
	 }
 	
	 */
 
 
	//gtKeeper.Setup();
 
	//Test::include("sdlog*"); //Test de sd & Logs
	//Test::include("config*"); //Test de configuracion
	//Test::include("prog*"); //Test de programas
	//Test::include("esta*"); //Test de estadisticas
	//Test::include("sali*"); //Test de salidas
	//Test::include("riego*"); //Test de riegos

	Test::include("web_OnWeb"); //Test de webs

	//Test::include("config_EEPROMGuardaConfig");
	
	
	//Test::include("riego_EnciendeSector_ApagarRiegos");
	//Test::include("riego_multiple_cruzado");
	
	//Test::include("riego_multiple_cruzado");
	//Test::include("riego_multiples");
	
	//Test::include("dayToDiasSemana");
	//Test::include("elapsedSecsThisWeek2");
	//Test::include("dayOfWeek2*");
	//Test::include("getnextEjecucion*");*/
 
 
}



void loop()
{	
	Test::run();
 
}
 


 