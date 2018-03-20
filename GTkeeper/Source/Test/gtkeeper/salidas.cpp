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
		gtKeeper.RegistrarSalida(i,i, actSector);

	gtKeeper.ShowInfoSalidas();
 }

 test (sali_GetPosicion)
 {
	int8_t posicion=gtKeeper.RegistrarSalida(1,1, actSector);
	assertTrue(posicion==0);
	
	posicion= gtKeeper.GetPosicion(1,actSector);
	assertTrue(posicion==0);
	
	//No esta registrada =-1
	posicion= gtKeeper.GetPosicion(1,actAbono);
	assertTrue(gtKeeper.GetSalidasActivas()==1); 
	assertTrue(posicion==-1);

	//Registramos salida de abono, habra 2 salidas
	posicion=gtKeeper.RegistrarSalida(1,1,actAbono);
	assertTrue(gtKeeper.GetSalidasActivas()==2); 
	

 }

 test (sali_SalidaRegistrada)
 {

	//Si existe la borramos
 	if (gtKeeper.SalidaRegistrada(1,actSector))
 	gtKeeper.EliminarSalida(1,actSector);


	uint8_t numsalida=gtKeeper.GetSalidasActivas();
	int8_t posicion=-1;

	//REgistramos salida
	posicion=gtKeeper.RegistrarSalida(1,1, actSector);
	assertTrue( (numsalida+1) == (gtKeeper.GetSalidasActivas()));
	assertTrue(gtKeeper.salidas[posicion].Ident==1 && gtKeeper.salidas[posicion].Tipo==actSector);
	

 }

 test (sali_RiegosActivosEnSector)
 {

 	uint8_t numsalidas=gtKeeper.GetSalidasActivas();
	

	//El for no vale ya que cada vez que eliminamos o añadimos cambia el indice 
	while(gtKeeper.GetSalidasActivas()>0)
	{
 
		gtKeeper.EliminarSalida(gtKeeper.salidas[0].Ident, gtKeeper.salidas[0].Tipo);
	}
	
	assertTrue(gtKeeper.GetSalidasActivas()==0);

  	int8_t posicion=gtKeeper.RegistrarSalida(1,1, actSector);
  	posicion= gtKeeper.GetPosicion(1,actSector);
  	assertTrue(gtKeeper.GetSalidasActivas()==1);
  	assertTrue(posicion==0);

	uint8_t numriegos= gtKeeper.RiegosActivosEnSector(1);
	assertTrue(numriegos==1);
 }

 test (sali_EliminarSalida)
 {

 	int8_t posicion=gtKeeper.RegistrarSalida(1,1, actSector);
	posicion= gtKeeper.GetPosicion(1,actSector);
	assertTrue(gtKeeper.GetSalidasActivas()==1);
	assertTrue(posicion==0);


	gtKeeper.EliminarSalida(1,actSector);
	posicion= gtKeeper.GetPosicion(1,actSector);
	assertTrue(gtKeeper.GetSalidasActivas()==0);
	assertTrue(posicion==-1);


 }

 test (sali_GetSalidasActivas)
 {
	for (uint i=1;i<=PORTS_NUM;i++)
	{
		 gtKeeper.RegistrarSalida(i,i, actSector);
		 gtKeeper.RegistrarSalida(i,i, actPrograma);
		 (gtKeeper.GetSalidasActivas()==i*2);
	}

	
	//El for no vale ya que cada vez que eliminamos o añadimos cambia el indice
	while(gtKeeper.GetSalidasActivas()>0)
	{
		
		gtKeeper.EliminarSalida(gtKeeper.salidas[0].Ident, gtKeeper.salidas[0].Tipo);
	}
	
	assertTrue(gtKeeper.GetSalidasActivas()==0);
 }

 test (sali_SalidaToString)
 {
 	for(uint8_t i=1;i<=PORTS_NUM;i++)
	{
	 	gtKeeper.RegistrarSalida(i,i, actSector);
		memset(buffer_test,0,MAIN_BUFFER_SIZE);
 		gtKeeper.SalidaToString(i,buffer_test);
		LOG_DEBUG_ARGS("SALIDA->%s",buffer_test);
	}
 }
 

 test (sali_RegistrarSalidaEnWeb)
 {
	//TO-DO...
	skip();
 }