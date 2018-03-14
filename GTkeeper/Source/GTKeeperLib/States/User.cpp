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
	//Marca de tiempo
	time_t time=now();
	while (ELAPSED_SECONDS(time)>100 && screenManager.IsActive())
	{
		screenManager.Loop();
		
		//Chequeamos programacion
		
	}
 }

 //SALE
 void GTKeeper::OnLeaveUser()
 {
	 //Apagamos
	  screenManager.Apagar();
	 //REseteamos todo, config, programas y estadisticas.
	 LOG_DEBUG("Salimos de User");
	 
	 
	 
 
 }