/*
 * ScreenBase.cpp
 *
 *  Created on: 18/09/2010
 *      Author: Admin
 */

#include "ScreenBase.h"
#include "ScreenManager.h"


ScreenBase::ScreenBase() {
	// TODO Auto-generated constructor stub

}

void ScreenBase::OnEnter(){}
void ScreenBase::OnExit(){}
void ScreenBase::OnPaint(){}
void ScreenBase::OnClickButton(uint8_t field){}
void ScreenBase::OnOkButton(){}
void ScreenBase::OnCancelButton(){}
void ScreenBase::OnDrawFields(){}
void ScreenBase::OnFieldChange(uint8_t field){}
void ScreenBase::OnChangePage(uint8_t field)

{

}

void ScreenBase::OnSetFocus(uint8_t* field,uint8_t* selpos){}

bool ScreenBase::SetKey(unsigned char c)
{
	return true;
}


bool ScreenBase::NeedRefresh()
{
	return screenManager.needrefresh;
}
/*
virtual void OnEnter();
virtual void OnExit();
virtual void Paint();
virtual void ClickButton(uint8_t field);
virtual void OkButton();
virtual void CancelButton();
virtual void DrawFields();
virtual void OnChangePage();
*/
