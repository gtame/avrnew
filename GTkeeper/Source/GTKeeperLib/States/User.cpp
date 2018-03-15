/*
 * User.cpp
 *
 * Created: 14/03/2018 8:07:39
 *  Author: gtame
 */ 
 #include <gtkeeper.h>

//interrupcion
void wakeUpInt ()
{
	
}

 //CHECK
 bool GTKeeper::CheckUser()
 {
	 //Entra en user mode cuando el user pulsa una tecla
	 return (keypad.getKey()!=NO_KEY);
 }

 //ACCION
 void GTKeeper::OnUser()
 {

	LOG_DEBUG("OnUser");

	if (!screenManager.IsActive())
		screenManager.Encender();

	screenManager.SetTimeInactivity(SCREEN_TIMEOUT);
	 
	//Marca de tiempo
	time_t time=now();
	while (screenManager.IsActive())
	{
		screenManager.Loop();
		//Chequeamos programacion
	}

	LOG_DEBUG("End OnUser");
 }

 //SALE
 void GTKeeper::OnLeaveUser()
 {
	LOG_DEBUG("Salimos de User Init");
	
	 //Apagamos
	  if (screenManager.IsActive())
		screenManager.Apagar();
    
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de User Fin");
	 
	 
	 
 
 }