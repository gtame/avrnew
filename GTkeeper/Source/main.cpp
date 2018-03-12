/*
 * main.cpp


 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */
//Referencia a la libreria comun de avr
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
//Referencia al core de arduino
#include <Arduino.h>
#include <string.h>
#include "settings.h"
#include "gtKeeper.h"
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "main.h"
#include "maintest.h"




//Clase para hacer el log
#ifdef LOG
		//SoftwareSerial sserial(PIN_A0,PIN_A1);
		Stream *sserial=&Serial;
		Logger Log(&Serial);
#endif


//Clase para comunicar con el SIM900 (ACCESO A DATOS)
#ifdef SIM900_SOFTSERIAL
	SIM900 Sim900(3,4);
#else
	SIM900 Sim900(&Serial1);
#endif
//#include "memdebug.h"


#ifdef PANTALLA_TECLADO

LiquidCrystal_I2C lcd(0x20, 20, 4);//Display LCD I2C

//Pin Teclado

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {

	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};



byte colPins[COLS] =  { KEYBOARD_COL1_PIN,KEYBOARD_COL2_PIN,KEYBOARD_COL3_PIN,KEYBOARD_COL4_PIN};  //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {KEYBOARD_ROW1_PIN, KEYBOARD_ROW2_PIN, KEYBOARD_ROW3_PIN, KEYBOARD_ROW4_PIN};  //connect to the column pinouts of the keypad
 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


#endif


bool blnResetLoop=true;





//buffer para convertir Flashstring a buffer :P
//char p2b[MAIN_BUFFER_SIZE];


//OJO:Se probo poniendola como static dentro de la funcion pero daba problemas al asignarle un valor por segunda vez
//porque sino el linker da error!!!
//Using new, delete, and dynamic memory management generally is not recommended for small embedded
//devices because it can become a resource hog. If you must have it, create a .cpp file in the project
//source folder with this content:
/*
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" void * operator new(size_t size){  return malloc(size);}
extern "C" void operator delete(void * ptr){  free(ptr);}
extern "C" void * operator new[](size_t size){    return malloc(size);}
extern "C" void operator delete[](void * ptr){	if (ptr)free(ptr);}
extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);} ////Añadimos esta funcion para admitir variables static dentro de funcion
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
extern "C" void __cxa_guard_abort (__guard *) {}
*/

//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}




void setLed(uint8_t led )
{


//
//	if (led==LED_WORKING)
//		//No encendido
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//	else if (led==LED_ERROR_SIM)
//	{
//		//Parpadeo lento
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//		delay(2000);
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//		delay(2000);
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//		delay(2000);
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//		delay(2000);
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//		delay(2000);
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//	}
//	else if (led==LED_ERROR_NETWORK)
//	{
//		//Parpadeo rapido
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//		delay(500);
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//		delay(500);
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//		delay(500);
//		digitalWrite(PIN_ERROR_MODULE,LOW);
//		delay(500);
//		digitalWrite(PIN_ERROR_MODULE,HIGH);
//
//	}
//	else if (led==LED_ERROR_MODULE)
//		//Encendio permanente
//		digitalWrite(PIN_ERROR_MODULE,HIGH);

	(led!=LED_WORKING_PIN? digitalWrite(LED_WORKING_PIN,LOW):digitalWrite(LED_WORKING_PIN,HIGH));
	(led!=LED_ERROR_MODULE_PIN? digitalWrite(LED_ERROR_MODULE_PIN,LOW):digitalWrite(LED_ERROR_MODULE_PIN,HIGH));
	(led!=LED_ERROR_SIM_PIN? digitalWrite(LED_ERROR_SIM_PIN,LOW):digitalWrite(LED_ERROR_SIM_PIN,HIGH));
	(led!=LED_ERROR_NETWORK_PIN?digitalWrite(LED_ERROR_NETWORK_PIN,LOW):digitalWrite(LED_ERROR_NETWORK_PIN,HIGH));
}


bool ProcessATMensajes(char * msg)
{
	gtKeeper.ProcessATMensajes(msg);
}



void ResetConfigsCallBack(uint8_t button,uint8_t func)
{
	if (button==0)
	{
		//REseteamos todo, config, programas y estadisticas.
		gtKeeper.ResetConfig();
		gtKeeper.EEPROMGuardaConfig();
		gtKeeper.ResetProgramas();
		gtKeeper.ResetearEstadisticas();
	}
	//Flag para salir del bucle loop.
	blnResetLoop=false;
}




#ifdef PANTALLA_TECLADO
void setupPantallaTeclado()
{

	uint8_t line_num=0;

	const uint8_t NUM_INTENTOS=5;
	//Inicializamos interfaz de usuario
	//Esta llamada es necesaria pq sino da un error el linker en la lib LiquidCrystal_I2C
	//./LiquidCrystal_I2C.cpp:41: undefined reference to `Wire'
	Wire.begin();


	//lcd inicializacion ;>
	lcd.init();
	//Inicializamos el gestor de ventanas
	screenManager.Initializate(&lcd,20,4,&keypad);


	screenManager.Encender();

	LOG_DEBUG_B("CHECK RESET");
	time_t time=now();

	keypad.getKeys();
	int indexA= keypad.findInList('A');
	int indexD= keypad.findInList('D');

	//Si pulsamos XX SEG estas dos teclas se resetearan TODAS configs. :)
	while (blnResetLoop &&
			(keypad.key[indexA].kstate==PRESSED || keypad.key[indexA].kstate==HOLD) &&
			(keypad.key[indexD].kstate==PRESSED || keypad.key[indexD].kstate==HOLD)
		 )
	{
		if (ELAPSED_SECONDS(time)>10)
		{
			screenManager.ShowMsgBox_P(PSTR("Desea resetear las \nconfiguraciones?"),MsgYesNoButton, ResetConfigsCallBack);

			while (blnResetLoop)
				screenManager.Loop();
		}
		delay(500);
		keypad.getKeys();
	}


	LOG_DEBUG_B("CHECK TERMINADO");

	//Cargamos la configuracion del terminal
	 bool config_error=!gtKeeper.EEPROMCargaConfig();

	//Inicializamos gtKeeper
	 if (!config_error)
		 screenManager.PrintTextLine_P(line_num,TXT_CONFIG, TXT_OK);
	 else
	 {
		 gtKeeper.ResetConfig();
		 screenManager.PrintTextLine_P(line_num,TXT_CONFIG, TXT_ERROR);
	 }
	 line_num++;


	 LOG_DEBUG_B("ACTIVANDO GSM");
	//Configura puertos y demas
	gtKeeper.Initializate();

	//Comprobamos si tiene gsm configurado
	if (gtKeeper.IsGSMEnable())
	{

		LOG_DEBUG_B("ACTIVANDO GSM");
		bool blnOK=true;
		uint8_t num_veces=0;

		 //Arranca Modulo
		 while(!(blnOK=Sim900.ActivaModulo()) &&  num_veces<NUM_INTENTOS)
		 {
			 setLed(LED_ERROR_MODULE_PIN);
			 delay(500);
			 num_veces++;
		 }

		 if (blnOK)
			 screenManager.PrintTextLine_P(line_num,TXT_MOD_GSM, TXT_OK);
		 else
			 screenManager.PrintTextLine_P(line_num,TXT_MOD_GSM, TXT_ERROR);

		 line_num++;


		//SIM -- Ready¿?
		 if (blnOK)
		 {

			 num_veces=0;
			 while (!(blnOK=gtKeeper.CheckSIM())  &&  num_veces<NUM_INTENTOS)
			 {
				 setLed(LED_ERROR_SIM_PIN);
				 delay(500);
				 num_veces++;
			 }

			 if (blnOK)
				 screenManager.PrintTextLine_P(line_num,TXT_SIM, TXT_OK);
			 else

				 screenManager.PrintTextLine_P(line_num,TXT_SIM, TXT_OK);

			 line_num++;
		 }
		 else
			 LOG_DEBUG_B("GSM NO ACTIVO");



		 //Esperamos 2 seg, para que coga red..
		 delay(2000);


		 if (blnOK)
		 {

			 num_veces=0;
			//Network
			 while (!(blnOK=gtKeeper.EstaRegistradoGSM()) &&  num_veces<NUM_INTENTOS)
			 {
				 setLed(LED_ERROR_NETWORK_PIN);
				 delay(500);
				 num_veces++;
			 }

			 if (blnOK)
				 screenManager.PrintTextLine_P(line_num,TXT_CONECTIVIDAD,TXT_OK);
			 else
				 screenManager.PrintTextLine_P(line_num,TXT_CONECTIVIDAD,TXT_ERROR);

			 line_num++;
		 }







		//eNVIA SMS REINICIO

		 //Antes de hacer un clear de la pantalla esperamos 1 seg.


		//Fijar Hora desde red GSM
		 if (blnOK)
		 {


			lcd.clear();
			line_num=0;
			if (gtKeeper.config.AvisosSMS & SMSReset)
			{
				 //Esperamos 2 seg SMS
		 		 delay(2000);

				screenManager.PrintTextLine_P(line_num,TXT_SMS, PSTR(""));
				//Envio mail de aviso
				gtKeeper.SendSmsIniReinicio();
				screenManager.PrintTextLine_P(line_num,TXT_SMS, TXT_OK);
				line_num++;
			}


			 num_veces=0;
			 delay(5000);
			//Network
			 while (!(blnOK=gtKeeper.FijarHoraGSM()) &&  num_veces<NUM_INTENTOS)
			 {
				 delay(500);
				 num_veces++;
			 }



			if (blnOK)
				screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_OK);
			else
				screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_ERROR);

			line_num++;
		 }
	}
	else
	{
		screenManager.PrintTextLine_P(line_num,TXT_RELOJ, TXT_ERROR);
		line_num++;
	}


	if (line_num==4)
		lcd.clear();



	//Cargamos programas
	if (gtKeeper.EEPROMCargaProgramas())
		screenManager.PrintTextLine_P(line_num,TXT_PROGRAMAS, TXT_OK);
	else
		screenManager.PrintTextLine_P(line_num,TXT_PROGRAMAS, TXT_ERROR);


#ifdef PROTEUS
	if (!gtKeeper.EstaEnHora())
	{
    TimeElements timeEl;
    timeEl.Day=8;
    timeEl.Month=4;
    timeEl.Year=y2kYearToTm(16);
    timeEl.Hour=23;
    timeEl.Minute=38;

	gtKeeper.SetHora(makeTime(timeEl));

	}

	screenManager.SetTimeInactivity(0);
#endif


	//Avisa que ya se ha acabado la incializacion de gtkeeper
	gtKeeper.EndInitializate();





	if (strlen(gtKeeper.config.APN)==0)
	{
		strcpy(gtKeeper.config.APN,"gprs-service.com");
	}

	//Si no hemos conseguido ponerlo en hora , la solicitaremos manualmente
	if (gtKeeper.EstaEnHora())
		screenManager.SetPantallaActual((ScreenBase*)&menuScreen);
	else
		screenManager.SetPantallaActual((ScreenBase*)&fechahoraScreen);


	if (config_error)
		screenManager.ShowMsgBox_P(PSTR("Error configuracion\nParametros->Sistema"),MsgOkButton,NULL);



	//Fijamos la pantalla actual..
	 if (!config_error)
		 //Ajustamos Config Web
		 gtKeeper.CargaConfigWeb();


	//eNVIA SMS REINICIO ALL
	if (gtKeeper.IsGSMEnable())
			gtKeeper.SendSmsFinReinicio();

}
#endif



#ifndef PANTALLA_TECLADO
#ifndef TEST
void setupSimple()
{
 const uint8_t NUM_INTENTOS=5;

		LOG_DEBUG_B("CHECK RESET");
		//Si pulsamos XX SEG boton de reset.. nos cargaremos la configuracion:)
		time_t time=now();
		while (digitalRead(PIN_RESET)==HIGH)
		{
			if (ELAPSED_SECONDS(time)>10)
			{
				ResetConfigsCallBack(0,0);

			}
			delay(500);
		}
		LOG_DEBUG_B("CHECK RESET TERMINADO");



		LOG_DEBUG_B("CARGANDO CONFIG");
		//Cargamos la configuracion del terminal
		bool config_error=!gtKeeper.EEPROMCargaConfig();
		if (config_error)
		{
			LOG_INFO("Error cargando config!");
			gtKeeper.ResetConfig();
		}

		LOG_DEBUG_B("CARGANDO CONFIG TERMINADO");



		LOG_DEBUG_B("INICIALIZANDO RELES");
		//Configura puertos y demas
		gtKeeper.Initializate();
		LOG_DEBUG_B("INICIALIZANDO RELES TERMINADO");



			//Comprobamos si tiene gsm configurado
			if (gtKeeper.IsGSMEnable())
			{

				LOG_DEBUG_B("ACTIVANDO GSM");
				bool blnOK=true;
				uint8_t num_veces=0;

				 //Arranca Modulo
				 while(!(blnOK=Sim900.ActivaModulo()) &&  num_veces<NUM_INTENTOS)
				 {
					 setLed(LED_ERROR_MODULE);
					 delay(500);
					 num_veces++;
				 }

				 if (!blnOK)
					 setLed(LED_ERROR_MODULE);


				//SIM -- Ready¿?
				 if (blnOK)
				 {

					 num_veces=0;
					 while (!(blnOK=gtKeeper.CheckSIM())  &&  num_veces<NUM_INTENTOS)
					 {
						 setLed(LED_ERROR_SIM);
						 delay(500);
						 num_veces++;
					 }

					 if (!blnOK)
						 setLed(LED_ERROR_SIM);
				 }
				 else
					 LOG_DEBUG_B("GSM NO ACTIVO");



				 //Esperamos 2 seg, para que coga red..
				 delay(2000);


				 if (blnOK)
				 {

					 num_veces=0;
					//Network
					 while (!(blnOK=gtKeeper.EstaRegistradoGSM()) &&  num_veces<NUM_INTENTOS)
					 {
						 setLed(LED_ERROR_NETWORK);
						 delay(500);
						 num_veces++;
					 }

					 if (blnOK)
						 setLed(LED_ERROR_NETWORK);
				 }






				//Fijar Hora desde red GSM
				 if (blnOK)
				 {


					 num_veces=0;
					 delay(10000);
					//Network
					 while (!(blnOK=gtKeeper.FijarHoraGSM()) &&  num_veces<NUM_INTENTOS)
					 {
						 delay(500);
						 num_veces++;
					 }


					if (gtKeeper.config.AvisosSMS & SMSReset)
					{
						 //Esperamos 2 seg SMS
				 		 delay(2000);
						//Envio mail de aviso
						gtKeeper.SendSmsIniReinicio();

					}


					if (blnOK)
						 setLed(LED_ERROR_NETWORK);
				 }
			}





			//Cargamos programas
			if (gtKeeper.EEPROMCargaProgramas())
				 setLed(LED_ERROR_NETWORK);


		#ifdef PROTEUS
			if (!gtKeeper.EstaEnHora())
			{
		    TimeElements timeEl;
		    timeEl.Day=8;
		    timeEl.Month=4;
		    timeEl.Year=y2kYearToTm(16);
		    timeEl.Hour=23;
		    timeEl.Minute=38;

			gtKeeper.SetHora(makeTime(timeEl));

			}

			screenManager.SetTimeInactivity(0);
		#endif


			//Avisa que ya se ha acabado la incializacion de gtkeeper
			gtKeeper.EndInitializate();

			//eNVIA SMS REINICIO ALL
		if (gtKeeper.IsGSMEnable())
				gtKeeper.SendSmsFinReinicio();


}
#endif
#endif





void setup() {



	delay(200);
	//pinMode(PIN_A0, INPUT);

	//sserial.begin(9600);

	//Inicializamos puertos series
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial2.begin(9600);

	//Establecemos los punteros a funcion
	Sim900.ProcessResultPtr = ProcessATMensajes;
	gtKeeper.ChangeStatus=setLed;






#ifdef TEST
	setupTest();
#else

	#ifdef PANTALLA_TECLADO
		setupPantallaTeclado();
	#else
		setupSimple();
	#endif
#endif



}
/*
char * P2B(const __FlashStringHelper * __fmt,...)
{


	memset(buff_parse,0,MAIN_BUFFER_PARSE);
	memset(p2b,0,MAIN_BUFFER_SIZE);

	va_list ap;
	strcpy_P (buff_parse, (char*)__fmt);
	va_start(ap,buff_parse);
	vsnprintf(p2b, MAIN_BUFFER_SIZE,buff_parse, ap);

	va_end(ap);

	return p2b;

}
*/

#ifdef TEST




#else


//Ultima vez que se comprobo el loop
time_t lastgtLoop=0;




void loop() //----( LOOP: RUNS CONSTANTLY )----
{

	bool hashError=false;


#ifdef PROTEUS
	//Cada vez que pasa por CheckRiegos LE añadimos 5 seg
	//si valor==30 aprox 11 seg x min
	static uint8_t valor=0;
	if (valor==15)
	{
		setTime(now()+1);
		valor=0;
	}
	else
		valor++;
#endif

 	if (gtKeeper.EstaEnHora())
		gtKeeper.CheckRiegos(!SCREEN_ACTIVE());


#ifdef PANTALLA_TECLADO
	//Procesamos el teclado-pantalla

	//LOG_DEBUG("INI SCREEN LOOP");

 	//char c=keypad.waitForKey();
 	//Log.Debug("Key %c",c);

	//LOG_DEBUG("FIN SCREEN LOOP");
	screenManager.Loop();

#endif


	if (!SCREEN_ACTIVE())
	{


		//Si no esta activa la consola no se testea web mas alla del process
		if ( ELAPSED_SECONDS(lastgtLoop)>15 )
		{

			//LOG_DEBUG("INI GTKEEPER LOOP");

				hashError=gtKeeper.Loop();
				lastgtLoop=now();
			//	LOG_DEBUG("FIN GTKEEPER LOOP");

				//LOG_DEBUG_ARGS("Cobertura %i",Sim900.GetCobertura());
		}
	}


	//Si no hay errores
	if (!hashError)
	{
		if (gtKeeper.IsGSMEnable())
		{
			//LOG_DEBUG("INI PROCESS LOOP");
			if(SCREEN_ACTIVE())
				Sim900.ProcessResults(1);
			else
				Sim900.ProcessResults(1000);
		}

		//LOG_DEBUG("FIN PROCESS LOOP");
	}

//#endif
	//LOG_INFO("======FIN LOOP=======");

}

#endif



int main() {

	//Necesario para la inicializacion de arduino
	init();

	//Setup
	setup();


	delay(100);
	//Loop
	while (true)
 		loop();

}
