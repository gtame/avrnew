/* 
* Salida.cpp
*
* Created: 18/03/2018 17:30:28
* Author: Gabi
*/


#include "Salida.h"
#include "Utils\util.h"

//Negativo A>B
//Cero =
//Positivo B>A
int  sortmethod(const void* a, const void* b)
{
	tSalida *salidaA = (tSalida *)a;
	tSalida *salidaB = (tSalida *)b;

	//Lo que mas pesara sera el tipo :>
	int8_t result= (salidaA->Tipo-salidaB->Tipo);

	//Si son del mismo tipo, fecha desde
	if (result==0)
	result=(salidaA->Desde-salidaB->Desde);

	return result;
}





// default constructor
Salida::Salida( char * ibuffer,uint8_t isizebuffer)
{
	internalbuffer=ibuffer;
	sizebuffer=isizebuffer;
	salidas_activas=0;//Al arrancar no hay salidas activas ;)
	changed=false; 

} //Salida



void Salida::ShowInfoSalidas()
{

	memset(internalbuffer,0,sizebuffer);
	for(int8_t i=0;i<GetSalidasActivas();i++)
	{
		switch (salidas[i].Tipo)
		{
			case actPrograma:
			{
				sprintf_P(internalbuffer,PSTR("P%02i S%02i"),salidas[i].Ident,salidas[i].Sector);
			}
			break;
			case actSector:
			{
				strcpy_P(internalbuffer,PSTR("SECTOR"));
			}
			break;
			case actAbono:
			{
				strcpy_P(internalbuffer,PSTR("ABONO"));
			}
			break;
			case actMotor:
			{
				strcpy_P(internalbuffer,PSTR("MOTOR"));
			}
			break;
			case actLimpieza:
			{
				strcpy_P(internalbuffer,PSTR("LIMPIEZA"));
			}
			break;
			case actNone:
			{
				strcpy_P(internalbuffer,PSTR("NONE "));
			}
			break;


		}

		LOG_DEBUG_ARGS("%s %i %02d:%02d hasta %02d:%02d",internalbuffer,salidas[i].Ident, hour(salidas[i].Desde),minute(salidas[i].Desde),hour(salidas[i].Hasta),minute(salidas[i].Hasta));
	}
}

//Obtiene la posici�n en la que la salida ha sido registrada
int8_t Salida::GetPosicion(uint8_t ProgSectorIndex , TipoSalida tipo)
{
	for(int8_t i=0;i<GetSalidasActivas();i++)
	{
		if (salidas[i].Tipo== tipo && salidas[i].Ident==ProgSectorIndex )
		{
			return i;
		}
	}
	return -1;
}

//Indica si esa salida esta registrada
bool Salida::SalidaRegistrada(uint8_t ProgSectorIndex , TipoSalida tipo)
{
	return (GetPosicion(ProgSectorIndex,tipo)!=-1);
}

uint8_t Salida::RiegosActivosEnSector(uint8_t sector)
{
	int num_riegos=0;
	for(int8_t i=0;i<GetSalidasActivas();i++)
	{
		if ((salidas[i].Tipo== actPrograma || salidas[i].Tipo==actSector) && salidas[i].Sector==sector )
		num_riegos++;

	}

	return num_riegos;
}

//Registra la salida
int8_t Salida::RegistrarSalida(uint8_t ProgSectorIndex,uint8_t sector , TipoSalida tipo, time_t tiempo)
{
	int8_t pos=GetPosicion(ProgSectorIndex,tipo);
	if (pos==-1)
	{

		uint8_t i=GetSalidasActivas();

		LOG_DEBUG_ARGS_B("SAl %i No registrada, registrar en %i, ",ProgSectorIndex,i);
		//Pal saco - Lo asocio al 1� que esta libre
		salidas[i].Tipo= tipo;
		salidas[i].Ident= ProgSectorIndex;
		salidas[i].Desde=now();//Registra la hora desde la que esta activa!!


		salidas[i].Sector=0;

		if (tipo==actSector)
			salidas[i].Sector=ProgSectorIndex;
		else if (tipo==actPrograma)
		{	
			if (sector==0)
				LOG_DEBUG_ARGS_B("Sector no puede ser =0 %i %i",ProgSectorIndex,i);

			salidas[i].Sector=sector;
	  	    salidas[i].Hasta= TIME_WITHOUT_SECONDS(now()) + (tiempo  * SECS_PER_MIN);
		}
		else if (tipo==actAbono)
			salidas[i].Hasta= TIME_WITHOUT_SECONDS(now()) + (tiempo * SECS_PER_MIN);





		salidas_activas++;
		qsort (salidas, salidas_activas, sizeof(tSalida),sortmethod);

		pos =GetPosicion(ProgSectorIndex,tipo);

		SetChangedSalidas(true);
	}
	else 
	{
	 
		//Actualizamos la fecha hasta si procede
		if (salidas[pos].Hasta!=0 && tiempo>0  && salidas[pos].Hasta<(now() +tiempo))
			salidas[pos].Hasta= TIME_WITHOUT_SECONDS(now()) + (tiempo * SECS_PER_MIN);
	 
	 }


	return pos;
}


//Elimina la salida
void Salida::EliminarSalida(uint8_t salida , TipoSalida tipo)
{
	
	int8_t pos=GetPosicion(salida,tipo);
	if (pos!=-1)
	{
		salidas[pos].Hasta=now();


		//if (salidas[pos].Tipo==)
		//if (RiegosActivosEnSector(salidas[pos].Sector))
		//salidas[pos].Desde=now

		//Registrar estadistica
		//salidas[pos].Hasta=0;
		LOG_DEBUG_ARGS("ORDENANDO %i",salidas_activas);
		salidas[pos].Tipo=actNone;
		qsort (salidas, salidas_activas, sizeof(tSalida),sortmethod);
		salidas_activas--;//Importante para que reordene correctamente hacer la resta despues del qsort

		SetChangedSalidas(true);
	}
}



void Salida::SalidaToString(uint8_t salidaIndex, char *text) {

	tSalida* stat=&salidas[salidaIndex];
 	sprintf_P(text,
	PSTR("%02i%02i%08lu%08lu%02i"),
	stat->Tipo,
	stat->Ident,
	stat->Desde,
	stat->Hasta,
	stat->Sector);
}



/*
bool Salida::PostHttpParametersCallback()
{

	//Set the Content as multipart/form-data type of HTTP POST and also set the boundary value
	//AT+HTTPPARA="CONTENT","multipart/form-data; boundary=----WebKitFormBoundaryvZ0ZHShNAcBABWFy"
	//OK
	//====================================
	//AT+HTTPDATA=192,10000
	//OK
	//DOWNLOAD
	//Una vez recibimos el download , enviamos toda peticion el 192, es de todo Cabecera + Contenido .
	//------WebKitFormBoundaryvZ0ZHShNAcBABWFy
	//Content-Disposition: form-data; name="fileToUpload"; filename="data.txt"
	//Content-Type: text/plain
	//Hello Ravi
	//------WebKitFormBoundaryvZ0ZHShNAcBABWFy
	//OK

	//http://www.raviyp.com/embedded/194

	const char  boundary[] = "---------------------------8278697928671";


	if (Salida.SendCommandCheck( F("AT+HTTPPARA=\"CONTENT\",\"multipart/form-data; boundary=%s\""),(const  __FlashStringHelper*) ATSerial::AT_OK,boundary)==RX_CHECK_OK)
	{

		//Salida.salidas_web-GET_ADDRES_SALIDAS_WEB
		//	if (SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"),195)==RX_CHECK_OK)



		int16_t totalLen=191 + (Salida.salidas_web-EEADDR_SW); //191 es el constante, boundarys etc etc

		LOG_DEBUG_ARGS_B("totalLen %i",totalLen);

		if (Salida.SendCommandCheck( F("AT+HTTPDATA=%i,10000"),F("DOWNLOAD"), totalLen)==RX_CHECK_OK)
		{

			delay(500);

			uint8_t t=0;
			//t+=SendRawData("Accept-Encoding: deflate");
			//Vomitamos lo que hayamos registrado
			//Boundary
			t=Salida.SendRawData("--");
			t+=Salida.SendRawData(boundary);
			t+=Salida.SendRawData("\r\n");

			//Cabecera
			t+=Salida.SendRawData("Content-Disposition: form-data; name=\"submitted\"; filename=\"abcd.txt\"\r\n");
			t+=Salida.SendRawData("Content-Type: text/plain\r\n\r\n");

			//Ahora enviamos todo lo que haya en estadisticas
			//t+=SendRawData("abcd\r\n");

			uint16_t ee_addr=EEADDR_SW;

			while ( ee_addr != Salida.salidas_web) {
				//ch = EEPROM.read(ee_addr);
				memset (Salida.buff_parse,0,MAIN_BUFFER_PARSE);

				if (MAIN_BUFFER_PARSE<(Salida.salidas_web-ee_addr))
				{
					eeprom_read_block((void *)Salida.buff_parse,(void *)ee_addr,MAIN_BUFFER_PARSE);
					ee_addr+=MAIN_BUFFER_PARSE;
				}
				else
				{
					eeprom_read_block((void *)Salida.buff_parse,(void *)ee_addr,(Salida.salidas_web-ee_addr));
					ee_addr+=(Salida.salidas_web-ee_addr);
				}


				t+=Salida.SendRawData(Salida.
				);
			}


			//Boundary
			t+=Salida.SendRawData("\r\n--");
			t+=Salida.SendRawData(boundary);
			t+=Salida.SendRawData("--\r\n");

			LOG_DEBUG_ARGS_B("Enviado x post--> %i",t);
			delay(500);
			return true;
		}
		else
		return false;
	}
	else
	return false;
}

void Salida::PostHttpResultCallback(const char* url,int length)
{

	uint16_t contador=0;
	uint8_t readResult=0;
	uint8_t result =-1;

	//Solo esperamos 2 posibles valores OK � ERR
	if (length==2)
	{
		//Leemos parcialmente para no saturar el buffer de Recepcion del puerto serie
		if (Salida.SendCommandCheck( F("AT+HTTPREAD=%i,%i"),F("+HTTPREAD:"),contador,length)==RX_CHECK_OK)
		{
			//19 Es por que el programa es de longuitud 17 +2 (CR+LF)
			//+HTTPREAD: 19
			//char *buffer=GetLastResponse();
			//buffer=buffer+10;
			//int serialLen= atoi(buffer);<--Serial len

			Salida.WaitResponse(500);
			//Ahora llamamos a la funcion que gestionara los datos recibidos de la peticion GET
			readResult= Salida.ReadSerialLine();
			char *line=Salida.GetLastResponse();

			LOG_INFO_ARGS("Web=>%s",line);

			if (readResult==RX_OK_READ && strcmp_P(line,PSTR("OK"))==0 ) //Mientras no haya errores de procesamiento
			{
				result =LOAD_WEB_OK;
				//Si es ok.. inicializamos contador .. :)
				Salida.salidas_web=EEADDR_SW;
			}



		}
	}

}
*/
bool Salida::RegistrarSalidaEnWeb()
{


return true;
/*
	bool result=true;
	//Comprobamos si hay algo pdte de enviar a la web

	if (salidas_web!=0)
	{

		if (salidas_web!=EEADDR_SW)
		{

			//A enviar ..
			memset(buffer,0,MAIN_BUFFER_SIZE);
			sprintf(buffer,URL_SEND_SALIDAS,config.Imei);
			if (!URLRequest(buffer,false,PostHttpParametersCallback,PostHttpResultCallback))
			result=false;
			//Reiniciamos modulo GSM
		}
	}

	return result;*/
}

