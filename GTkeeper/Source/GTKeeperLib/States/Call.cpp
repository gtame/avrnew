/*
 * Call.cpp
 *
 * Created: 14/03/2018 8:45:56
 *  Author: gtame
 */ 
#include <gtkeeper.h>

//CHECK
bool GTKeeper::CheckCall()
{
		//Entra en user mode cuando el user pulsa una tecla
		//1-Recibi una interrupcion  GSM
		if (int_input_gsm)
		{
			//Hay LLamada ¿?
			return(EstaArrancado() && TieneLlamadas());
		}
		else
			//Entra en user mode cuando el user pulsa una tecla
			return false;
}

//ACCION
void GTKeeper::OnCall()
{
	 LOG_DEBUG("OnCall");
	
	 
	  memset(buffer,0,MAIN_BUFFER_SIZE);
	  uint8_t contador=0;

	 //Descolgamos la llamada
	 if (SendCommandCheckError(F("ATA"),(__FlashStringHelper*) AT_OK,F("NO CARRIER"))==RX_CHECK_OK)
	 {
		 MILLISECONDS_DELAY(500);
		 //Emitimos dos pitidos
		 GenerarPulsoDTMF('F',20);
		 MILLISECONDS_DELAY(100);
		 GenerarPulsoDTMF('F',20);
		 MILLISECONDS_DELAY(100);

		 //Pasamos a modo DTMF
		 if (SendCommandCheck(F("AT+DDET=1,1000,0,0"),(__FlashStringHelper*) AT_OK)==RX_CHECK_OK)
		 {
			 //Marca de tiempo
			 time_t time=now();
			 bool bexit=false;

			 while (ELAPSED_SECONDS(time)<MAX_SECONDS_DTMF_TONE && !bexit )
			 {
				 if (ReadSerialLine()==RX_OK_READ)
				 {
					char *response=GetLastResponse();
					LOG_DEBUG_ARGS("LEIDA %s",response);
					 if (strncmp_P(response,PSTR("+DTMF:"),6)==0)
					 {
						//En el 6 viene la tecla
						LOG_DEBUG_ARGS("Tecla %c",response[6]);
						GenerarPulsoDTMF(response[6],20);
						if (response[6]=='*')
						{
							if (ExecuteCommand(buffer))
							{
								//indicamos al usuario que todo OK
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);
							}

							bexit=true;
						}
						else if (MAIN_BUFFER_SIZE>contador)
						{
							buffer[contador]=response[6];
							contador++;
						}
						else
							bexit=true;
					
						 
						//Refrescamos marca de tiempo
						time=now();
					 }
					 else if (strcmp_P(response,PSTR("NO CARRIER"))==0)
					 {	
						//Si termina la llamada salimos... 
						bexit=true;
					 }
					
				 }
				 
			 }
		 }
	 

			
	 }
	 
	//COLGAMOS
	if (TieneLlamadas())
		SendCommandCheck(F("ATH"),(__FlashStringHelper*) AT_OK,(__FlashStringHelper*) AT_ERROR);

	 int_input_gsm=false;

}

//SALE
void GTKeeper::OnLeaveCall()
{ 
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("OnLeaveCall");
	
}