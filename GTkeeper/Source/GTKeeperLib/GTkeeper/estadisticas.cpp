/*
 * gtKeeperEstadisticas.cpp
 *
 * Created: 18/03/2018 18:39:06
 *  Author: Gabi
 */ 
 #include "../gtkeeper.h"

 

 void GTKeeper::RegistrarEstadisticas()
 {
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


				 LOG_DEBUG_ARGS_B ("Registrando S%i A%i",Salidas.salidas[salida].Sector,bAbono1);

				 EEPROMGuardarEstadistica(&estadistica);



			 }
		 }
	 }

 }

 void GTKeeper::ResetearEstadisticas()
 {
	 for (uint8_t contador=1;contador<=PORTS_NUM;contador++)
	 {
		 estadistica.sector=contador;
		 estadistica.tiempo_abono=0;
		 estadistica.tiempo_riego=0;
		 EEPROMGuardarEstadistica(&estadistica);
	 }
 }

 void GTKeeper::EEPROMLeerEstadistica(uint8_t sector,Estadistica * stat)
 {
	 uint16_t ee_addr= GET_ADDRES_ESTADISTICA(sector);
	 eeprom_read_block((void *)stat,(void *)ee_addr,sizeof(Estadistica));

 }

 void GTKeeper::EEPROMGuardarEstadistica(Estadistica * stat)
 {

	 uint16_t ee_addr= GET_ADDRES_ESTADISTICA(stat->sector);
	 //Ahora lo que vamos a hacer es comparar con lo que hay guardado en la eeprom
	 while (!eeprom_is_ready());

	 eeprom_write_block((void *)stat,(void *)ee_addr,sizeof(Estadistica));

	 //Solo guardo cuando sean iguales
	 //	if (strcmp(buffer,buffer_reader)!=0)
	 //	{
	 //		LOG_DEBUG_ARGS("Guardando.. %s",buffer);
	 //		eeprom_write_block((void *)buffer,(void *)ee_addr,LEN_PROGRAMA_STRING);
	 //	}
 }

