/*
 * maintest.cpp
 *
 * Created: 21/10/2017 22:16:46
 *  Author: Gabi
 */ 

#include "maintest.h"

 /*
 Para debug
 EL uart es el que comunica con el PC
 El SoftSerial comunica con el modulo GMS -Neoway M590
  */
 //Cuando corre en real haremos las pruebas de apagado y encendido

 #ifdef ONOFF
//Enciende modulo
test(onoff_enciende)
{
pass();

}

//Modo ahorro de energia
test(onoff_sleepmode)
{
tNeoway590.SendCommandCheckOK( F("AT+ENPWRSAVE=1"));
pass();
}

//Apaga modulo
test(onoff_apaga)
{
	pass();

}

//Attach interrupt
test(onoff_attachInterrupt)
{
	tNeoway590.ActivaModulo();
	pass();
	///tNeoway590.AttachRing(TestRing,PIN_INTERRUPT_RING);
}
 

 #endif
 
test(estaactivo)
{
	assertTrue(tNeoway590.EstaArrancado());
	
}

testing (estaregistrado)
{
	if (checkTestPass(estaactivo))
	{
		if (tNeoway590.EstaRegistrado())
		pass();
		else
		fail();
	}

}

test (tcp_config)
{
	if (!tNeoway590.IsTCPConfig())
	{
		
		tNeoway590.TCPConfig("orangeworld",NULL,NULL);
		delay(1000);
		assertTrue( tNeoway590.IsTCPConfig());
		delay(1000);
		pass();
	}
	else
		skip();
}
 
 //Check IP names
 test(ip_isIP)
 {

	assertTrue(tNeoway590.IsIP("192.168.1.1"));
	assertTrue(tNeoway590.IsIP("10.141.1.1"));
	assertFalse(tNeoway590.IsIP("1host.es"));
	assertFalse(tNeoway590.IsIP("www.google.es"));
 }


 //Check the DNS resolution names
testing(ip_GetDNS)
{
	if (checkTestDone(tcp_config) || checkTestSkip(tcp_config))
	{
		assertEqual(tNeoway590.GetIp("192.168.1.1"),"192.168.1.1");
		assertTrue(strcmp(tNeoway590.GetIp("www.google.es"),NULL)!=0);
		assertTrue(strcmp(tNeoway590.GetIp("www.goasdadasdasdogle.es"),NULL)==0);
		pass();
	}
}


#ifdef FTP
//Check the FTP operations
//Login
//->Get
//->Put
//->Delete
//Logout

 int ftpReceive(char * first ,int length)
 {
	 int result=strlen(first)+2;//Porque tiene un \r\n que no es incluido
	 LOG_DEBUG_ARGS("FIRST %s",first);
	 while (mySerial.available() && result<length)
	 {
		 Serial.print((char)mySerial.read());
		 result++;

	 }
	 LOG_DEBUG_ARGS("Leido %i vs %i",result,length);
	 return result;
 }

 void ftpPutData()
 {
	 char sentence[] = "tocate los webostocate los webos";
	 for (uint8_t i=0;i<20;i++)
	 {
		 tNeoway590.SendRawData(sentence[i]);

	 }
 }



testing (ftp)
{
	if (checkTestDone(tcp_config) || checkTestSkip(tcp_config))
	{

			//speedtest.tele2.net -> 90.130.70.73
		
		if (!tNeoway590.FtpIsConnected())
		tNeoway590.FtpLogin("188.85.185.34",21,"anonymous","anonymous");



		if (tNeoway590.FtpIsConnected())
		{
			if (!tNeoway590.FtpGet("/prueba.txt",ftpReceive))
				fail();
			


			if (!tNeoway590.FtpPut("/upload.txt",1,1,20,ftpPutData))
				fail();
			if (!tNeoway590.FtpDel("/upload.txt"))
				fail();

			if (tNeoway590.FtpLogout())
				pass();
			else
				fail();
		}
		else
			fail();

		
	}

}
 
#endif



#ifdef HTTP
void SendPostData()
{
	tNeoway590.SendRawData("Hola");

}


//http put request
testing(http_put)
{
	if (checkTestPass(tcp_config) || checkTestSkip(tcp_config))
	{
		////Peticion put 
		assertTrue(tNeoway590.HttpPutRequest("postman-echo.com","/put?var=value",4,SendPostData,NULL));
		pass();
	}
}

//http get request
testing (http_post)
{
	if (checkTestPass(tcp_config) || checkTestSkip(tcp_config))
	{
		////Peticion post , envia un fichero con el contenido que queramos
		//tNeoway590.HttpPostFileResquest("octavustriatlon.com","/custom/qdq.php",4,SendPostData,NULL);
		assertTrue(tNeoway590.HttpPostFileResquest("postman-echo.com","/post",4,SendPostData,NULL));
		pass();
	}
}

//http get request
testing (http_get)
{
	if (checkTestPass(tcp_config) || checkTestSkip(tcp_config))
	{
		//http://www.arduino.cc/asciilogo.txt
		assertTrue( tNeoway590.HttpGetRequest("postman-echo.com","/get?param1=value",NULL));
		pass();
	}
}


#endif


#ifdef PHONE

testing(sim_contacto)
{
	if (checkTestPass(estaactivo) && checkTestDone(estaregistrado))
	{
		if(tNeoway590.SIMEstaLista())
		{
			char phone[25] = {0};
			char name[25] = {0};

			tNeoway590.GetSIMContact(1,phone,name);
			assertEqual(phone,"653316799");
			pass();
		}
	}
	
	//tNeoway590.SendCommandCheckOK( F("AT+CMGL=4"));
}
 
 
testing(phone_llamada)
 {
	if (checkTestPass(estaregistrado))
	{
	 tNeoway590.SendCommandCheckOK(F("ATD653316799;"));
	 uint8_t result=tNeoway590.WaitResponseResult(F("CONNECT"),F("NO CARRIER"),15000);
	 if (result==RX_CHECK_OK)
	 {
		 //A esperar a que cuelgue..:)
		 delay(1000);
		 
	 }
	 else if (result==RX_ERROR)
	 {
		 if (tNeoway590.EstaRegistrado())
			 LOG_DEBUG_B("Ha colgado");
		 else
			 LOG_DEBUG_B("No se pudo realizar llamada!!");
	 }
	 else if (result == RX_NO_DATA)
	 {
		 LOG_DEBUG_B("No ha respondido!");

	 }
	 tNeoway590.SendCommandCheckOK(F("ATH"));
	 pass();
	 }
	 
 }


testing(phone_sms)
{
	if (checkTestPass(estaregistrado))
	{
		if (tNeoway590.SendSMS("653316799",(__FlashStringHelper *)("Hola gabi")))
			pass();
		else
			fail();
	}
}

 

testing(phone_info)
{
	 if (checkTestPass(estaactivo))
	 {

	 
	tNeoway590.SendCommandAsync(F("ATI"));
	tNeoway590.SIMEstaLista();
	tNeoway590.EstaRegistrado();
	 
	uint8_t cobertura=tNeoway590.GetCobertura();
	pass();
	}
}

#endif

 #ifdef WSOCKETS

testing(ws_check)
{


if (checkTestPass(tcp_config) || checkTestSkip(tcp_config))
{

	if (tNeoway590.IsTCPOpen())
	tNeoway590.TCPClose();
	
	//if (tNeoway590.clientConnect("demos.kaazing.com","/echo"))
	if (tNeoway590.clientConnect("gabitame.ddns.net","/ws"))
	{
		/*
		if (tNeoway590.sendTXT("at+test"))
		{
			LOG_DEBUG("PRocesar");
			tNeoway590.ProcessResults(500);
		}
		else
		{
			LOG_DEBUG("Error");
		}
		*/

		uint8_t i=0;
		while (i<15)
		{
				 tNeoway590.ProcessResults(3000);
		//
			//if (tNeoway590.sendTXT("at+result"))
			//{
				//LOG_DEBUG("WS PRocesar");
				//
				//
			//}
			//else
			//{
				//LOG_DEBUG("WS Error");
			//}
			i++;
		}

		pass();
		tNeoway590.clientDisconnect(1113);
	}
	
	}
}

#endif




 /*
 char *ftoa(double f, char *a)
 {
	 // Convert float to ascii!
	 char *ret = a;
	 long heiltal = (long)f;
	 itoa(heiltal, a, 10);
	 while (*a != '\0') a++;
	 *a++ = '.';
	 int desimal = abs((int)((f - heiltal) * 100));  // int is enough for 2 digits
	 if (desimal< 10)  //are there leading zeros?
	 { *a='0'; a++; }
	 itoa(desimal, a, 10);
	 return ret;
 }
 void TestBattery()
 {
	char buffercommand[20];
	//No activamos divisor resistivo pq lo podemos leer desde el pin de arduino.
	//https://rogershobbycenter.com/lipoguide/
	//Baterias lipo- Celda 3.7V
	//3.3V - Corte
	//3.2V - Entrada zona critica
	//3.0V - Posibilidades de jodida..
	//https://forum.arduino.cc/index.php?topic=420527.0 Calculador voltage

	digitalWrite(PIN_CONTROL_BATTERY,HIGH);
	analogReference(INTERNAL);
		
	delay(10);
		memset(buffercommand,0,20);
	double volts = (analogRead(PIN_READ_BATTERY) * 4.675)/1024;
	ftoa(volts,buffercommand);
	LOG_DEBUG_ARGS("Voltage: %s",buffercommand);

	digitalWrite(PIN_CONTROL_BATTERY,LOW);

 }


 void TestAgua()
 {
	 //No activamos divisor resistivo pq lo podemos leer desde el pin de arduino.
	 //https://rogershobbycenter.com/lipoguide/
	
	 
	 delay(10);
	 uint8_t readed= digitalRead(PIN_READ_WATER);

	 if (readed==HIGH)
	 {
		 LOG_DEBUG("Agua presente");
	 	 if (tNeoway590.EstaRegistrado())
	 	 {	
			TestLLamada();
		 }

	}
	 else
	 	LOG_DEBUG("NO hay agua");

	 digitalWrite(PIN_CONTROL_WATER,LOW);
	 //Baterias lipo- Celda 3.7V
	 //3.3V - Corte
	 //3.2V - Entrada zona critica
	 //3.0V - Posibilidades de jodida..
 }
 
 */
    


