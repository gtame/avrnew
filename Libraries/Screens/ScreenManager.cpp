/*
 * Pantalla.cpp
 *
 *  Created on: 08/06/2010
 *      Author: gabi
 */



#include "ScreenManager.h"

//Gestor de pantallas
ScreenManager screenManager;

ScreenManager::ScreenManager() {
	// TODO Auto-generated constructor stub

	bisInactive=true;
	bOptionMode=false;
	_lastActivity=0;
	inactivity_seconds=60;
	memset(BufferScreen,0, SC_BUFFER_SIZE);
	isMessageBox=false;


}

void ScreenManager::Apagar()
{

	//Inactivar lcd
	m_lcd->noDisplay();
	m_lcd->noBacklight();
	bisInactive=true;
}

void ScreenManager::Encender()
{
	//Inactivar lcd
	m_lcd->clear();//Limpia pantalla
	m_lcd->backlight();
	m_lcd->display();
	_lastActivity=now();
	needrefresh=true;
	bisInactive=false;
}



void ScreenManager::SetPantallaActual(ScreenBase* screen)
{
	if (isMessageBox) _ExitMsgbox();
	memset(BufferScreen,0, SC_BUFFER_SIZE);
	this->frame=screen;
	OnEnter();
	needrefresh=true;
	_lastActivity=now();
}


int8_t ScreenManager::ShowMsgBox(char *msg,msgboxmode msgmode,uint8_t delayseconds)
{
	static uint8_t result=-1;

	result=-1;

	//Solicitamos confirmación
	this->ShowMsgBox(
	msg,
	msgmode,
	[](uint8_t button,uint8_t func) {
		//Flag para salir del bucle loop.
		result=button;
	}
	);
	time_t timestamp=now();

	while(result==-1 && (delayseconds==0 || ELAPSED_SECONDS(timestamp)<delayseconds))
	this->Loop();


	return result;
}

int8_t ScreenManager::ShowMsgBox_P(PGM_P msg,msgboxmode msgmode,uint8_t delayseconds)
{
	static int8_t result=-1;

	result=-1;

	//Solicitamos confirmación
	this->ShowMsgBox_P(
	msg,
	msgmode,
	[](uint8_t button,uint8_t func) {
		//Flag para salir del bucle loop.
		result=button;
	}
	);
	time_t timestamp=now();

	while(result==-1 && (delayseconds==0 || ELAPSED_SECONDS(timestamp)<delayseconds))
		this->Loop();


	return result;

}

void ScreenManager::ShowMsgBox_P(PGM_P msg,msgboxmode msgmode,void (*MsgFunc)( uint8_t result,uint8_t codefunc ))
{
	//Mostramos el msgbox ;)
	memset(BufferScreen,0,SC_BUFFER_SIZE);
	strncpy_P(BufferScreen,msg,SC_BUFFER_SIZE);
	ShowMsgBox(BufferScreen, msgmode,MsgFunc);

}


void ScreenManager::ShowMsgBox(char *msg,msgboxmode msgmode,void (*MsgFunc)( uint8_t result,uint8_t codefunc ))
{
	//Mostramos el msgbox ;)
	isMessageBox=true;
	msgboxtxt=msg;
	msgboxmod=msgmode;
	MsgboxCallBack=MsgFunc;
	msgboxbtn=0;
	needrefresh=true;

}

void ScreenManager::_ExitMsgbox()
{
	if (isMessageBox)
	{
	isMessageBox=false;
	msgboxtxt=NULL;

	MsgboxCallBack=NULL;

	m_lcd->clear();

	screenManager.DrawFields(true);
	SetFocus(fieldpos,0);
	}
}

void ScreenManager::Loop()
{


	//DDEBUG("========BEGIN SCREEN LOOP========");



	char key = m_keypad->getKey();

	if (key != NO_KEY)
	{

		LOG_DEBUG_ARGS("Tecla pulsada %c",key);

		//Si no esta activa la encendemos
		//Encdemos pero esa tecla no la registramos
		if (!IsActive())
		{
			this->Encender();
			if (bOptionMode)
			{
				DrawFields(true);
				needrefresh=false;
				SetFocus(fieldpos,0);
			}

		}
		else
			//Pasamos la key al frame
			SetKey(key);

		_lastActivity=now();



	}
	//Si pasa el minuto de inactividad apagamos la pantalla
	else if (ELAPSED_SECONDS(_lastActivity)>GetTimeInactivity()  && IsActive() && GetTimeInactivity()!=0)
	{
		//LOG_DEBUG_ARGS("Apagando screen por inactividad %lu %lu %i",_lastActivity,now(), ELAPSED_SECONDS(_lastActivity));
		this->Apagar();
	}


	 if (IsActive())
	 {
		 //Preguntamos al frame
		 if (frame!=NULL && !isMessageBox)
		 {
			 this->needrefresh=frame->NeedRefresh();
		 }


		 //Repaint¿?
		if (needrefresh)
		{
			//LOG_DEBUG("=====PAINT!!======");
			this->Paint();
		}
	 }

	//DDEBUG("========END SCREEN LOOP========");
//	else
//		DDEBUG("No tecla");



}


void ScreenManager::PrintTextLine_P(uint8_t row,PGM_P mensaje,PGM_P result)
{
	memset(buffer,0, sizeof(buffer));
	strcpy_P(buffer, mensaje);

	PrintBufferToLine(row,result,true);
}

void ScreenManager::PrintTextLine(uint8_t row,const char* mensaje, const char* result) {

	memset(buffer,0, sizeof(buffer));
	strcpy(buffer, mensaje);

	PrintBufferToLine(row,result,false);
}

void ScreenManager::PrintBufferToLine(uint8_t row,const char* result,bool isPGM){
	m_lcd->setCursor(0,row);


	for(uint8_t i=strlen(buffer);i<ncols;i++)
		buffer[i]=T_POINTS;

	buffer[ncols] = '\0';

	if (isPGM)
		strcpy_P(buffer + (strlen(buffer) - strlen_P(result)), result);
	else
		strcpy(buffer + (strlen(buffer) - strlen(result)), result);

	m_lcd->print(buffer);

}


uint8_t ScreenManager::GetCursorPos()
{
	return this->fieldpos;
}
void ScreenManager::Initializate(LiquidCrystal_I2C * lcd,uint8_t cols,uint8_t rows,Keypad *keypad)
{
	m_lcd=lcd;
	ncols=cols;
	nrows=rows;
	m_keypad=keypad ;
}


//Comment:Si pasamos un 0 quiere indicar que no es TextMode
void ScreenManager::SetOptionMode()
{
		bOptionMode=true;
		actualpage=255;//La inicializamos para que cambie y se redibujen
}



void ScreenManager::SetTextMode()
{

		bOptionMode=false;
}

void ScreenManager::Paint()
{

	if (frame==NULL && !isMessageBox)
		return;


	  if (frame!=NULL && !isMessageBox)
		  frame->OnPaint();

	   // Print a message to the LCD.
	  if (needrefresh)
	  {


		  m_lcd->noBlink();
		   m_lcd->clear();

		  if (isMessageBox)
		  {


			  //DrawFields (Botones y texto)

			  //Opcion trozeo de salto de linea
			  //Se probo con strtok pero daba problemas con el 2º refresco
			  //Tambien se probo a trozear por longitud ..

			  const int splitchar='\n';

			  uint8_t linea=0;
			  char * ptrant=msgboxtxt;
			  char * ptr;
			  while ((ptr=strchr(ptrant, splitchar)))
			  {
				  memset(buffer,0,INTERNAL_SC_BUFFER_SIZE);
				  strncpy(buffer,ptrant,  ptr-ptrant);

				  m_lcd->setCursor(linea,0);
				  m_lcd->print(buffer);
				  ptrant=ptr+1;
				  linea++;
			  }
			  m_lcd->setCursor(0,linea);
		  	 m_lcd->print(ptrant);


/*
  		  	  char *token=strtok(msgboxtxt, splitchar);
				while (token !=NULL )
				{
					m_lcd->setCursor(0,linea);
					m_lcd->print(token);
					token=strtok(NULL,splitchar);
					linea++;
				}
*/


				//Opcion de trozear por longitud
				/*
			  char *msgboxt=msgboxtxt;
			  uint8_t len=strlen(msgboxt);
			  uint8_t linea=0;
			  while (len>0)
			  {
				  if (len>20)
					  len=20;

				  strncpy(buffer,msgboxt,20);
				  m_lcd->setCursor(0,linea);
				  m_lcd->print(buffer);

				  msgboxt=msgboxt+len;

				  len=strlen(msgboxt);
				  linea++;
			  }*/
			  switch (msgboxmod)
			  {
			  	  case MsgOkButton:
			  	  {
			  		  m_lcd->setCursor(8,3);
			  		  m_lcd->print("OK");
			  		  m_lcd->setCursor(8,3);
			  	  }
			  		  break;
			  	  case MsgOKCancelButton:
			  	  {
			  		  m_lcd->setCursor(3,3);
			  		  m_lcd->print("OK");
			  		  m_lcd->setCursor(13,3);
			  		  m_lcd->print("CANCEL");


			  		  if (msgboxbtn==0)
			  			m_lcd->setCursor(3,3);
			  		  else
			  			m_lcd->setCursor(13,3);
			  	  }

			  		  break;
			  	  case MsgYesNoButton:
			  	  {
			  		  m_lcd->setCursor(3,3);
			  		  m_lcd->print("YES");
			  		  m_lcd->setCursor(13,3);
			  		  m_lcd->print("NO");

			  		  if (msgboxbtn==0)
			  			m_lcd->setCursor(3,3);
			  		  else
			  			m_lcd->setCursor(13,3);
			  	  }
			  		  break;
			  }



			  //Ponemos lcd blink
			  m_lcd->blink();

		  }
		  else
		  {
			  //Draw fields..
			   if (frame!=NULL)
			   {
				  frame->OnDrawFields();
			   }


			  DrawFields();



			 // if (!bOptionMode)
			  m_lcd->setCursor(posX,posY);


			  if (frame!=NULL)
			  {
				  if (frame->ShowBlink())
					  m_lcd->blink();
				  else
					  m_lcd->blink_off();

				  if (frame->ShowCursor())
					  m_lcd->cursor_on();
				  else
					  m_lcd->cursor_off();
			  }
			  else
				  m_lcd->blink();
		  }
		  needrefresh=false;
	  }
}

void ScreenManager::DrawFields(bool force)
{

	if (!isMessageBox)
		{


			if (bOptionMode)
			{

				//Serial.print("Fieldpos-->");
				//Serial.println(fieldpos,DEC);
				uint8_t new_actualpage=ceil(((double)(fieldpos+1)/(double)nrows));
				new_actualpage--;

				LOG_DEBUG_ARGS("P %i N %i",actualpage,new_actualpage);

				//Si no son distintos nº pages -- No haria falata actualizar
				if (new_actualpage!=actualpage || force)
				{

					frame->OnChangePage(new_actualpage);
					actualpage=new_actualpage;




				//Para determinar los campos a dibujar nos posicionaremos en la pagina actual x rows
				uint8_t desde=(actualpage*nrows);
				uint8_t hasta=(fieldscount>(actualpage*nrows)+nrows?(actualpage*nrows)+nrows:fieldscount);


				//Limpiamos para escribri
				m_lcd->clear();


					//Para el caso normal...
					for (uint8_t i=desde;i<hasta;i++)
					{
						if (i<fieldscount)
						{
							m_lcd->setCursor(fields[i].posX,fields[i].posY - (actualpage*nrows));
							m_lcd->print(fields[i].value);
						}
					}
				}



			}
			else
			{
				for (uint8_t i=0;i<fieldscount;i++)
				{
					m_lcd->setCursor(this->fields[i].posX,this->fields[i].posY);
					memset(buffer,0,sizeof(buffer));

					if (fields[i].visible)
					{
						memcpy(buffer,this->fields[i].value,this->fields[i].length);
						m_lcd->print(buffer);
					}
					else
					{
						//Para hacer que desaparezcan mandamos a 'imprimir' vacios :)
						//Si no si no hay refresh de toda la pantalla no se ocultaban
						memset(buffer,' ',this->fields[i].length);
						m_lcd->print(buffer);
					}
				}
			}
		}

}
void ScreenManager::DrawFields()
{
	DrawFields(false);
}

void ScreenManager::SetCursorPos(uint8_t X,uint8_t Y)
{
	posX=X;
	posY=Y;
	m_lcd->setCursor(X,Y);
}


void ScreenManager::SetFocus(uint8_t field,uint8_t selpos)
{
	if (!isMessageBox)
	{

		if (frame!=NULL)
			frame->OnSetFocus(&field,&selpos);

		fieldpos=field;

		if (bOptionMode)
		{
			//Llamamos a drawfield para que se pagine de modo correcto
			DrawFields();
			//actualpage=ceil(((double)(fieldpos+1)/(double)nrows));
			//actualpage--;



		}
		SetCursorPos(fields[field].posX+selpos,fields[field].posY - (actualpage*nrows));
	}
}
void ScreenManager::PutChar(unsigned char c)
{
	if (!isMessageBox)
	{
		//LOG_DEBUG_ARGS("Pulsada tecla %i -> %c -> valor %s",fieldpos,c, fields[fieldpos].value);

		//Comprobamos si estamos ya en el tope del campo
		if ((fields[fieldpos].length-1)>sellength)
		{
			//Ponemos la tecla pulsada en el valor de la variable
			fields[fieldpos].value[sellength]=c;
			sellength++;
			SetFocus(fieldpos,sellength);

		}
		else
		{
			if (sellength>=fields[fieldpos].length)
			{
				if (fields[fieldpos].editmode==Estandar)
				{
					//truncamos un caracter por la derecha y agregamos a la izqda
					for(uint8_t i=1;i<fields[fieldpos].length;i++)
					{
						fields[fieldpos].value[i-1]=fields[fieldpos].value[i];
					}
				}
			}
			else
				sellength++;

			fields[fieldpos].value[fields[fieldpos].length-1]=c;
		}

		//LOG_DEBUG_ARGS("Valor despues %s", fields[fieldpos].value);

		frame->OnFieldChange(fieldpos);
	}
}
/*
int ScreenManager::SendToBuffer(const char *__fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap,__fmt);
	memset(buffer,0,sizeof(buffer));
	ret = vsprintf(buffer,__fmt, ap);
	va_end(ap);
	return (ret);
}*/

void ScreenManager::SetKey(unsigned char c)
{

		LOG_DEBUG_ARGS("Procesando tecla %c",c);


		//"Salvapantallas"
		_lastActivity=now();
		 if (isMessageBox)
		 {

			 LOG_DEBUG_ARGS("tecla MsgBox %c %i",c,msgboxmod);
			 //Si es el tabulador
			 if (c=='*')
			 {
				 //Cambia de foco si procede.
				 //Dependiendo del modo que este moveremos el foco al boton correspondiente.
				 if(msgboxmod!=MsgOkButton)
				 {
					 if (msgboxbtn==0)
						 msgboxbtn=1;
					 else
						 msgboxbtn=0;

				 }
				 else
					 msgboxbtn=0;


				  if (msgboxbtn==0)
					m_lcd->setCursor(3,3);
				  else
					m_lcd->setCursor(13,3);

				// needrefresh=true;

			 }
			 else if (c=='#')
			 {
				 if (MsgboxCallBack!=NULL)
					 MsgboxCallBack(msgboxbtn,msgboxbtn);

				 _ExitMsgbox();
			 }

			 return;
		 }
		//Si no se desea procesar..
		 else if (frame==NULL || !frame->SetKey(c))
		{
			LOG_DEBUG_ARGS("No procesada tecla %c",c);
			return;
		}
		//Tabulador
		else if (c=='*')
		{

			//Serial.print("Comienza!!");
			uint8_t fieldposinit=fieldpos;
			bool blnContinuar=true;
			while (blnContinuar)
			{
				fieldpos++;


				sellength=0;
				if (fieldpos>=fieldscount) fieldpos=0;
				if (fields[fieldpos].enabled && fields[fieldpos].visible)
				{
					SetFocus(fieldpos,0);
					blnContinuar=false;
				}
				else if (fieldposinit==fieldpos)
					blnContinuar=false;

			}
			//Si es tabulacion...
			return;
			//Serial.print("Posicion->");
			//Serial.println(fieldpos,DEC);

		}

		//Teclas numericas
		else if (c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9')
		{

			if (bOptionMode)
			{
				 char num[2];
				 memset(num,'\0',2);
				 num[0]=c;
				 uint8_t pos=atoi(num);
				 pos--;


				 //Si la tecla pulsada esta dentro de las opciones disponibles y no es cero
				 if (fieldscount>=pos && pos>=0)
				 {
					 if (fields[pos].enabled && fields[pos].visible)
						 //Ponemos el foco sobre la opcion
						 SetFocus(pos,0);
				 }
			}
			else if (fields[fieldpos].type==TextEdit || fields[fieldpos].type==AlfaTextEdit)
			{
				if (fields[fieldpos].enabled)
					PutChar(c);
			}
		}
		//Enter
		else if (c=='#')
		{
			//Serial.print("FieldPos->");
			//Serial.println(fieldpos,DEC);

			if (fields[fieldpos].type==Button || fields[fieldpos].type==Option)
			{
				if (fields[fieldpos].enabled)
				{
				//Serial.print("Click!!");
					if (frame!=NULL)
						frame->OnClickButton(fieldpos);
					ClickButton(fieldpos);
				}
			}
			else if (fields[fieldpos].type==AlfaTextEdit)
			{
				//Hay que tener en cuenta el offset para aquellos campos que no quepan en el display
				if ((fields[fieldpos].length-1)>sellength)
					sellength++;
				else
				{
					sellength=0;
				}

				SetFocus(fieldpos,sellength);

			}
		}
		//Si el campo es Numerico las teclas de up & Down seran utilizadas para incrementar valores y B para resetear al min
		else if (c=='A' || c=='D' || c=='B')
		{


				if (fields[fieldpos].type==TextEdit )
				{
					if (fields[fieldpos].maxvalue!=fields[fieldpos].minvalue)
					{

						char * cval=GetValueField(fieldpos);
						uint16_t valor= atoi(cval);



						//Sube
						if (c=='A')
							valor++;
						else if (c=='D')
							valor--;
						else if  (c=='B')
							valor=fields[fieldpos].minvalue;

						if (valor<fields[fieldpos].minvalue)
							valor=fields[fieldpos].maxvalue;
						else if (valor>fields[fieldpos].maxvalue)
							valor=fields[fieldpos].minvalue;


						memset(buffer,0,INTERNAL_SC_BUFFER_SIZE);
						//Por ej->%02i
						sprintf(buffer,"%c0%ii",'%', fields[fieldpos].length);
						sprintf(buffer+5,buffer,valor);
						strncpy(fields[fieldpos].value,buffer+5,strlen(buffer+5));
						frame->OnFieldChange(fieldpos);

					}
				}
				else if (fields[fieldpos].type==AlfaTextEdit )
				{
					//desde el 45=. al 57 y del 64=@ 65->'A' al 90 ->'Z'.
					//Hay que tener en cuenta el offset para aquellos campos que no quepan en el display
					char caracter=fields[fieldpos].value[sellength];

					char nextchar=65;
					if (caracter!=0)
					{
						caracter+=(c=='A'?1:-1);

					  if (caracter>90) nextchar=45;
					  else if (caracter==58) nextchar=64;
					  else if (caracter==63) nextchar=57;
					  else if (caracter<45) nextchar=90;
					  else nextchar=caracter;

					}

					fields[fieldpos].value[sellength]=nextchar;
					frame->OnFieldChange(fieldpos);
				}


		}


		 //Si no necesita dibujarse entera
		 //pintamos los fields de datos y situamos el cursor
		 if (!needrefresh)
		 {
			 DrawFields();
			 m_lcd->setCursor(posX,posY);
		 }
		//needrefresh=true;


}

void ScreenManager::OkButton()
{
	if (frame!=NULL )
		frame->OnOkButton();
}

void ScreenManager::CancelButton()
{
	if (frame!=NULL )
		frame->OnCancelButton();
}

void ScreenManager::ClickButton(uint8_t field)
{


		if (strcmp(fields[field].value,OK_TEXT)==0)
		{
			//Validamos && Aceptamos
			OkButton();
		}
		else if (strcmp(fields[field].value,CANCEL_TEXT)==0)
		{
			//Cancelamos
			CancelButton();
		}


}
 char* ScreenManager::GetValueField(uint8_t field)
{
//	char fieldvalue[this->fields[field].length];
	memset(buffer,0,INTERNAL_SC_BUFFER_SIZE);
	strncpy(buffer,this->fields[field].value,this->fields[field].length);
	return buffer;
}

void ScreenManager::SetTextField(uint8_t field,const char* txt)
{
		memcpy(this->fields[field].value,txt,strlen(txt));
}


void ScreenManager::AddFields(uint8_t posX, uint8_t posY, fieldstype_t type,const char* txt)
{
	this->AddFields(posX,posY,strlen(txt)+1,type,txt);
}


void ScreenManager::AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type,uint8_t minvalue,uint8_t maxvalue)
{
	this->AddFields(posX,posY,length,type);
	this->fields[fieldscount-1].maxvalue=maxvalue;
	this->fields[fieldscount-1].minvalue=minvalue;
}

void ScreenManager::AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type,const char* txt)
{
	this->AddFields(posX,posY,length,type);
 	memcpy(this->fields[fieldscount-1].value,txt,strlen(txt));
}

void ScreenManager::AddFields(uint8_t posX, uint8_t posY, uint8_t length,fieldstype_t type)
{


	this->fields[fieldscount].posY=posY;
	this->fields[fieldscount].posX=posX;
	this->fields[fieldscount].length=length;
	this->fields[fieldscount].type=type;
	this->fields[fieldscount].enabled=true;
	this->fields[fieldscount].visible=true;
	this->fields[fieldscount].editmode=Estandar;
	this->fields[fieldscount].maxvalue=0;
	this->fields[fieldscount].minvalue=0;


	//Reservamos memoria para la longitud de campo
	this->fields[fieldscount].value= this->strings+lenstring;





	//Halla la longitud del string
	this->lenstring+=length;


	//fieldscount=(sizeof(fields)/sizeof(t_fields));


	this->fieldscount++;



}


void ScreenManager::ClearFields()
{
	//for (uint8_t i=0;i<fieldscount;i++)
	//	free(fields[i].value);

	//Limpiamos contenido fields
	memset(this->strings,0,SC_MAX_TEXT_FIELDS);
	memset(this->fields,0,sizeof(t_fields)*(SC_MAX_FIELDS_SCREEN));

	//reset vars
	this->fieldscount=0;
	this->fieldpos=0;
	this->sellength=0;
	this->actualpage=0;
	this->posX=0;
	this->posY=0;
	this->lenstring=0;
//	needrefresh=true;

}





void ScreenManager::OnEnter()
{

	this->ClearFields();

	this->frame->OnEnter();

	this->needrefresh=true;


}

void ScreenManager::OnExit()
{
	this->frame->OnExit();
	this->needrefresh=true;
}


