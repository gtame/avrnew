#include <ArduinoUnit.h>

test(correct)
{
	int x=1;
	assertEqual(x,1);
}

test(incorrect)
{
	int x=1;
	assertNotEqual(x,1);
}

void setup()
{
	Serial.begin(9600);
	while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop()
{
	Test::run();
}



//Funcion necesaria para el compilador
extern "C" void __cxa_pure_virtual() {}


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
