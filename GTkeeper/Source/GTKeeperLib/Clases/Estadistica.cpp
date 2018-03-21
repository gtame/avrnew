/* 
* Estadistica.cpp
*
* Created: 20/03/2018 19:04:39
* Author: Gabi
*/


#include "Estadistica.h"

// default constructor
Estadistica::Estadistica(char * ibuffer,uint8_t isizebuffer)
{
	internalbuffer=ibuffer;
	sizebuffer=isizebuffer;


	for (uint8_t contador=1;contador<=MAX_PORTS;contador++)
		 estadisticas[contador-1].sector=contador ;
}


void Estadistica::ResetearEstadistica(uint8_t statIndex)
{
		 estadisticas[statIndex].tiempo_abono=0;
		 estadisticas[statIndex].tiempo_riego=0;
}

void Estadistica::RegistrarEstadisticas(uint8_t sector,bool abono)
 {
 /*
	 //Chequeamos si tiene abono
	 bool bAbono1= SalidaRegistrada(1,actAbono);



	 for (uint8_t salida=0;salida<GetSalidasActivas();salida++)
	 {
		 if (salidas[salida].Tipo==actSector || salidas[salida].Tipo==actPrograma)
		 {



			 //salidas[salida].sector
			 //Check en caso que ese sector este en ejecucion x mas de un programa
			 bool bImputado=false;
			 for (uint8_t i=0;i<salida;i++)
			 {
				 if (salidas[i].Sector==salidas[salida].Sector)
				 {
					 bImputado=true;
					 break;
				 }
			 }

			 //Si no esta imputado el tiempo para ese Sector... lo registramos
			 if (!bImputado)
			 {



				 EEPROMLeerEstadistica(salidas[salida].Sector,&estadistica);


				 //Registramos la estadistica para el sector
				 estadistica.sector=salidas[salida].Sector;
				 estadistica.tiempo_riego+=60;//60seg

				 //Si tiene abono
				 if (bAbono1)
				 estadistica.tiempo_abono+=60;//60seg


				 LOG_DEBUG_ARGS_B ("Registrando S%i A%i",salidas[salida].Sector,bAbono1);

				 EEPROMGuardarEstadistica(&estadistica);



			 }
		 }
	 }
	 */
 }

 	//eeprom_write_block((void*)&config, ( void*) GET_ADDRES_CONFIG, sizeof(tConfiguracion));
//
 //
	 ////Cargamos la configuracion
	 //eeprom_read_block((void*)&config, (const void*) GET_ADDRES_CONFIG, sizeof(tConfiguracion));

 

void Estadistica::EEPROMLeerEstadistica(uint8_t statIndex,tEstadistica *stat)
{
	eeprom_read_block((void *)stat,(const void *)GET_ADDRES_ESTADISTICA(statIndex),sizeof(tEstadistica));
 }

void Estadistica::EEPROMGuardarEstadistica(tEstadistica * stat)
{

	
	tEstadistica statEeprom;
	statEeprom.sector=0;
	statEeprom.tiempo_abono=0;
	statEeprom.tiempo_riego=0;
	EEPROMLeerEstadistica (stat->sector-1, &statEeprom);
	
	//La lectura de la eeprom es mucha mas rapida que la escritura, ademas el nº de escrituras en una eeprom es limitado
	//Por lo que comprobaremos antes de escribir que ha cambiado (memcmp)
	if (memcmp((void *)stat,(void *)&statEeprom,sizeof(tEstadistica)) !=0)
	{
		
		uint16_t address=(stat->sector-1);
		address= GET_ADDRES_ESTADISTICA(address);
		//Ahora lo que vamos a hacer es comparar con lo que hay guardado en la eeprom
		while (!eeprom_is_ready());
		eeprom_write_block((const void *)stat,(void *)address,sizeof(tEstadistica));

	}
 
}

void Estadistica::ResetearEstadisticas()
 {
	 for (uint8_t contador=1;contador<=MAX_PORTS;contador++)
	 {
		 estadisticas[contador-1].sector=contador;
		 ResetearEstadistica(contador-1);
		 EEPROMGuardarEstadistica(&estadisticas[contador-1]);
	 }
 }

void Estadistica::CargarEstadisticasEEPROM()
 {
	 for (uint8_t contador=1;contador<=MAX_PORTS;contador++)
	 { 
		 EEPROMLeerEstadistica(contador-1,&estadisticas[contador-1]);
	 }
 }
 
void Estadistica::GuardarEstadisticasEEPROM()
{
	for (uint8_t contador=1;contador<=MAX_PORTS;contador++)
	{
		EEPROMGuardarEstadistica(&estadisticas[contador-1]);
	}
}


