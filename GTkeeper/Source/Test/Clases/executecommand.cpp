///*
 //* executecommand.cpp
 //*
 //* Created: 17/03/2018 15:49:05
 //*  Author: Gabi
 //*/ 
 #include <main.h>


 test(ExecuteCommand)
 {

	 
	 //Configuro
	 strcpy(Config.config.PasswordSMS,"1111");

	 //Lanza programaa no existe
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#P53");
	 assertFalse(gtKeeper.ExecuteCommand((buffer_test)));
	 
	 //Lanza sector 1
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#01");
	 assertTrue(gtKeeper.ExecuteCommand(buffer_test));

	 //Lanza programa
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#P1");
	 assertTrue(gtKeeper.ExecuteCommand(buffer_test));
	 

	 //Lanza sector no existe
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#045");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
	 //PAsssword Incorrecta
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1011#P1");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
	 //Comando 101
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#101");
	 assertTrue(gtKeeper.ExecuteCommand(buffer_test));

	 //Comando Alfanumerico
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#xad");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
	 //Comando Largismooo
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#1231231231233");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
	 //Comando Largismooo alfanumerico
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#123123123123xxxxxxxxxxx");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
	 //Comando Incorrecto
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#121");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));

	 //Comando missing
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1111#");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));

	 //PAsssword Incorrecta
	 memset(buffer_test,0,MAIN_BUFFER_SIZE);
	 strcpy(buffer_test,"#1110#11");
	 assertFalse(gtKeeper.ExecuteCommand(buffer_test));
	 
 }

