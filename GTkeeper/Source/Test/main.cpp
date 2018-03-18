 
#include "main.h"
 
GTKeeper gtKeeper;  
definedtest tests[25];
uint8_t contador;
char buffercommand[25];
char buffer_test[MAIN_BUFFER_SIZE];

FakeStream myStream;



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


	Test::out = &Serial;
	//Test::exclude("*");
	//Test::include("dayToDiasSemana");
	//Test::include("elapsedSecsThisWeek2");
	//Test::include("dayOfWeek2*");
	//Test::include("getnextEjecucion*");
	
}

void loop()
{ 
	SDCheck();
	Test::run();
 
}
 


 