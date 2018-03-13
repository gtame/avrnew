
/*
 * Run.cpp
 *
 * Created: 13/03/2018 16:21:00
 *  Author: Gabi
 */ 
   #include <gtkeeper.h>

   bool GTKeeper::CheckRun()
   {
		//Comprobaremos si toca parar// lanzar 
	   return true;
   }
   
   void GTKeeper::OnRun()
   {
	   LOG_DEBUG("OnRun");


		//Sleep();
   }
   
   
   void GTKeeper::OnLeaveRun()
   {
   	   LOG_DEBUG("OnLeaveRun");
   }
