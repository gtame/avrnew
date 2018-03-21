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

GTKeeper gtKeeper;

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
