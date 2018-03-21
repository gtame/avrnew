/*
 * LongTextBase.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "GSMUserAPN.h"
#ifdef PANTALLA_TECLADO
GSMUserAPNScreen gsmuserapnScreen;

GSMUserAPNScreen::GSMUserAPNScreen() {
	// TODO Auto-generated constructor stub

}



void  GSMUserAPNScreen::OnSave ()
{
	char *val1=screenManager.GetValueField(0);
	if (strlen(val1)>0)
	{
		memset(Config.config.userAPN,0,25);
		strcpy(Config.config.userAPN,val1);

		char *val2=screenManager.GetValueField(1);
		if (strlen(val2)>0)
			strcpy(Config.config.userAPN+strlen(Config.config.userAPN),val2);

		Config.EEPROMGuardaConfig();
		GSMModem.CargaConfigWeb();
		OnGotoMenu();
	}


}


void  GSMUserAPNScreen::OnGotoMenu()
{

	screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
}


char* GSMUserAPNScreen::GetValue()
{
	return Config.config.userAPN;
}



char* GSMUserAPNScreen::GetTitle()
{
	return "User APN:";
}


#endif
