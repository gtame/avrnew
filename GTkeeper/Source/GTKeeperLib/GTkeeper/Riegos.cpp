/*
 * gtKeeperRiegos.cpp
 *
 * Created: 18/03/2018 18:33:33
 *  Author: Gabi
 */ 
 #include "../gtkeeper.h"
 
 ///////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////

 void GTKeeper::CheckRiegos()
 {
	 //Comprobamos si hay que lanzar-parar algun riego (Lo hacemos cuando cambia cada minuto)
	 time_t current_minute=now();
	 if (last_RiegosCheck==0 ||
	 (
	 minute(last_RiegosCheck) != minute(current_minute) ||
	 hour (last_RiegosCheck) != hour(current_minute)
	 )
	 )
	 {
		 current_minute=TIME_WITHOUT_SECONDS(current_minute);

		 //Si cambia el minuto registramos estadisticas
		// RegistrarEstadisticas();

		 //actualizamos la hora de arduino
		 //time_t hora_actual=GetTime(timeEl);
		 //setTime(hora_actual);
		 breakTime( current_minute, timeEl);
		 //LOG_INFO7
		 LOG_INFO_ARGS("Chk->%02i/%s/%02i %02i:%02i:%02i",timeEl.Day, monthStr(timeEl.Month),timeEl.Year, timeEl.Hour,timeEl.Minute,timeEl.Second);

		 LOG_INFO("Chequeando riegos");
		 if (last_RiegosCheck == 0) {

			 LOG_INFO("Check 0");
			 ChequearRiegos(current_minute);

			 } else {
			 while (last_RiegosCheck < current_minute) {
				 last_RiegosCheck += SECS_PER_MIN;

				 if (last_RiegosCheck > current_minute)
				 last_RiegosCheck = current_minute;



				 ChequearRiegos(last_RiegosCheck);
			 }
		 }


		 last_RiegosCheck = current_minute; // convert time elements into time_t


	 }

 }
 
 void GTKeeper::LanzaRiego(uint8_t contador,bool sendsms=false) {

	 //Si esta entre los sectores activos :)
	 if (programas[contador].Sector>0 && programas[contador].Sector<=PORTS_NUM )
	 {


		 //Log.Debug("Parada programada ->  %04i/%02i/%02i/ %02i:%02i", year(programas[contador].Hasta), month(programas[contador].Hasta), day(programas[contador].Hasta), hour(programas[contador].Hasta), minute(programas[contador].Hasta));

		 //ACtivamos salida ;>
		 EnciendePrograma(contador);

		 LOG_INFO_ARGS
		 (("Lanza P%02i S%02i R%02d:%02d A%02d:%02d")
		 ,contador+1
		 ,programas[contador].Sector
		 ,hour(programas[contador].TiempoRiego)
		 ,minute(programas[contador].TiempoRiego)
		 ,hour(programas[contador].TiempoAbono)
		 ,minute(programas[contador].TiempoAbono)
		 );

		 //Actualizamos la fecha Hasta
		 uint8_t pos=GetPosicion(contador,actPrograma);
		 if (pos!=-1)
		 salidas[pos].Hasta= TIME_WITHOUT_SECONDS(now()) + programas[contador].TiempoRiego;



		 //Miramos los tiempos de abonos :)
		 if (programas[contador].TiempoAbono>0)
		 {
			 EnciendeAbono(1);
			 uint8_t pos=GetPosicion(1,actAbono);
			 if (pos!=-1)
			 {
				 //Actualizamos la fecha de abono si es mas posterior
				 if (salidas[pos].Hasta<(now() + programas[contador].TiempoAbono))
				 salidas[pos].Hasta= TIME_WITHOUT_SECONDS(now()) + programas[contador].TiempoAbono;
			 }

		 }

		 #ifdef SMS
		 if (IsGSMEnable() &&  sendsms || (config.AvisosSMS & SMSInicioSector))
		 {

			 SmsMessage(PBB(F("Lanza S%i R%02d:%02d A%02d:%02d\n"),programas[contador].Sector
			 ,hour(programas[contador].TiempoRiego),minute(programas[contador].TiempoRiego)
			 ,hour(programas[contador].TiempoAbono),minute(programas[contador].TiempoAbono)));
		 }
		 #endif
		 //Log.Debug("Rega
		 //Log.Debug("Regando-> Programa:%i Sector:%i Tiempo:%02i:%02i",contador, programas[contador].Sector, hour(programas[contador].TiempoRiego), minute(programas[contador].TiempoRiego));
		 //Log.Debug("Hasta - %04i/%02i/%02i %02i:%02i", year(programas[contador].Hasta), month(programas[contador].Hasta), day(programas[contador].Hasta), hour(programas[contador].Hasta), minute(programas[contador].Hasta));

	 }
 }

 void GTKeeper::PararRiego(uint8_t contador) {

	 //Desactiva la salida
	 ApagaPrograma(contador);


	 //Log.Debug("Parando riego-> Programa:%i Sector:%i", contador, programas[contador].Sector);
	 //Miramos si hay algun programa que debemos lanzar. ;>
	 for (uint8_t i = 0; i < MAX_PROGRAMAS; i++)
	 {
		 if (i != contador)
		 {
			 //Chequeamos que no haya otro programa que este en ejecucion para el mismo sector y acabe mas tarde
			 if (programas[i].HoraInicio == 88)
			 {
				 uint8_t programa = programas[i].MinutoInicio;
				 //El contador empieza en 0
				 if ((programa - 1) == contador && programas[i].TiempoRiego>0)
				 {
					 LanzaRiego(i);
				 }
			 }
		 }
	 }





	 #ifdef SMS
	 if (IsGSMEnable() && (config.AvisosSMS & SMSFinSector))
	 {
		 SmsMessage(PBB(F("Paro P%i S%i\n"),contador+1, programas[contador].Sector));

	 }
	 #endif



 }

 void GTKeeper::ChequearRiegos(time_t tiempo) {

	 //Recorremos todos los programas miramos si debemos lanzar alguno
	 //O parar alguno que este en ejecucion...


	 uint8_t hora_actual = hour(tiempo);
	 uint8_t minuto_actual = minute(tiempo);

	 //SmsOpen(config.MovilAviso);

	 //Log.Info("Chequeando riegos de esta hora %02i:%02i", hora_actual, minuto_actual);

	 //Chequeamos si debemos parar algun riego ó abono
	 uint8_t contador = 0;
	 //Deberemos chequear con un while, ya que con un for no nos vale pq al parar algun programa reordena el listado
	 //y nos la lia
	 while(contador<GetSalidasActivas())
	 {
		 switch (salidas[contador].Tipo)
		 {
			 case actPrograma:
			 {
				 if (salidas[contador].Hasta <= now())
				 {
					 PararRiego(salidas[contador].Ident);
					 contador=0;
				 }
				 else
				 contador++;
			 }
			 break;
			 case actAbono:
			 {
				 if (salidas[contador].Hasta <= now())
				 {
					 LOG_DEBUG("Parando abono");
					 ApagaAbono(salidas[contador].Ident);
					 contador=0;
					 #ifdef SMS
					 if (IsGSMEnable() && (config.AvisosSMS & SMSFinSector))
					 {
						 SmsMessage_P(PSTR("Parada bomba abono\n"));

					 }
					 #endif
				 }
				 else
				 contador++;
			 }
			 break;
			 default:
			 contador++;
		 }
	 }



	 for (uint8_t contador = 0; contador < MAX_PROGRAMAS; contador++) {

		 //Si todavia no esta arrancado
		 if (!SalidaRegistrada(contador,actPrograma) )
		 {
			 //Log.Debug("Programa. %i - hora %i minuto %i", contador, programas[contador].HoraInicio,	programas[contador].MinutoInicio);

			 if (hora_actual == programas[contador].HoraInicio && minuto_actual ==programas[contador].MinutoInicio) {
				 bool lanzar = false;
				 //Log.Debug("Esta en hora de lanzar riego. %i", contador);

				 //Tenemos que ver para que dias de la semana esta configurado que salte. ;>
				 if ((programas[contador].Dias & L) && weekday(tiempo) == dowMonday)
				 lanzar = true;

				 if ((programas[contador].Dias & M) && weekday(tiempo) == dowTuesday)
				 lanzar = true;

				 if ((programas[contador].Dias & X) && weekday(tiempo) == dowWednesday)
				 lanzar = true;

				 if ((programas[contador].Dias & J) && weekday(tiempo) == dowThursday)
				 lanzar = true;

				 if ((programas[contador].Dias & V) && weekday(tiempo) == dowFriday)
				 lanzar = true;

				 if ((programas[contador].Dias & S) && weekday(tiempo) == dowSaturday)
				 lanzar = true;

				 if ((programas[contador].Dias & D) && weekday(tiempo) == dowSunday)
				 lanzar = true;

				 //Si la tenemos que lanzar
				 if (lanzar)
				 LanzaRiego(contador);
			 }

		 }

	 }


	 #ifdef SMS
	 //Enviara el SMS - o si no tiene ningun msj lo cancela
	 SmsSend();
	 #endif
 }

 //PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele
 //Con una polaridad abre la valvula, con la otra la cierra
 //De este modo la valula no tiene que estar alimentada continuamente.(ahorro)
 void GTKeeper::AbrirValvulaLatch(uint8_t sector)
 {
	 //El comun si no se activa esta a -(negativo)
	 //Activamos el puente para que le llege positivo a la valvula
	 digitalWrite(PORT_PUENTEH1_PIN,HIGH);
	 digitalWrite(PORT_PUENTEH2_PIN,LOW);
	 delay(500);

	 //Enviamos pulso
	 //Activamos el rele comun
	 ENCIENDE_RELE(PORT_COMUN_PIN);
	 delay(250);
	 //Activamos el rele de la valvula
	 ENCIENDE_RELE(ports[(sector-1)]);
	 delay(25);
	 
	 //Tiramos abajo el rele
	 APAGA_RELE(PORT_COMUN_PIN);
	 APAGA_RELE(ports[(sector-1)]);

	 digitalWrite(PORT_PUENTEH1_PIN,LOW);
	 digitalWrite(PORT_PUENTEH2_PIN,LOW);


 }

 //PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele

 //PAra las valvulas latch hay que dar un golpe(abrir y cerrar) el rele
 //Con una polaridad abre la valvula, con la otra la cierra
 //De este modo la valula no tiene que estar alimentada continuamente.(ahorro)
 void GTKeeper::CerrarValvulaLatch(uint8_t sector)
 {
	 ///El comun si no se activa esta a -(negativo)
	 //Activamos el puente para que le llege positivo a la valvula
	 digitalWrite(PORT_PUENTEH1_PIN,LOW);
	 digitalWrite(PORT_PUENTEH2_PIN,HIGH);
	 delay(500);

	 //Enviamos pulso
	 //Activamos el rele comun
	 ENCIENDE_RELE(PORT_COMUN_PIN);
	 delay(250);
	 //Activamos el rele de la valvula
	 ENCIENDE_RELE(ports[(sector-1)]);
	 delay(25);
	 
	 //Tiramos abajo el rele
	 APAGA_RELE(PORT_COMUN_PIN);
	 APAGA_RELE(ports[(sector-1)]);

	 digitalWrite(PORT_PUENTEH1_PIN,LOW);
	 digitalWrite(PORT_PUENTEH2_PIN,LOW);

 }

 bool GTKeeper::EnciendeSector(uint8_t sector)
 {

	 if (!SalidaRegistrada(sector,actSector))
	 {
		 //Lo ponemos en marcha si aun no lo estaba
		 if (RiegosActivosEnSector(sector)==0)
		 this->AbrirValvulaLatch(sector);

		 RegistrarSalida(sector,sector,actSector);
		 if (config.motor_diesel)
		 EnciendeMotor();

		 return true;
	 }
	 else
	 return false;

 }

 bool GTKeeper::ApagaSector (uint8_t sector)
 {

	 LOG_DEBUG_ARGS_B("Apagando sector %i",sector);

	 if (SalidaRegistrada(sector,actSector))
	 {
		 //Antes de apagar el rele, tendremos que comprobar de los programas activos no sean para ese sector.
		 if (RiegosActivosEnSector(sector)==1)
		 {
			 LOG_DEBUG_ARGS_B("Apaga sector %i",sector);
			 this->CerrarValvulaLatch(sector);
		 }

		 EliminarSalida(sector,actSector);
		 if (GetSalidasActivas()==1)
		 {
			 if (SalidaRegistrada(1,actMotor))
			 //Si solo queda la salida activa del motor se apaga
			 ApagaMotor();
		 }

		 return true;
	 }
	 else
	 return false;
 }

 bool GTKeeper::EnciendeAbono(uint8_t unidAbono)
 {
	 if (!SalidaRegistrada(unidAbono,actAbono))
	 {
		 ENCIENDE_RELE(ports_abono[unidAbono-1]);
		 RegistrarSalida(unidAbono,0,actAbono);
		 return true;
	 }
	 else
	 return false;
 }

 bool GTKeeper::ApagaAbono (uint8_t unidAbono)
 {
	 if (SalidaRegistrada(unidAbono,actAbono))
	 {
		 APAGA_RELE(ports_abono[unidAbono-1]);
		 EliminarSalida(unidAbono,actAbono);
		 stop_abono=0;
		 return true;
	 }
	 else
	 return false;
 }

 bool GTKeeper::EnciendePrograma(uint8_t program)
 {
	 if (!SalidaRegistrada(program,actPrograma))
	 {
		 //Lo ponemos en marcha si aun no lo estaba
		 if (RiegosActivosEnSector(programas[program].Sector)==0)
		 this->AbrirValvulaLatch(programas[program].Sector);

		 RegistrarSalida(program,programas[0].Sector,actPrograma);

		 if (config.motor_diesel)
		 {
			 EnciendeMotor();
		 }
		 return true;
	 }
	 else
	 return false;

 }

 bool GTKeeper::ApagaPrograma (uint8_t program)
 {
	 if (SalidaRegistrada(program,actPrograma))
	 {

		 if (RiegosActivosEnSector(programas[program].Sector)==1)
		 this->CerrarValvulaLatch(programas[program].Sector);

		 EliminarSalida(program,actPrograma);
		 if (GetSalidasActivas()==1)
		 {
			 if (SalidaRegistrada(1,actMotor))
			 //Si solo queda la salida activa del motor se apaga
			 ApagaMotor();
		 }
		 return true;
	 }
	 else
	 return false;
 }

 bool GTKeeper::ApagaMotor()
 {
	 if (SalidaRegistrada(1,actMotor))
	 {
		 APAGA_RELE(PORT_MOTOR_PIN);
		 EliminarSalida(1,actMotor);
		 return true;
	 }
	 else
	 return false;
 }

 bool GTKeeper::EnciendeMotor ()
 {
	 if (!SalidaRegistrada(1,actMotor))
	 {
		 ENCIENDE_RELE(PORT_MOTOR_PIN);
		 RegistrarSalida(1,0,actMotor);
		 return true;
	 }
	 else
	 return false;
 }

 void GTKeeper::EnciendeSectorSMS(uint8_t sector)
 {
	 //Apagamos todos sectores , excepto el que estamos arrancando
	 ApagarRiegos();

	 //encendemos el seleccionado
	 if (sector>0 && sector<=PORTS_NUM)
	 {
		 EnciendeSector(sector);


		 LOG_INFO_ARGS("Encendiendo Sector %i, puerto %i",sector,ports[sector-1]);

		 #ifdef SMS
		 if (config.GSMAvailable &&  (config.AvisosSMS & SMSInicioSector))
		 {

			 Sms(config.MovilAviso,PBB(F("Arrancado Sector %i desde SMS"),sector));
		 }
		 #endif
	 }


 }

 void GTKeeper::ApagarRiegos()
 {

	 //Si hacemos un for, como lo reordena, nos hace la pirula y se quedan activos
	 while (GetSalidasActivas()>0)
	 {
		 switch (salidas[0].Tipo)
		 {
			 case actPrograma:
			 {
				 ApagaPrograma(salidas[0].Ident);
			 }
			 break;
			 case actSector:
			 {
				 ApagaSector(salidas[0].Ident);
			 }
			 break;
			 case actAbono:
			 {
				 ApagaAbono(salidas[0].Ident);
			 }
			 break;
			 case actMotor:
			 {
				 ApagaMotor();
			 }
			 break;
			 case actLimpieza:
			 {
				 //---No implementado
			 }
			 break;
			 default:
			 {
				 LOG_DEBUG_B("Apagar!!--None");
			 }
			 break;
		 }

	 }

 }
