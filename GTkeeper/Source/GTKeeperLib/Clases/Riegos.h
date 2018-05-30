/* 
* Riegos.h
*
* Created: 21/03/2018 15:06:31
* Author: gtame
*/

#include <Logger.h>
#include <Time.h>
#include "../types.h"
#include "../settings.h"
#include "../pinout.h"
#include "../Utils/util.h"

#include "Salida.h"
#include "Estadistica.h"
#include "Programa.h"
#include "Configuracion.h"

#ifndef __RIEGOS_H__
#define __RIEGOS_H__




//Defines para operar con puertos
#define APAGA_RELE(x)  digitalWrite(x,LOW)// LOG_INFO_ARGS_B("RELE OFF %i",x);
#define ENCIENDE_RELE(x)  digitalWrite(x,HIGH)// LOG_INFO_ARGS_B("RELE ON %i",x);



class Riegos:public Salida,public Estadistica,public Programa
{



//Definimos eventos para facilitar pruebas y debuging;
typedef void(*LanzaCallbackAction)(tSalida *salida,tPrograma * programa);
typedef void(*ParaCallbackAction)(tSalida *salida, tPrograma * programa);

//variables
public:
protected:
private:
	char * internalbuffer;
	uint8_t sizebuffer;
	time_t last_RiegosCheck; //Ultima vez que se realizo el control 
	time_t nextaction;//Proxima accion, calculado desde el metodo CalculateNextAction
	tConfiguracion* config;
	LanzaCallbackAction lanzaCallback;
	ParaCallbackAction paraCallback;
	
//functions
public:
	Riegos(tConfiguracion *configuracion ,char * ibuffer,uint8_t isizebuffer);
	
	inline void LanzaCallBack(	LanzaCallbackAction lanzacallback) { lanzaCallback= lanzacallback;}
	inline void ParaCallBack ( ParaCallbackAction  paracallback) { paraCallback=paracallback;}

	void Initializate();
	void UpdateWebSuccess();//Indica que los cambios fueron actualizados en Web


	void CheckRiegos();
	void ChequearRiegos(time_t tiempo);
		
	//Riegos Arranca-para...
	//Apaga todos riegos
	void ApagarRiegos();//Desconecta todos riegos
	

		
	//Valvula Latch

	//Sector
	bool EnciendeSector(uint8_t sector);
	bool ApagaSector (uint8_t sector);
	//Programa
	bool EnciendePrograma(uint8_t program);
	bool ApagaPrograma (uint8_t program);
    //Abono	
	bool EnciendeAbono(uint8_t unidAbono,time_t tiempo);
	bool ApagaAbono (uint8_t unidAbono);
	//Motor
	bool ApagaMotor();
	bool EnciendeMotor ();
	
	//Lanza Riego ¿?
	void LanzaRiego(uint8_t programaIndex,bool sendsms) ;
	void PararRiego(uint8_t programaIndex);
	time_t CalculateNextAction ();//Calcula la fecha de la proxima accion programada a ejecutar (Programas & Salidas) <-> (Parar - arrancar) devuelta por GetNextAction

	 inline time_t GetNextAction() { return nextaction;}//Devuelve la fecha de la proxima accion programada a ejecutar CalculateNextAction
	 inline bool GetChangedRiegos() { return GetChangedSalidas();} // Indica si se activo o desactivo algun riego ;)
protected:
	void AbrirValvulaLatch(uint8_t sector);
	void CerrarValvulaLatch(uint8_t sector);

 
	 void OnRegistrarSalida(uint8_t salida ,uint8_t sector, TipoSalida tipo,time_t tiempo);
	 void OnEliminarSalida(uint8_t salida , TipoSalida tipo);

private:

}; //Riegos


//Constantes
extern const uint8_t ports[PORTS_NUM] ;
extern  const uint8_t ports_abono[PORTS_ABONO];



#endif //__RIEGOS_H__
