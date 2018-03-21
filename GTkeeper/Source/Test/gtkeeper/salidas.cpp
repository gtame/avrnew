/*
 * salidas.cpp
 *
 * Created: 20/03/2018 22:15:45
 *  Author: Gabi
 */ 
 #include "../main.h"

 test (sali_ShowInfoSalidas)
 {
	for(uint8_t i=1;i<=PORTS_NUM;i++)
		Riego.RegistrarSalida(i,i, actSector);

	Riego.ShowInfoSalidas();
 }

 test (sali_GetPosicion)
 {
	int8_t posicion=Riego.RegistrarSalida(1,1, actSector);
	assertTrue(posicion==0);
	
	posicion= Riego.GetPosicion(1,actSector);
	assertTrue(posicion==0);
	
	//No esta registrada =-1
	posicion= Riego.GetPosicion(1,actAbono);
	assertTrue(Riego.GetSalidasActivas()==1); 
	assertTrue(posicion==-1);

	//Registramos salida de abono, habra 2 salidas
	posicion=Riego.RegistrarSalida(1,1,actAbono);
	assertTrue(Riego.GetSalidasActivas()==2); 
	

 }

 test (sali_SalidaRegistrada)
 {

	//Si existe la borramos
 	if (Riego.SalidaRegistrada(1,actSector))
 	Riego.EliminarSalida(1,actSector);


	uint8_t numsalida=Riego.GetSalidasActivas();
	int8_t posicion=-1;

	//REgistramos salida
	posicion=Riego.RegistrarSalida(1,1, actSector);
	assertTrue( (numsalida+1) == (Riego.GetSalidasActivas()));
	assertTrue(Riego.salidas[posicion].Ident==1 && Riego.salidas[posicion].Tipo==actSector);
	

 }

 test (sali_RiegosActivosEnSector)
 {

 	uint8_t numsalidas=Riego.GetSalidasActivas();
	

	//El for no vale ya que cada vez que eliminamos o añadimos cambia el indice 
	while(Riego.GetSalidasActivas()>0)
	{
 
		Riego.EliminarSalida(Riego.salidas[0].Ident, Riego.salidas[0].Tipo);
	}
	
	assertTrue(Riego.GetSalidasActivas()==0);

  	int8_t posicion=Riego.RegistrarSalida(1,1, actSector);
  	posicion= Riego.GetPosicion(1,actSector);
  	assertTrue(Riego.GetSalidasActivas()==1);
  	assertTrue(posicion==0);

	uint8_t numriegos= Riego.RiegosActivosEnSector(1);
	assertTrue(numriegos==1);
 }

 test (sali_EliminarSalida)
 {

 	int8_t posicion=Riego.RegistrarSalida(1,1, actSector);
	posicion= Riego.GetPosicion(1,actSector);
	assertTrue(Riego.GetSalidasActivas()==1);
	assertTrue(posicion==0);


	Riego.EliminarSalida(1,actSector);
	posicion= Riego.GetPosicion(1,actSector);
	assertTrue(Riego.GetSalidasActivas()==0);
	assertTrue(posicion==-1);


 }

 test (sali_GetSalidasActivas)
 {
	for (uint i=1;i<=PORTS_NUM;i++)
	{
		 Riego.RegistrarSalida(i,i, actSector);
		 Riego.RegistrarSalida(i,i, actPrograma);
		 (Riego.GetSalidasActivas()==i*2);
	}

	
	//El for no vale ya que cada vez que eliminamos o añadimos cambia el indice
	while(Riego.GetSalidasActivas()>0)
	{
		
		Riego.EliminarSalida(Riego.salidas[0].Ident, Riego.salidas[0].Tipo);
	}
	
	assertTrue(Riego.GetSalidasActivas()==0);
 }

 test (sali_SalidaToString)
 {
 	for(uint8_t i=1;i<=PORTS_NUM;i++)
	{
	 	Riego.RegistrarSalida(i,i, actSector);
		memset(buffer_test,0,MAIN_BUFFER_SIZE);
 		Riego.SalidaToString(i,buffer_test);
		LOG_DEBUG_ARGS("SALIDA->%s",buffer_test);
	}
 }
 

 test (sali_RegistrarSalidaEnWeb)
 {
	//TO-DO...
	skip();
 }