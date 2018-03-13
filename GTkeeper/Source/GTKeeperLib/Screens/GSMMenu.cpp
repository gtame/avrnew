/*
 * GSMMenu.cpp
 *
 *  Created on: 09/06/2010
 *      Author: gabi
 */

#include "GSMMenu.h"
#ifdef PANTALLA_TECLADO


GSMMenu::GSMMenu() {


	// TODO Auto-generated constructor stub
}



void GSMMenu::OnEnter()
{


	screenManager.SetOptionMode();

	screenManager.AddFields(0,0,Option,MNU_GSM_TEXT_NUM );
	screenManager.AddFields(0,1,Option,MNU_GSM_TEXT_SMS);
	screenManager.AddFields(0,2,Option,MNU_GSM_TEXT_APN);
	screenManager.AddFields(0,3,Option,MNU_GSM_TEXT_USR);
	screenManager.AddFields(0,4,Option,MNU_GSM_TEXT_PWD);
	if (gtKeeper.IsGSMEnable())
	{
	screenManager.AddFields(0,5,Option,MNU_GSM_TEXT_TEST_SMS);
	screenManager.AddFields(0,6,Option,MNU_GSM_TEXT_TEST_WEB);
	}

}


bool GSMMenu::SetKey(unsigned char c)
{
	  //Tecla cancel
	  if (c=='C')
	  {
		  screenManager.SetPantallaActual((ScreenBase *)&menuScreen);
		  return false;
	  }

	  return true;
}



void GSMMenu::OnClickButton(uint8_t field)
{

	LOG_DEBUG_ARGS("Button %i",field);

	switch (field)
	{
		case 0: //GSM
			screenManager.SetPantallaActual((ScreenBase *)& gsmTelefonoScreen);
			break;
		case 1: //HORA
			screenManager.SetPantallaActual((ScreenBase*)&gsmSmsAvisoScreen);
			break;
		case 2: //SISTEMA
			screenManager.SetPantallaActual((ScreenBase*)&gsmapnScreen);
			break;
		case 3: //SISTEMA
			screenManager.SetPantallaActual((ScreenBase*)&gsmuserapnScreen);
			break;

		case 4:
			screenManager.SetPantallaActual((ScreenBase*)&gsmpwdapnScreen);
			break;
		case 5:
				{

					//Testing :>

					//Limpiamos pantalla
					screenManager.m_lcd->clear();
					screenManager.m_lcd->noCursor();

					//INicializando..
								//Peticion WEb
					if (isReady())
					{

						 if (gtKeeper.Sms(gtKeeper.config.MovilAviso,"Sms Test")==RX_CHECK_OK)
						 {
							 screenManager.PrintTextLine(3,"ENVIO SMS", "OK");
						 }
						 else
							 screenManager.PrintTextLine(3,"ENVIO SMS", "ERROR");
					}



					 delay(5000);

					 screenManager.m_lcd->cursor();
					 screenManager.DrawFields(true);

							//Pues probaremos a ver que tal..

				}
				 break;
		case 6:
		{
			//Testing :>

			//Limpiamos pantalla
			screenManager.m_lcd->clear();
			screenManager.m_lcd->noCursor();

			//INicializando..
						//Peticion WEb
			if (isReady())
			{

				 if (gtKeeper.URLRequest("http://www.google.es",true,NULL,NULL))
				 {
					 screenManager.PrintTextLine_P(3,TXT_WWW, TXT_OK);
				 }
				 else
					 screenManager.PrintTextLine_P(3,TXT_WWW, TXT_ERROR);
			}



			 delay(5000);

			 screenManager.m_lcd->cursor();
			 screenManager.DrawFields(true);
		}
			//Pues probaremos a ver que tal..
			break;


	}

}


bool GSMMenu::isReady()
{
	 if (gtKeeper.EstaArrancado())
	 {

		 gtKeeper.CargaConfigWeb();

		 screenManager.PrintTextLine_P(0,TXT_MOD_GSM, TXT_OK);

		 delay(100);
		 if (gtKeeper.CheckSIM())
		 {
			 screenManager.PrintTextLine_P(1,TXT_SIM,  TXT_OK);
			 delay(100);
			 if (gtKeeper.EstaRegistradoGSM())
			 {
				 screenManager.PrintTextLine_P(2,TXT_CONECTIVIDAD,  TXT_OK);
				 delay(100);
				 return true;
			 }
			 else
				 screenManager.PrintTextLine_P(2,TXT_CONECTIVIDAD,TXT_ERROR);
		 }
		 else
			 screenManager.PrintTextLine_P(1,TXT_SIM, TXT_ERROR);
	 }
	 else
		 screenManager.PrintTextLine_P(0,TXT_MOD_GSM, TXT_ERROR);

	 return false;
}

//Constructor de menuScreen
GSMMenu gsmMenuScreen;
#endif
