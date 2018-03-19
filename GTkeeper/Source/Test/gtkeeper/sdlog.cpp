/*
 * sdlog.cpp
 *
 * Created: 19/03/2018 12:36:50
 *  Author: gtame
 */ 
#include "..\main.h"

test (sdlog)
{
	if (gtKeeper.WriteLog("claro que hace")==-1)
	{
		//LOG_DEBUG("SD NO inicializada");
		fail();
		
	}
	else
		pass();
}