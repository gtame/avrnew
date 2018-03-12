/*
 * main.cpp
 *
 *  Created on: 17/10/2015
 *  Author: Gabriel Tamé
 */


/*-----( Import needed libraries )-----*/

#include <string.h>

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "hilo.h"
#include "tonos_const.h"
#include <SoftwareSerial.h>

#include <SnoozeLib.h>//Include the library


//#include "constants.h"
#define BATTERY_RATIO  2.35  //1.78 //2.35 // Change this to match the MEASURED ration of the circuit
#define VOLTAGE_VALUE_AD 0.00488      //  Calculate the voltage on the A/D pin
                                    //  A reading of 1 for the A/D = 0.0048mV


                                    //  if we multiply the A/D reading by 0.00488 then
                                    //  we get the voltage on the pin.
  //Esta constante sale de dividir los 5v /1024 (2^10) ya que los pines analogicos del micro leen valores de 0 a 1024.
  //Entonces cada 0.00488V que se incrementa , incrementa 1 el valor de la lectura analogica.

#define DEBUG 1
#define VCC_DIVIDER_PIN 5
#define READ_VCC_PIN A0


#define VCC_WATER_SENSOR 6
#define READ_WATER_SENSOR2 A2
#define READ_WATER_SENSOR A1


//#define SPEAKER_PIN 9
#define LED_WORKING 10
#define LED_ERROR_MODULE 11
#define LED_ERROR_SIM 12
#define LED_ERROR_NETWORK 13


#define LED_BATTERY_LOW 7
#define LED_BATTERY_MEDIUM 8
#define LED_BATTERY_HIGH 9


#define VOLT_RANGE_LOW 6 //Voltaje para indicar como estado de carga bajo
#define VOLT_RANGE_HIGH 6.5 //Voltaje para considerar carga alta ((Entre alto&& bajo lo considera medio)


#define PARSE_TO_SECONDS(a) ((a)/1000)
#define MAX_SECONDS_ELAPSED 120

//Funciones
boolean isExposedToWater();
void PlayMelody();
void setLed(uint8_t led );
void setLedBattery(float batteryVoltage);
bool CheckSIM();
void ParseFloat(float f,int decimals,char * result);
float GetCargaBateria();
//Funcion necesaria para el compilador
unsigned long startTime=0;
uint8_t currentled=0; //actual led de status encendido

// notes in the melody:
const int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
const int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

extern "C" void __cxa_pure_virtual() {

}



void setLedBatteryOff()
{
	digitalWrite(LED_BATTERY_LOW,HIGH);
	digitalWrite(LED_BATTERY_MEDIUM,HIGH);
	digitalWrite(LED_BATTERY_HIGH,HIGH);
}

void setLedBattery(float batteryVoltage)
{
	setLedBatteryOff();

  if (batteryVoltage<=VOLT_RANGE_LOW) //Esta seca?
	  digitalWrite(LED_BATTERY_LOW,LOW);
  else if (batteryVoltage>=VOLT_RANGE_HIGH)//ESta muy cargada?
	  digitalWrite(LED_BATTERY_HIGH,LOW);
  else//Medium
	  digitalWrite(LED_BATTERY_MEDIUM,LOW);
}


void setLed(uint8_t led )
{
	if (led!=0)
		currentled=led;

	(led!=LED_WORKING? digitalWrite(LED_WORKING,HIGH):digitalWrite(LED_WORKING,LOW));
	(led!=LED_ERROR_MODULE? digitalWrite(LED_ERROR_MODULE,HIGH):digitalWrite(LED_ERROR_MODULE,LOW));
	(led!=LED_ERROR_SIM? digitalWrite(LED_ERROR_SIM,HIGH):digitalWrite(LED_ERROR_SIM,LOW));
	(led!=LED_ERROR_NETWORK?digitalWrite(LED_ERROR_NETWORK,HIGH):digitalWrite(LED_ERROR_NETWORK,LOW));


}


void pressButton(){
	//Interrupt Handle Function
	LogInfo("Arrancando1");
	snoozeLib.wakeup(); //WakeUp The CPU
	LogInfo("Arrancando2");
}


void setup() {


	wdt_reset();
	wdt_disable();


	//TODO: jaja
	Serial.begin(9600);
	mySerial.begin(9600);


	//Instalamos interrupcion
	attachInterrupt(digitalPinToInterrupt(2),pressButton , LOW);


#ifdef DEBUG
	LogInfo("Arrancando modo DEBUG!");
#else
	LogInfo("Arrancando..");
#endif

	DesconectaModulo();




	pinMode(VCC_DIVIDER_PIN,OUTPUT);

	pinMode(READ_WATER_SENSOR, INPUT);

	//pinMode(SPEAKER_PIN, OUTPUT);
	pinMode(LED_WORKING, OUTPUT);
	pinMode(LED_ERROR_MODULE, OUTPUT);
	pinMode(LED_ERROR_SIM, OUTPUT);
	pinMode(LED_ERROR_NETWORK, OUTPUT);
	setLed(0);

	pinMode(LED_BATTERY_LOW,OUTPUT);
	pinMode(LED_BATTERY_MEDIUM,OUTPUT);
	pinMode(LED_BATTERY_HIGH,OUTPUT);
	setLedBatteryOff();


	digitalWrite(VCC_DIVIDER_PIN, HIGH);


	pinMode(6, OUTPUT);
	digitalWrite(VCC_WATER_SENSOR,LOW);


	//digitalWrite(SPEAKER_PIN,LOW);

#ifndef DEBUG
	 delay(3000);
#endif

	digitalWrite(LED_ERROR_MODULE, HIGH);
	digitalWrite(LED_ERROR_SIM, HIGH);
	digitalWrite(LED_ERROR_NETWORK,HIGH);
	digitalWrite(LED_WORKING, HIGH);

#ifndef DEBUG
	delay(5000);
#endif

	//Esperamos 1 segundos para condensador de Vcc
	float carga=GetCargaBateria();
	setLedBattery(carga);

	//Esperamos 2 segundos para condensador de Vcc

	uint8_t veces=0;
	if (carga<5.8)
	{
		//Si es menor que 5.8 Danger!!! esto esta seco!!!
		while ( GetCargaBateria()<5.8 && veces<10)
		{
			veces++;
			delay(100);
		}
	}

	//Si esta seco, lo dejamos parapadeando :(
	while (veces>=10)
	{
		LogInfo("Parpadeo..");

		//Encender todos leds
		setLedBattery(5);
		digitalWrite(LED_ERROR_MODULE, HIGH);
		digitalWrite(LED_ERROR_SIM, HIGH);
		digitalWrite(LED_ERROR_NETWORK,HIGH);
		digitalWrite(LED_WORKING, HIGH);
		delay(500);

		//Apagar Leds
		setLedBatteryOff();
		setLed(0);
		delay(500);
	}

#ifndef DEBUG
	//Esperarmos 10seg - para que se cargen los condensadores
	delay(4000);
#endif


	//Apagar Leds
	setLedBatteryOff();
	setLed(0);

	//Arrancamos modulo GSM
	//attachInterrupt(0,wakeup,RISING);//Attaching the 0 Interrupt(on Arduino UNO pin 2) for RISING.

}


//Si la SIM esta lista, y es la primera vez
//verificamos que haya un contacto al que llamar
bool CheckSIM()
{
	static bool firstCheck=true;
	bool result=SIMEstaLista();
	if (result && firstCheck)
	{
		if (ExisteContactoSIM(1))
			firstCheck=false;
		else
			result=false;
	}
	return result;
}




void loop() //----( LOOP: RUNS CONSTANTLY )----
{

	LogInfo("Prueba");

	//Para ahorrar bateria activamos modo ahorro de energia durante 1 minuto

#ifndef DEBUG
	  snoozeLib.snooze(60000);//Snooze the CPU
#else
	snoozeLib.snooze(1000);//Snooze the CPU
#endif



	//Chequeamos si detecta agua
	if (isExposedToWater())
	{


		LogInfo("Mojado!");
		unsigned long seconds=PARSE_TO_SECONDS(millis()-startTime);
		char quedan[25];
		memset (quedan,0,sizeof(quedan));
		sprintf(quedan,"Quedan: %lu",MAX_SECONDS_ELAPSED-seconds);
		LogInfo(quedan);



		//Si ha pasado mas  tiempo del fijado volvemos a realizar la llamada
		if (startTime==0 ||  PARSE_TO_SECONDS(millis()-startTime)>MAX_SECONDS_ELAPSED)
		{


				LogInfo("Registrado!!!");
				//Encendemos los leds de trabajando
				setLed(LED_WORKING);
				//Encendemos modulo GSM
				SwitchModule();

				//Esperamos 30 segundos para que de tiempo de reconocer sim etc.. etc..
				delay(25000);

				bool hashError=true;
				uint8_t intentos=0;

				while ( hashError &&  intentos<20)
				{
								//Chequeamos si esta arrancado
								if (EstaArrancado())
								{
									//Si la sim esta ok
									if (CheckSIM())
									{
										//Chequeamos si esta registrado en la red movil
										if (EstaRegistrado())
										{

											LogInfo("Registrado!!!");
											//Encendemos los leds de trabajando
											setLed(LED_WORKING);
											hashError=false;

											//Si esta recibiendo llamada , sacamos melodia por altavoz
											/*while (TieneLlamadas())
											{
												LogInfo("Llamada entrante");
												PlayMelody();
											}*/




										}
										else
										{
											hashError=true;
											LogInfo("No Registrado en la red movil");
											setLed(LED_ERROR_NETWORK);
											delay(5000);
										}
									}
									else
									{
										hashError=true;
										//Registrando en la red
										LogInfo("La Sim no esta lista");

										setLed(LED_ERROR_SIM);
										delay(3000);
									}
							}
							else
							{
									hashError=true;
									LogError("Modulo no responde - Reiniciando modulo");
									//Encendemos los led error en modo tranquilo
									setLed(LED_ERROR_MODULE);
									delay(1000);
							}


							//Nos aseguramos que no es tema del eco
							if (hashError)
								 ActivaModulo();
					}


			//Llamamos al primer numero de la sim
			//En caso que tenga exito la llamada fijamos el startTime
			if (!hashError && LlamarDesdeSim(1))
				startTime=millis();


			//Apagamos modulo GSM
			ApagaModulo();

		}



	}
	else
	{
		//No esta mojado reseteamos contador de llamada
		startTime=0;
		LogInfo("no mojado");
	}


	//Encendemos leds
	float batteryVolt=GetCargaBateria();
	setLedBattery(batteryVolt);
	setLed(currentled);
	delay(1000);

	//Apagamos leds

	setLed(0);
	setLedBatteryOff();

}

int main() {

	//Necesario para la inicializacion de arduino
	init();

	//Setup
	setup();


	//Loop
	while (true)
		loop();

}



/************************************************************************/
/*Function: Funcion para sacar la melodia 								*/
/*Parameter:-void           											*/
/*Return:	-void 														*/
//void PlayMelody()
//{
// 	  // iterate over the notes of the melody:
//	  for (int thisNote = 0; thisNote < 8; thisNote++) {
//
//	    // to calculate the note duration, take one second
//
//	    // divided by the note type.
//	    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//	    int noteDuration = 1000 / noteDurations[thisNote];
//	    tone(SPEAKER_PIN, melody[thisNote], noteDuration);
//
//	    // to distinguish the notes, set a minimum time between them.
//	    // the note's duration + 30% seems to work well:
//	    int pauseBetweenNotes = noteDuration * 1.30;
//	    delay(pauseBetweenNotes);
//	    // stop the tone playing:
//	    noTone(SPEAKER_PIN);
//	  }
//}

/************************************************************************/
/*Function: Determine whether the sensor is exposed to the water		*/
/*Parameter:-void           											*/
/*Return:	-boolean,if it is exposed to the water,it will return true. */
boolean isExposedToWater()
{
	  char exposed[50];
	uint8_t counter=0;
	int result=0;
	digitalWrite(VCC_WATER_SENSOR, HIGH);

	delay(500);

	while (counter<10 && ((result=analogRead(READ_WATER_SENSOR))>10))
	{
	  memset(exposed,0,sizeof(exposed));
	  sprintf(exposed,"Sensor 1  %i",result);
	  LogInfo(exposed);

		delay(100);
		counter++;
	}

	if (counter<10)
	{
		counter=0;
		while (counter<10 && ((result=analogRead(READ_WATER_SENSOR2))>10))
		{
		  memset(exposed,0,sizeof(exposed));
		  sprintf(exposed,"Sensor 1  %i",result);
		  LogInfo(exposed);

			delay(100);
			counter++;
		}
	}

	delay(200);
	digitalWrite(VCC_WATER_SENSOR, LOW);

	return (counter==10);
}



void ParseFloat(float f,int decimals,char * result)
{
	memset(result,0, sizeof(result));
	int d1 = f;            // Parte entera (678).
	float f2 = f - d1;     // Parte decimal (678.0123 - 678 = 0.0123).


	int d2 = trunc(f2*pow(10, 2));   // Entero *10^decimals;
	sprintf (result,"%i.%i", d1, d2); //retornamos la parte entera
}


/* Read voltage divider
 * Reads the voltage divider to calculate a battery voltage
 * This software has no warranty, real or implied and is free to distribute and modify
 */
float GetCargaBateria() {
	  char battery[50];
	int val = 0;

  digitalWrite(VCC_DIVIDER_PIN, HIGH);
  delay(100); //Estabilizar..

  val =analogRead(A0);    // read the voltage on the divider

  float pinVoltage = val * VOLTAGE_VALUE_AD;


  memset(battery,0,sizeof(battery));
  sprintf(battery,"Leido %i",val);
  LogInfo(battery);



  float batteryVoltage = pinVoltage * BATTERY_RATIO;    //  Use the ratio calculated for the voltage divider
                                          //  to calculate the battery voltage

  char number[10];
  memset(battery,0,sizeof(battery));
  ParseFloat(batteryVoltage,2,number);

  sprintf(battery,"Voltage: %s Volt",number);
  LogInfo(battery);


  digitalWrite(VCC_DIVIDER_PIN, LOW);




  return batteryVoltage;

}

