/* 
* Estadistica.h
*
* Created: 20/03/2018 19:04:39
* Author: Gabi
*/


#include <stdint.h>
#include <Time.h>
#include <Logger.h>
#include "../types.h"
#include "../settings.h"



#ifndef __ESTADISTICA_H__
#define __ESTADISTICA_H__

class Estadistica
{
//variables
public:
     //struct para almacenar estadisticas
     tEstadistica estadisticas[MAX_PORTS];
protected:
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;
	time_t lastsaved;//ultima vez que se guardaron estadisticas a eeprom

//functions
public:
	Estadistica(char * ibuffer,uint8_t isizebuffer);
	//Metodos individuales
	void EEPROMLeerEstadistica(uint8_t index,tEstadistica *stat);
	void EEPROMGuardarEstadistica(tEstadistica * stat);
	void ResetearEstadistica(uint8_t index);
	void RegistrarEstadisticas(uint8_t sector,bool abono);


	inline time_t GetLastEstadisticasSaved() {return lastsaved;}
	//Metodos para la coleccion
	void ResetearEstadisticas();
	void CargarEstadisticasEEPROM();
	void GuardarEstadisticasEEPROM();

protected:

private:
	 

}; //Estadistica

#endif //__ESTADISTICA_H__
