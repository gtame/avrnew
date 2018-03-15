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
	return false;
}
//ACCION
void GTKeeper::OnWeb()
{
	 
}

//SALE
void GTKeeper::OnLeaveWeb()
{
	//Apagamos
	screenManager.Apagar();
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("Salimos de User");
	
}