/* 
* Salida.h
*
* Created: 18/03/2018 17:30:28
* Author: Gabi
*/

#include <stdint.h>
#include <Time.h>
#include <Logger.h>
#include "../types.h"
#include "../settings.h"


#ifndef __SALIDA_H__
#define __SALIDA_H__



int  sortmethod(const void* a, const void* b);

class Salida
{
//variables
public:
		tSalida salidas [MAX_PROGRAMAS];//Array para guardar las salidas activas
protected:
		uint8_t salidas_activas;//Contador para almacenar las salidas activas
		//uint32_t salidas_web;//Almacen la posicion EEPROM donde esta actualmente el cursor de salidas.
		//tSalida pendingWeb[MAX_PROGRAMAS*2];//Array para guardar las salidas
private:
 	char * internalbuffer;
 	uint8_t sizebuffer;
	bool changed;//Flag si indica que cambio algun programa
//functions
public:
	Salida(char * internalbuffer,uint8_t sizebuffer);

	//Estos metodos manejan las salidas activas 
	void ShowInfoSalidas();
	int8_t GetPosicion(uint8_t ProgSectorIndex , TipoSalida tipo);
	bool SalidaRegistrada(uint8_t ProgSectorIndex , TipoSalida tipo);
	uint8_t RiegosActivosEnSector(uint8_t sector);

	int8_t RegistrarSalida(uint8_t ProgSectorIndex ,uint8_t sector, TipoSalida tipo,time_t tiempo);
	void EliminarSalida(uint8_t ProgSectorIndex , TipoSalida tipo);

	inline uint8_t GetSalidasActivas() { return salidas_activas;}
	void SalidaToString(uint8_t salidaIndex, char *text) ;
	bool RegistrarSalidaEnWeb();
	/*
	bool PostHttpParametersCallback();
	void PostHttpResultCallback(const char* url,int length);
	
	*/
	inline bool GetChangedSalidas() { return changed;}
protected:
	inline void SetChangedSalidas(bool value) { changed=value;}
private:
}; //Salida
 

#endif //__SALIDA_H__
