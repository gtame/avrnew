#include "main.h"

//OJO:Se probo poniendola como static dentro de la funcion pero daba problemas al asignarle un valor por segunda vez
//porque sino el linker da error!!!
//Using new, delete, and dynamic memory management generally is not recommended for small embedded
//devices because it can become a resource hog. If you must have it, create a .cpp file in the project
//source folder with this content:
/*
__extension__ typedef int __guard __attribute__((mode (__DI__)));git s
extern "C" void * operator new(size_t size){  return malloc(size);}
extern "C" void operator delete(void * ptr){  free(ptr);}
extern "C" void * operator new[](size_t size){    return malloc(size);}
extern "C" void operator delete[](void * ptr){	if (ptr)free(ptr);}
extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);} ////Añadimos esta funcion para admitir variables static dentro de funcion
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
extern "C" void __cxa_guard_abort (__guard *) {}
*/

 
 char bufferapp[MAIN_BUFFER_SIZE];
 
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

 const uint8_t ports[PORTS_NUM]= {PORT_SECTOR1_PIN };//,PORT_SECTOR2_PIN,PORT_SECTOR3_PIN};//{224,25,26,27,28,29,30,31,32,33,34,35,36,37,38 } ;
 const uint8_t ports_abono[PORTS_ABONO]= {PORT_ABONO1_PIN,PORT_ABONO2_PIN } ;
 
 //Interrupciones variables (Volatile)
 volatile bool int_input_user=false; //User interaccion
 volatile bool int_input_gsm=false;//GSM interaccion



 //Creamos objetos
 LogSD SDCard(bufferapp,MAIN_BUFFER_SIZE );
 Configuracion Config(bufferapp,MAIN_BUFFER_SIZE);
 Riegos Riego(&Config.config,bufferapp,MAIN_BUFFER_SIZE );
 GSM GSMModem (&Config.config,&Serial1,bufferapp,MAIN_BUFFER_SIZE );
 GTKeeper gtKeeper(&Config,&GSMModem, &Riego,&SDCard, bufferapp,MAIN_BUFFER_SIZE);
 Logger Log (&Serial);


void setup() {

	gtKeeper.Setup();
	
} 

void loop() //----( LOOP: RUNS CONSTANTLY )----
{

	gtKeeper.Update();
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
