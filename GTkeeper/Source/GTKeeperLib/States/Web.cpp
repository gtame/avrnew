/*
 * Web.cpp
 *
 * Created: 14/03/2018 8:46:53
 *  Author: gtame
 */ 
#include <gtkeeper.h>

//CHECK
bool GTKeeper::CheckToWeb()
{
	//Entra en user mode cuando el user pulsa una tecla
	return (keypad.getKey()!=NO_KEY);
}

//ACCION
void GTKeeper::OnWeb()
{
	//Marca de tiempo
	time_t time=now();
	while (ELAPSED_SECONDS(time)>100 && screenManager.IsActive())
	{
		screenManager.Loop();
	}
}

//SALE
void GTKeeper::OnLeaveWeb()
{
	//Apagamos
	screenManager.Apagar();
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}