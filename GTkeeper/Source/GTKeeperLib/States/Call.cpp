/*
 * Call.cpp
 *
 * Created: 14/03/2018 8:45:56
 *  Author: gtame
 */ 
#include <gtkeeper.h>

//CHECK
bool GTKeeper::CheckCall()
{
	//Entra en user mode cuando el user pulsa una tecla
	return true;
}

//ACCION
void GTKeeper::OnCall()
{
	//Marca de tiempo
	time_t time=now();
	while (ELAPSED_SECONDS(time)>100 && screenManager.IsActive())
	{
		screenManager.Loop();
	}
}

//SALE
void GTKeeper::OnLeaveCall()
{
	//Apagamos
	screenManager.Apagar();
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}