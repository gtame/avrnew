/* 
* Programa.h
*
* Created: 18/03/2018 15:58:30
* Author: Gabi
*/
#include <stdint.h>
#include <Time.h>
#include <Logger.h>
#include <types.h>
#include "settings.h"


#ifndef __PROGRAMA_H__
#define __PROGRAMA_H__

#define dayOfWeek2(_time)  (dayOfWeek((_time))==1?7:dayOfWeek((_time))-1)//El Lunes es 1 y el domingo 7
#define dayToDiasSemana(_day_)   (DiasSemana)(pow(2,(_day_-1))+0.5)//Si pasas el dia lunes 1 -domingo7 te devuelve la enum DiasSemana
#define elapsedSecsThisWeek2(_time_)  (elapsedSecsToday((_time_)) +  ((dayOfWeek2((_time_))-1) * SECS_PER_DAY) )// note that week starts on day 1



//Sumatorio total 126 para todos los dias
typedef enum {
	NONE =0,
	L = (1 << 0),//1
	M= (1 << 1),//2
	X= (1 << 2),//4
	J= (1 << 3),//8
	V= (1 << 4),//16
	S= (1 << 5),//32
	D= (1 << 6)//64
} DiasSemana;


#define NUMERO_HORAS(SEG) (SEG/SECS_PER_HOUR)
#define NUMERO_MINUTOS(SEG) (SEG % SECS_PER_HOUR) / SECS_PER_MIN


class Programa
{
//variables
public:
	tPrograma programas[MAX_PROGRAMAS];
protected:
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;
//functions
public:
	Programa(char * internalbuffer,uint8_t sizebuffer);
	
	bool CargaProgramaDesdeString(uint8_t progIndex,char *programa);
	bool GrabarProgramaAEEPROM(uint8_t progIndex);
	bool CargarProgramaDesdeEEPROM(uint8_t progIndex);
	void ResetPrograma(uint8_t progIndex);
	void ProgramaToDisplay(uint8_t progIndex,char *text);
	void ProgramaToString(uint8_t progIndex,char *text);
	time_t GetNextEjecucion(uint8_t progIndex);
	
	
	//Metodos para toda la coleccion
	bool EEPROMCargaProgramas();
	void ResetProgramas();
	void ShowInfoProgramas();

protected:
private:

}; //Programa

#endif //__PROGRAMA_H__

