/*
 * SMS.cpp
 *
 * Created: 14/03/2018 8:43:40
 *  Author: gtame
 */ 
#include <gtkeeper.h>

//CHECK
bool GTKeeper::CheckSMS()
{
	//Entra en user mode cuando el user pulsa una tecla
	return (keypad.getKey()!=NO_KEY);
}

//ACCION
void GTKeeper::OnSMS()
{
	//Marca de tiempo
	time_t time=now();
	while (ELAPSED_SECONDS(time)>100 && screenManager.IsActive())
	{
		screenManager.Loop();
	}
}

//SALE
void GTKeeper::OnLeaveSMS()
{
	//Apagamos
	screenManager.Apagar();
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}