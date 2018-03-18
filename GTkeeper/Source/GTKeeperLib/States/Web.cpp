/*
 * Web.cpp
 *
 * Created: 14/03/2018 8:46:53
 *  Author: gtame
 */ 
#include <gtkeeper.h>


bool wwwping=false;
bool wwwexit=false;
void WebRequestCallBack (const char* x,int l)
{
	//Nos vendran los cambios
	wwwping=true;

}

void WebRequestParameters()
{
	//Aqui enviaremos todo lo pendiente
	//--Config
	//--Status S1;
	//--Programas
	//--Logs
	//gtKeeper.GetConfig4Web()
}

//CHECK
bool GTKeeper::CheckToWeb()
{
	//Entra en user mode cuando el user pulsa una tecla
	return false;
}



//ACCION
void GTKeeper::OnWeb()
{
	//MArca de tiempo
	time_t time=now();
	wwwexit=false;

	while (ELAPSED_SECONDS(time)<100 && !wwwexit)
	{
		 //Peticion de cambios
		// URLRequest("http:\\www.kk.es",WebRequestParameters,WebRequestCallBack);
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