/*
 * CPPFile1.cpp
 *
 * Created: 13/03/2018 17:43:09
 *  Author: Gabi
 */ 
 #include <gtkeeper.h>

 
 const char  keys[KEYPAD_ROWS][KEYPAD_COLUMNS] = {

	 {'1','2','3','A'},
	 {'4','5','6','B'},
	 {'7','8','9','C'},
	 {'*','0','#','D'}
 };

 //connect to the row pinouts of the keypad
byte colPins[KEYPAD_COLUMNS] =  { KEYBOARD_COL1_PIN,KEYBOARD_COL2_PIN,KEYBOARD_COL3_PIN,KEYBOARD_COL4_PIN};
//connect to the column pinouts of the keypad
byte rowPins[KEYPAD_ROWS] = {KEYBOARD_ROW1_PIN, KEYBOARD_ROW2_PIN, KEYBOARD_ROW3_PIN, KEYBOARD_ROW4_PIN};
//Inicializacion para I2C
LiquidCrystal_I2C lcd(0x20, 20, 4);//Display LCD I2C
//Instanciamos Keypad
Keypad keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLUMNS);
//Instanciamos Logger
Logger Log(&Serial);
const uint8_t GTKeeper::ports[PORTS_NUM]= {PORT_SECTOR1_PIN };//,PORT_SECTOR2_PIN,PORT_SECTOR3_PIN};//{224,25,26,27,28,29,30,31,32,33,34,35,36,37,38 } ;
const uint8_t GTKeeper::ports_abono[PORTS_ABONO]= {PORT_ABONO1_PIN,PORT_ABONO2_PIN } ;
 
 //Interrupciones variables (Volatile)
 volatile bool int_input_user=false; //User interaccion
 volatile bool int_input_gsm=false;//GSM interaccion

bool GTKeeper::CheckON()
{
	return true;
}

void GTKeeper::OnON()
  {
		//Inicializamos los puertos series
		Serial.begin(9600);
		Serial1.begin(9600);
		Serial2.begin(9600);

		LOG_DEBUG("OnON");
		
	 
		//Inicializamos variables
		t_last_web=0; //Tiempo para controlar los tiempos de la web.
		last_RiegosCheck =0;
		stop_abono=0;
		error_web=0;//Numero de errores que se producen al intentar acceder al dispositivo
		salidas_activas=0;//Al arrancar no hay salidas activas ;)

		bSetupCompleted=false; //Flag para indicar que ya esta dentro del bloquee loop, y el terminal esta configurado
		//bWebInProcess=false;//Flag para indicar que estamos actualmente refrescando desde la web
		bpendingWeb=false;//Flag para indicar que es necesario el update de web
		bRebootSIM=false;//Flag para indicar que hemos reiniciado el modulo GSM, y necesitamos reconfigurarlo
		isHourSet=false;//Flag para saber si se ha fijado la hora
		//salidas_web=0; //Numero de salidas pendientes de ser enviadas x WEB

		//Limpiamos buffer
		memset(buffer,0,MAIN_BUFFER_SIZE);
		memset(buff_parse,0,MAIN_BUFFER_PARSE);

		ResetConfig();


		Wire.begin();//Necesario para comunicacion I2C
		//Inicializacion LCD
		lcd.init();
		//Inicializamos el gestor de ventanas
		screenManager.Initializate(&lcd,20,4,&keypad);
	   
  }
	 
	  
	  
 void GTKeeper::OnLeaveON()
 {
	LOG_DEBUG("OnLeaveOn");

 /*
	
	 */
	 //Callbacks o punteros a funcion
	 //ProcessResultPtr = ProcessATMensajesCallback;
	 //gtKeeper.ChangeStatus=setLed;

	 
 }