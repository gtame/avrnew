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
	bool result=false;

	//1-Recibi una interrupcion  GSM
	if (int_input_gsm)
	{
		//Levantamos modulo GSM
		WakeUp();
		//Hay SMS ¿?

	}
	//Entra en user mode cuando el user pulsa una tecla
	return result;
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