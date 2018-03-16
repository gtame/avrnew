/*
 * SMS.cpp
 *
 * Created: 14/03/2018 8:43:40
 *  Author: gtame
 */ 
#include <gtkeeper.h>

//Mensaje AT asi
//+CMT: "+34653316799","1casa","18/03/16,00:45:25+04"
//#1111#1


//CHECK
bool GTKeeper::CheckSMS()
{
	bool result=false;

	LOG_DEBUG("CheckSMS");

	//1-Recibi una interrupcion  GSM
	if (int_input_gsm)
	{
		//Levantamos modulo GSM
		MILLISECONDS_DELAY(100);
		//Entra en user mode cuando el user pulsa una tecla
		//1-Recibi una interrupcion  GSM
		 while  (!result && ReadSerialLine(buffer)==RX_OK_READ  )
		{
			//char * lastresponse= GetLastResponse();
			LOG_DEBUG_ARGS("ENTRO SMS %s %i",buffer,strncmp_P(buffer,PSTR("+CMT:"),5));
			result = (strncmp_P(buffer,PSTR("+CMT:"),5)==0);
		}
		
		//Hay SMS ¿?
		
	}
	//Entra en user mode cuando el user pulsa una tecla
	return result;
}

//ACCION
void GTKeeper::OnSMS()
{
	LOG_DEBUG("OnSmS");
	//Esperamos a recibir la siguiente linea
	MILLISECONDS_DELAY(200);
	ReadSerialLine(buffer);
	//No se pq llega con RX_NO_DATA, debe ser pq no le llega el salto de linea
	//if (ReadSerialLine(buffer)==RX_OK_READ)
	if (buffer!=NULL && strlen(buffer)>0)
	{	
		LOG_DEBUG_ARGS("Mensaje %s",buffer);

		if (ExecuteCommand(buffer))
		{
			//Avisamos
			LOG_DEBUG("Procesado mensaje!!");
		}
	}

}

//SALE
void GTKeeper::OnLeaveSMS()
{
	LOG_DEBUG("OnLeaveSMS");
	
}