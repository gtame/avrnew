/*
 * LongTextBase.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "GSMPwdAPN.h"
#ifdef PANTALLA_TECLADO
GSMPwdAPNScreen gsmpwdapnScreen;

GSMPwdAPNScreen::GSMPwdAPNScreen() {
	// TODO Auto-generated constructor stub

}



void  GSMPwdAPNScreen::OnSave ()
{
	char *val1=screenManager.GetValueField(0);
	if (strlen(val1)>0)
	{
		memset(gtKeeper.config.pwdAPN,0,25);
		strcpy(gtKeeper.config.pwdAPN,val1);

		char *val2=screenManager.GetValueField(1);
		if (strlen(val2)>0)
			strcpy(gtKeeper.config.pwdAPN+strlen(gtKeeper.config.pwdAPN),val2);

		gtKeeper.EEPROMGuardaConfig();
		gtKeeper.CargaConfigWeb();

		OnGotoMenu();
	}


}


void  GSMPwdAPNScreen::OnGotoMenu()
{

	screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
}


char* GSMPwdAPNScreen::GetValue()
{
	return gtKeeper.config.pwdAPN;
}



char* GSMPwdAPNScreen::GetTitle()
{
	return "Pass APN:";
}

#endif

