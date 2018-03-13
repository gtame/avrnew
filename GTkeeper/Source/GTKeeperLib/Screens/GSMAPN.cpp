/*
 * LongTextBase.cpp
 *
 *  Created on: 11/06/2010
 *      Author: gabi
 */

#include "GSMAPN.h"
#ifdef PANTALLA_TECLADO
GSMAPNScreen gsmapnScreen;

GSMAPNScreen::GSMAPNScreen() {
	// TODO Auto-generated constructor stub

}



void  GSMAPNScreen::OnSave ()
{
	char *val1=screenManager.GetValueField(0);
	if (strlen(val1)>0)
	{
		memset(gtKeeper.config.APN,0,25);
		strcpy(gtKeeper.config.APN,val1);

		char *val2=screenManager.GetValueField(1);
		if (strlen(val2)>0)
			strcpy(gtKeeper.config.APN+strlen(gtKeeper.config.APN),val2);

		gtKeeper.EEPROMGuardaConfig();
		gtKeeper.CargaConfigWeb();
		OnGotoMenu();
	}
	else
		screenManager.ShowMsgBox("ATENCION:\nAPN Vacio",MsgOkButton);


}


void  GSMAPNScreen::OnGotoMenu()
{

	screenManager.SetPantallaActual((ScreenBase *)&gsmMenuScreen);
}


char* GSMAPNScreen::GetValue()
{
	return gtKeeper.config.APN;
}



char* GSMAPNScreen::GetTitle()
{
	return "Configurar APN:";
}


#endif
