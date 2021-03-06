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
		LOG_DEBUG("CheckCall");
		//Entra en user mode cuando el user pulsa una tecla
		//1-Recibi una interrupcion  GSM
		if (int_input_gsm)
		{

			//Hay LLamada �?
			return(gsm->EstaArrancado() && gsm->TieneLlamadas());
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
	 if (gsm->DescolgarLlamada())
	 {
		 MILLISECONDS_DELAY(500);
		 //Emitimos dos pitidos
		 gsm->GenerarPulsoDTMF('F',2);
		 MILLISECONDS_DELAY(100);
		 gsm->GenerarPulsoDTMF('F',2);
		 MILLISECONDS_DELAY(100);

		 //Pasamos a modo DTMF
		 if (gsm->PasarDTMFMode())
		 {
			 //Marca de tiempo
			 time_t time=now();
			 bool bexit=false;

			 while (ELAPSED_SECONDS(time)<MAX_SECONDS_DTMF_TONE && !bexit )
			 {
				 if (gsm->ReadSerialLine()==RX_OK_READ)
				 {
					char *response=gsm->GetLastResponse();
					LOG_DEBUG_ARGS("LEIDA %s",response);
					 if (strncmp_P(response,PSTR("+DTMF:"),6)==0)
					 {
						//En el 6 viene la tecla
						LOG_DEBUG_ARGS("Tecla %c",response[6]);
						//GenerarPulsoDTMF(response[6],20);
						if (response[6]=='*')
						{
							if (ExecuteCommand(buffer))
							{

							LOG_DEBUG("OK!!!");
							/*
								//indicamos al usuario que todo OK
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);
								GenerarPulsoDTMF('F',20);
								MILLISECONDS_DELAY(100);*/
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
	if (gsm->TieneLlamadas())
		gsm->SendCommandCheck(F("ATH"),(__FlashStringHelper*) gsm->AT_OK,(__FlashStringHelper*) gsm->AT_ERROR);

	 int_input_gsm=false;

}

//SALE
void GTKeeper::OnLeaveCall()
{ 
	//REseteamos todo, config, programas y estadisticas.
	LOG_DEBUG("OnLeaveCall");
	
}