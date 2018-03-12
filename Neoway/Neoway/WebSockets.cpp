/**
 * @file WebSockets.cpp
 * @date 20.05.2015
 * @author Markus Sattler
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the WebSockets for Arduino.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "WebSockets.h"


 const  char WebSockets::HTTP_BOUNDARY[] PROGMEM= "---------------------------114782935826962";
 const  char WebSockets::HTTP_TICKS[] PROGMEM="--";
 const  char WebSockets::HTTP_HEADER_HOST[] PROGMEM ="Host: %s";
 const  char WebSockets::HTTP_HEADER_CONTENT_TYPE_TEXT[] PROGMEM="Content-type: text/html";
 const  char WebSockets::HTTP_HEADER_CONTENT_LENGTH[] PROGMEM="Content-Length: %i";
 const  char WebSockets::AT_TCPSEND_COMMAND[] PROGMEM= "AT+TCPSEND=1,%i";
 const  char WebSockets::AT_TCPCLOSE_RESPONSE[] PROGMEM = "+TCPCLOSE:1";


 const char WebSockets::MSG_INFO_CONNECT[] PROGMEM	= "Conectado";
 const char WebSockets::MSG_ERR_RESPOND[] PROGMEM	= "No responde";
 const char WebSockets::MSG_ERR_INPUT[] PROGMEM	= "No inputmode";
 const char WebSockets::MSG_ERR_CONNECT[] PROGMEM	= "No Conecta";





WebSockets::WebSockets(Stream * stream):ATSerial(stream) {	// TODO Auto-generated constructor stub
WSMessageHeader_t miheader;
header=&miheader;

}


uint8_t  WebSockets::_ReceiveData( uint8_t length, int (*tcpreadcallback) (char * ,int ))
{

	
	int leidos=0;
	char *result=GetLastResponse();

	if (strlen(result)>length)
	{
		LOG_DEBUG("====RECEIVE DATA===");

		//Obtenemos length
		char * resultlen=GetToken(result+length,0,",");
		uint16_t tcpresponselen=atoi(resultlen);
		result=GetNextToken(0,",");
		//int leidos=strlen(result)+2; //El+2 es porque se lee un salto de linea final

		if (tcpreadcallback!=NULL)
		int leidos=tcpreadcallback(result,tcpresponselen);
		else
		{
			

			leidos= tcpresponselen;
			while (mSerial->available())
			{
				if (mSerial->available())
				{
					char c=mSerial->read();
					Serial.print(c);
					leidos++;
				}
			}

			
			leidos= tcpresponselen;
		}
		
		if (tcpresponselen==leidos)
		return RX_CHECK_OK;
		else
		return RX_CHECK_KO;

	}
	else
	return RX_NO_DATA;

}





#ifdef WSOCKETS

bool WebSockets::clientConnect(char* host,char* urlpath)
{

/* ------------ REQUEST -------------------*/
//GET /echo HTTP/1.1
//Host: demos.kaazing.com
//Upgrade: websocket
//Connection: Upgrade
//Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
//Sec-WebSocket-Version: 13

//


	if (connectedWS)
		LOG_DEBUG("YA CONECTADO");
	else
	{
		 
		if (TCPOpenHost(host,80))
		{
				
				
				//const uint8_t CARACTERES_NUM =134;
				const uint8_t CARACTERES_NUM =138;
				uint8_t  requestlen= CARACTERES_NUM + strlen(host) +strlen(urlpath);
				//requestlen=160;
				uint16_t size=0;


				SendCommandAsync((__FlashStringHelper *)AT_TCPSEND_COMMAND,requestlen);
				//SendRawData(AT_ENDLINE);
				if (CheckInputMode())
				{

						size=SendCommandAsync(F("GET %s HTTP/1.1"),urlpath);
						size+=SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_HOST,host);
						size+=SendCommandAsync(F("Upgrade: websocket"));
						size+=SendCommandAsync(F("Connection: Upgrade"));

						size+=SendRawData(F("Sec-WebSocket-Key: "));
						size+=SendRawData(F("dGhlIHNhbXBsZSBub25jZQ=="));
						SendEndLine();

						size+=SendCommandAsync(F("Sec-WebSocket-Version: 13"));
						SendEndLine();
						
						SendRawData(EXIT_INPUTMODE);
						
						/* ------------ RESPONSE -------------------*/
						//HTTP/1.1 101 Web Socket Protocol Handshake
						//Connection: Upgrade
						//Date: Thu, 02 Nov 2017 17:20:55 GMT
						//Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
						//Server: Kaazing Gateway
						//Upgrade: websocket
						 
						uint8_t result=WaitResponseResult(F("Sec-WebSocket-Accept"),(__FlashStringHelper*)ATSerial::AT_ERROR_RESPONSE,15000);
						if (result==RX_CHECK_OK)
						{
							//Procesamos el resto de respuestas
							ProcessResults(1000);
							Log.Info(MSG_INFO_CONNECT);
							connectedWS=true;
						}
						else
						{
							
							Log.Error(MSG_ERR_RESPOND);
							TCPClose();
							connectedWS=false;
						}
				}
				else
				 Log.Error(MSG_ERR_INPUT);
		}
		else
			Log.Error(MSG_ERR_CONNECT);
	}

	return connectedWS;
}

bool  WebSockets::clientIsConnected()
{
	///TODO
	return connectedWS;
}

/**
 *
 * @param client  ptr to the client struct
 * @param code uint16_t see RFC
 * @param reason ptr to the disconnect reason message
 * @param reasonLen length of the disconnect reason message
 */
void WebSockets::clientDisconnect( uint16_t code, char * reason, size_t reasonLen) {
    LOG_DEBUG_ARGS("[WS disconnect] %u", code);
    if( clientIsConnected() && code) {

		isDisconnecting=true;
        if(reason) {
            sendFrame( WSop_close, (uint8_t *) reason, reasonLen);
        } else {
            uint8_t buffer[2];
            buffer[0] = ((code >> 8) & 0xFF);
            buffer[1] = (code & 0xFF);
            sendFrame(WSop_close, &buffer[0],2);
        }

		TCPClose();

		isDisconnecting=false;
		connectedWS=false;
    } 
	
	connectedWS=false;
}
void WebSockets::clientDisconnect( uint16_t code)
{
	clientDisconnect(code,NULL,0);

}
 


/**
 *
  * @param opcode WSopcode_t
  * @param payload uint8_t *     ptr to the payload
  * @param length size_t         length of the payload
  * @param mask bool             add dummy mask to the frame (needed for web browser)
 * @param fin bool              can be used to send data in more then one frame (set fin on the last frame)
 * @param headerToPayload bool  set true if the payload has reserved 14 Byte at the beginning to dynamically add the Header (payload neet to be in RAM!)
 * @return true if ok
 */
bool WebSockets::sendFrame( WSopcode_t opcode, uint8_t * payload, size_t length, bool mask, bool fin, bool headerToPayload) {

     
    if(opcode == WSop_text) {
        LOG_DEBUG_ARGS("[WS][sendFrame] text: %s",  (payload + (headerToPayload ? 14 : 0)));
    }

    uint8_t maskKey[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8_t buffer[WEBSOCKETS_MAX_HEADER_SIZE] = { 0 };

    uint8_t headerSize;
    uint8_t * headerPtr;
    uint8_t * payloadPtr = payload;
    bool useInternBuffer = false;
    bool ret = true;

    // calculate header Size
    if(length < 126) {
        headerSize = 2;
    } else if(length < 0xFFFF) {
        headerSize = 4;
    } else {
        headerSize = 10;
    }

    if(mask) {
        headerSize += 4;
    }


    // set Header Pointer
    if(headerToPayload) {
        // calculate offset in payload
        headerPtr = (payloadPtr + (WEBSOCKETS_MAX_HEADER_SIZE - headerSize));
    } else {
        headerPtr = &buffer[0];
    }

    // create header

    // byte 0
    *headerPtr = 0x00;
    if(fin) {
        *headerPtr |= bit(7);    ///< set Fin
    }
    *headerPtr |= opcode;        ///< set opcode
    headerPtr++;

    // byte 1
    *headerPtr = 0x00;
    if(mask) {
        *headerPtr |= bit(7);    ///< set mask
    }

    if(length < 126) {
        *headerPtr |= length;
        headerPtr++;
    } else if(length < 0xFFFF) {
        *headerPtr |= 126;
        headerPtr++;
        *headerPtr = ((length >> 8) & 0xFF);
        headerPtr++;
        *headerPtr = (length & 0xFF);
        headerPtr++;
    } else {
        // Normally we never get here (to less memory)
        *headerPtr |= 127;
        headerPtr++;
        *headerPtr = 0x00;
        headerPtr++;
        *headerPtr = 0x00;
        headerPtr++;
        *headerPtr = 0x00;
        headerPtr++;
        *headerPtr = 0x00;
        headerPtr++;
        *headerPtr = ((length >> 24) & 0xFF);
        headerPtr++;
        *headerPtr = ((length >> 16) & 0xFF);
        headerPtr++;
        *headerPtr = ((length >> 8) & 0xFF);
        headerPtr++;
        *headerPtr = (length & 0xFF);
        headerPtr++;
    }

    if(mask) {
        if(useInternBuffer) {
            // if we use a Intern Buffer we can modify the data
            // by this fact its possible the do the masking
            for(uint8_t x = 0; x < sizeof(maskKey); x++) {
                maskKey[x] = random(0xFF);
                *headerPtr = maskKey[x];
                headerPtr++;
            }

            uint8_t * dataMaskPtr;

            if(headerToPayload) {
                dataMaskPtr = (payloadPtr + WEBSOCKETS_MAX_HEADER_SIZE);
            } else {
                dataMaskPtr = payloadPtr;
            }

            for(size_t x = 0; x < length; x++) {
                dataMaskPtr[x] = (dataMaskPtr[x] ^ maskKey[x % 4]);
            }

        } else {
            *headerPtr = maskKey[0];
            headerPtr++;
            *headerPtr = maskKey[1];
            headerPtr++;
            *headerPtr = maskKey[2];
            headerPtr++;
            *headerPtr = maskKey[3];
            headerPtr++;
        }
    }

#ifndef NOLOG_DEBUG
    unsigned long start = micros();
#endif

    if(headerToPayload) {
        // header has be added to payload
        // payload is forced to reserved 14 Byte but we may not need all based on the length and mask settings
        // offset in payload is calculatetd 14 - headerSize
        if(write( &payloadPtr[(WEBSOCKETS_MAX_HEADER_SIZE - headerSize)], (length + headerSize)) != (length + headerSize)) {
            ret = false;
        }
    } else {
        // send header
        if(write( &buffer[0], headerSize) != headerSize) {
            ret = false;
        }

        if(payloadPtr && length > 0) {
            // send payload
            if(write( &payloadPtr[0], length) != length) {
                ret = false;
            }
        }
    }
	 
    return ret;
}
bool WebSockets::sendFrame(WSopcode_t opcode, uint8_t * payload, size_t length, bool mask)
{
	return this->sendFrame(opcode,payload,length,mask);
}
bool WebSockets::sendFrame(WSopcode_t opcode, uint8_t * payload, size_t length)
{
	return this->sendFrame(opcode,payload,length,true,true,false);
}

/**
* send text data to client
* @param num uint8_t client id
* @param payload uint8_t *
* @param length size_t
* @param headerToPayload bool  (see sendFrame for more details)
* @return true if ok
*/
//bool WebSockets::sendTXT(uint8_t * payload, size_t length, bool headerToPayload) {

bool WebSockets::sendTXT(const char * txt) {

size_t length=strlen(txt);
memset(buffinternal,0,WS_BUFFER_SIZE);
strncpy(buffinternal+WEBSOCKETS_MAX_HEADER_SIZE,txt,WS_BUFFER_PAYLOAD);

LOG_DEBUG_ARGS("PARCIAL!->%s",buffinternal);
//No acaba el frame
return sendFrame(WSop_text, buffinternal, WS_BUFFER_PAYLOAD,true,true,true);

/*
size_t charsSend=0;
size_t length=strlen(txt);
bool result=true;

	do
	{
		memset(buffinternal,0,WS_BUFFER_SIZE);

		if (WS_BUFFER_PAYLOAD>(length-charsSend))
		{
			
			strncpy(buffinternal+WEBSOCKETS_MAX_HEADER_SIZE,txt+charsSend,(length-charsSend));
			LOG_DEBUG_ARGS("TODO!->%s",buffinternal);
			//acaba el frame
			result=sendFrame( WSop_text, buffinternal, (length-charsSend), true, true, true);
			charsSend=length;

		}		
		else
		{
			
			strncpy(buffinternal+WEBSOCKETS_MAX_HEADER_SIZE,txt+charsSend,WS_BUFFER_PAYLOAD);

			LOG_DEBUG_ARGS("PARCIAL!->%s",buffinternal);
			//No acaba el frame
			result=sendFrame(WSop_text, buffinternal, WS_BUFFER_PAYLOAD,true,false,true);
			charsSend+=WS_BUFFER_PAYLOAD;
		}
		
		LOG_DEBUG_ARGS("RESULT %i %i %i",length,charsSend,result);
	}
	while (length>charsSend && result);
	LOG_DEBUG("RESULT");
	return result;*/
 
}




/**
 * write x byte to tcp or get timeout
 * @param client WSclient_t *
 * @param out  uint8_t * data buffer
 * @param n size_t byte count
 * @return bytes send
 */
size_t WebSockets::write( uint8_t *out, size_t n) {
	if(out == NULL) return 0;

	SendCommandAsync((__FlashStringHelper *)AT_TCPSEND_COMMAND,n);
	//SendRawData(AT_ENDLINE);
	if (CheckInputMode())
	{
		 size_t sizemsg =mSerial->write(out,n);
		 SendRawData(EXIT_INPUTMODE);
		 if (sizemsg)
		 {
			 if (WaitResponseResult(F("+TCPSEND:1,"),F("+TCPSEND:"),15000)!=RX_CHECK_OK)
			 sizemsg=0;
		 }
		 return sizemsg;
	}
	else
		return -1;

}

size_t WebSockets::write( const char *out) {
 	if(out == NULL) return 0;
	return write( (uint8_t*)out, strlen(out));
}



bool WebSockets::OnProcessResult( char * respuesta)
{


	const char COMMA  = ',';
	//LOG_DEBUG	("Proc-WebSockets");
	bool catched=false;
	//Si hay abierto un WS y recibimos el mensaje...
	if (clientIsConnected())
	{
		
		uint8_t result=ExpectedResult(respuesta, F("+TCPRECV:1,"));
		if (result==RX_CHECK_OK)
		{
			//Recibido mensaje
			if (respuesta[12]==COMMA)
				 handleWebsocketCb(respuesta+13);
			else if (respuesta[13]==COMMA)
				handleWebsocketCb(respuesta+14);
			else if (respuesta[14]==COMMA)
				handleWebsocketCb(respuesta+15);
			//else
				//LOG_DEBUG("NO ENCONTRADA, ERROR");

			 catched=true;
		}
		else
			catched=ATSerial::OnProcessResult(respuesta);
	}
	else
		catched=ATSerial::OnProcessResult(respuesta);

	return catched;

}

void WebSockets::handleWebsocketCb(uint8_t * buffer) {

	
	
    uint8_t * payload = NULL;

	   if (mSerial->available())
	   {
		LOG_DEBUG("HAYY!!");
	   }

	// split first 2 bytes in the data
	header->fin = ((*buffer >> 7) & 0x01);
	header->rsv1 = ((*buffer >> 6) & 0x01);
	header->rsv2 = ((*buffer >> 5) & 0x01);
	header->rsv3 = ((*buffer >> 4) & 0x01);
	header->opCode = (WSopcode_t) (*buffer & 0x0F);

	buffer++;
	//LOG_DEBUG_ARGS("Handle %i",strlen(buffer));
	header->mask = ((*buffer >> 7) & 0x01);
	header->payloadLen = (WSopcode_t) (*buffer & 0x7F);
	buffer++;
	
	//
	//Esto es para mensajes largos mas de 125...
    //if(header->payloadLen == 126) {
        //headerLen += 2;
        //if(!handleWebsocketWaitFor( headerLen)) {
            //return;
        //}
        //header->payloadLen = buffer[0] << 8 | buffer[1];
        //buffer += 2;
    //} else if(header->payloadLen == 127) {
        //headerLen += 8;
        //// read 64bit integer as length
        //if(!handleWebsocketWaitFor( headerLen)) {
            //return;
        //}
		//
        //if(buffer[0] != 0 || buffer[1] != 0 || buffer[2] != 0 || buffer[3] != 0) {
            //// really too big!
            //header->payloadLen = 0xFFFFFFFF;
        //} else {
            //header->payloadLen = buffer[4] << 24 | buffer[5] << 16 | buffer[6] << 8 | buffer[7];
        //}
        //buffer += 8;
    //}

    LOG_DEBUG("[WS][handleWebsocket] ------- read massage frame -------\n");
	
  //  LOG_DEBUG_ARGS("[WS][handleWebsocket] fin: %u rsv1: %u rsv2: %u rsv3 %u  opCode: %u\n",header->fin, header->rsv1, header->rsv2, header->rsv3, header->opCode);
 //   LOG_DEBUG_ARGS("[WS][handleWebsocket] mask: %u payloadLen: %u\n", header->mask, header->payloadLen);

    if(header->payloadLen > WEBSOCKETS_MAX_DATA_SIZE) {
        LOG_DEBUG_ARGS("[WS][handleWebsocket] payload too big! (%u)\n",header->payloadLen);
        clientDisconnect( 1009);
        return;
    }

    if(header->mask) {
        //headerLen += 4;
        //if(!handleWebsocketWaitFor( headerLen)) {
            //return;
        //}
        header->maskKey = buffer;
        buffer += 4;
    }

    if(header->payloadLen > 0) {
        // if text data we need one more
        payload = buffer;
        handleWebsocketPayloadCb(true, payload);//,std::bind(&WebSockets::handleWebsocketPayloadCb, this, std::placeholders::_1, std::placeholders::_2, payload));
    } else {
        handleWebsocketPayloadCb( true, NULL);
    }
}

void WebSockets::handleWebsocketPayloadCb( bool ok, uint8_t * payload) {

	
	if(ok) {
		if(header->payloadLen > 0) {
			payload[header->payloadLen] = 0x00;

			if(header->mask) {
				//decode XOR
				for(size_t i = 0; i < header->payloadLen; i++) {
					payload[i] = (payload[i] ^ header->maskKey[i % 4]);
				}
			}
		}

		switch(header->opCode) {
			case WSop_text:
			LOG_DEBUG_ARGS("[WS][handleWebsocket] text: %s\n", payload);
			// no break here!
			case WSop_binary:
			case WSop_continuation:
			//messageReceived( header->opCode, payload, header->payloadLen, header->fin);
			break;
			case WSop_ping:
			// send pong back
			sendFrame(WSop_pong, payload, header->payloadLen, true);
			break;
			case WSop_pong:
			LOG_DEBUG_ARGS("[WS][handleWebsocket] get pong (%s)\n", payload ? (const char*)payload : "");
			break;
			case WSop_close: {
				uint16_t reasonCode = 1000;
				if(header->payloadLen >= 2) {
					reasonCode = payload[0] << 8 | payload[1];
				}

				LOG_DEBUG_ARGS("[WS][handleWebsocket] get ask for close. Code: %d",  reasonCode);
				if(header->payloadLen > 2) {
					LOG_DEBUG_ARGS(" (%s)\n", (payload + 2));
					} else {
					LOG_DEBUG("\n");
				}

				if (!isDisconnecting)
				clientDisconnect( 1000);
			}
			break;
			default:
			clientDisconnect( 1002);
			break;
		}

		

		} else {
		LOG_DEBUG("[WS][handleWebsocket] missing data!\n");
		clientDisconnect( 1002);
	}
}

#endif


//----------------------------------------------------------------//
//----------------------------------------------------------------//
//-------------------------  SIM Methods  -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//

bool  WebSockets::GetSIMContact(uint8_t position, char *phone_number,char *contact_name)
{

	char * token;
	bool result=false;
	ProcessResults();
	// response in case valid phone number stored:
	//+CPBR: 1,"653316799",129,"Casa"
	// <CR><LF>+CPBR: <index>,<number>,<type>,<text><CR><LF>
	// <CR><LF>OK<CR><LF>

	// response in case there is not phone number:
	// <CR><LF>OK<CR><LF>

	if (SendCommandCheckError (F("AT+CPBR=%i"),F("+CPBR:"),(__FlashStringHelper *)ATSerial::AT_ERROR_CME_RESPONSE, position)==RX_CHECK_OK)
	{

		char * line= GetLastResponse();
		uint8_t contador=0;
		const char *splitchar=",";
		

		if (phone_number)
		{
			token=GetToken(line,1,splitchar);
			
			//le quitamos las comillas
			if (token)
			strncpy(phone_number,token+1,strlen(token)-2);
			
			if(contact_name)
			{
				token=GetNextToken(1,splitchar);
				if (token)
				strncpy(contact_name,token+1,strlen(token)-2);
			}
		}
		else if (contact_name)
		{
			token=GetToken(line,3,splitchar);
			if (token)
			strncpy(contact_name,token+1,strlen(token)-2);
		}

		if (phone_number)
		LOG_DEBUG_ARGS("PHONE:%s",phone_number);

		if (contact_name)
		LOG_DEBUG_ARGS("NAME:%s",contact_name);

		result=true;
	}
	else
	LOG_DEBUG("NO respuesta");
	

	return result;

}

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//-------------------------  IP Methods  -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//

bool WebSockets::IsIP( char * ip)
{
	//192.168.1.1
	if (ip!=NULL)
	{

		
		uint8_t tokens=0;
		uint8_t totallen=0;
		char * result= GetToken(ip,0,".");
		do
		{
			if (totallen>0)
			{
				ip[totallen]='.';
				totallen++;
			}
			

			//LOG_DEBUG_ARGS("Token %s ",result);
			totallen+=strlen(result);
			int octet= atoi(result);
			
			
			if (octet>0 && octet<=255)
			tokens++;
			
		} while ((result=GetNextToken(0,"."))!=NULL);

		//LOG_DEBUG_ARGS("Token %s, %i", ip,tokens);
		return (tokens==4);
	}
	else
	return false;
	
}


char * WebSockets::GetIp(const char * host)
{
	if (host!=NULL || strlen(host)>3)
	{

		
		if (IsIP(host))
		return host;

		if (SendCommandCheck(F("AT+DNS=\"%s\""),F("+DNS:"),host)==RX_CHECK_OK)
		{
			char *buffer=GetLastResponse();
			if (ExpectedResult(buffer,F("+DNS:Error"))==RX_CHECK_OK)
			return NULL;
			else
			{
				LOG_DEBUG_ARGS_B("IP=%s", buffer+5);
				return buffer+5;
			}
		}
		else
		return NULL;
	}
	else
	return NULL;

}



//----------------------------------------------------------------//
//----------------------------------------------------------------//
//-------------------------  FTP Methods  -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
#ifdef FTP


/**
* Login into ftp 
* ip:   IP address - numeric format 0.0.0.0
* port: port
* user: username 
* pwd:  password
* @return true if ok otherwise false
*/
bool WebSockets::FtpLogin(const char * ip,uint8_t port,const char * user,const char *pwd)
{
	//AT+FTPLOGIN=<ip>,<port>,<user>,<pwd>

	uint8_t result=RX_NO_DATA;
	 //Lo envio con el sendrawdata para 'ahorrar ese tamaño en el buffer interno
	 //ya que si user && pwd es largo(por ej. anonymous) se hace corto
	SendRawData(F("AT+FTPLOGIN="));
	SendRawData(ip);
	if (SendCommandCheckOK(F(",%i,%s,%s"),port,user,pwd)==RX_CHECK_OK)
	{

		WaitResponseResult(F("+FTPLOGIN:"),NULL,10000);
		char *result=GetLastResponse();

		//+FTPLOGIN:User logged in
		//+FTPLOGIN:Have Logged In
     	 if (ExpectedResult(result,F("+FTPLOGIN:User"))==RX_CHECK_OK || ExpectedResult(result,F("+FTPLOGIN:Have"))==RX_CHECK_OK)
			return true;
		
		//else if (ExpectedResult(result,F("+FTPLOGIN:AT Busy")))
		//return false;
		//else if (ExpectedResult(result,F("+FTPLOGIN: 530"))
		//return false;
		//else if (ExpectedResult(result,F("+FTPLOGIN:Error")))
		//return false;
	}
	return false;
}



/**
* Check ftp is connected
* @return true if ok otherwise false
*/
bool WebSockets::FtpIsConnected()
{
	//AT+FTPSTATUS
	//return (SendCommandCheckError(F("AT+FTPSTATUS"),F("+FTPSTATUS:login"),F("+FTPSTATUS:logout"))==RX_CHECK_OK);
	SendCommandAsync(F("AT+FTPSTATUS"));
	if ( WaitResponseResult(F("+FTPSTATUS:"),F("ERROR"),1000 ) ==RX_CHECK_OK)
	{
		
		char * result=GetLastResponse();
		
		return (ExpectedResult(result,F("+FTPSTATUS:login")) ==RX_CHECK_OK);
	}
	else
		return false;
	

}



/**
* Logout FTP
* @return true if ok otherwise false
*/
bool WebSockets::FtpLogout()
{
	//AT+FTPLOGOUT
	setTimeout(8000);
	bool result= (SendCommandCheckOK(F("AT+FTPLOGOUT"))==RX_CHECK_OK);

	setTimeout(AT_DEFAULT_TIMEOUT);
}

bool WebSockets::FtpPut(const char * file,uint8_t type,uint8_t mode,uint16_t length, void (*funcion)())
{
	//AT+FTPPUT=<filename>,<type>,<mode>,<size>
	//<filename>?file name of need-to-send file.
	//<type>?file transimission type:
	//-1?ASCII
	//-2?Binary
	//<mode>?operation mode:
	//-1?STOR mode. Establish a document on server to write into data, if a document
	//has been already exist,it will be replaced by the new built one.
	//-2?APPE mode. Establish a document to write into data, if a document has been
	//already exist, the data will be attached at the end of the file.
	//-3?DELE mode. Delete a file, at this time, there need to set parameter ‘size’ to be 0,
	//then input 0x0d after the appearance of ‘>’.
	//<size> : the length of data, the maximum data can’t exceed 10240.

	

	//Lo envio con el sendrawdata para 'ahorrar ese tamaño en el buffer interno
	//ya que si user && pwd es largo(por ej. anonymous) se hace corto

	
	SendRawData(F("AT+FTPPUT="));
	SendRawData(file);
	SendRawData(F(",%i,%i,%i\r\n"),type,mode,length);
	//SendRawData("AT+FTPPUT=/upload.txt,1,1,20\r\n");

	if (CheckInputMode())
	{


		if (funcion!=NULL)
		funcion();

		SendRawData(EXIT_INPUTMODE);

		uint8_t result=WaitResponseResult(F("+FTPPUT:"),(__FlashStringHelper *)AT_ERROR_RESPONSE,8000);

		if (result==RX_CHECK_OK)
		{
			
			char response=GetLastResponse();
			response+=8;
			//Si esta en modo Delete
			if (mode==3)
				result=ExpectedResult(response,F("Delete"));		
			else
				result=ExpectedResult(response,(__FlashStringHelper *)AT_ERROR_RESPONSE);
		}

		//Check resultados
		if (result==RX_CHECK_OK)
			return AT_RESULT_OK;
		else if (result==RX_ERROR)
			return AT_ERROR_FTP;
		else
			return AT_ERROR_UNKNOW;
	}
	else
	{
		char * result=GetLastResponse();
		if (ExpectedResult(result,F("+FTPPUT:"))==RX_CHECK_OK)
		{
			result+=8;
			if  (ExpectedResult(result,F("AT Busy"))==RX_CHECK_OK)
			return AT_ERROR_FTP_BUSY;
			else if (ExpectedResult(result,F("Error Not Login"))==RX_CHECK_OK)
			return AT_ERROR_FTP_NO_LOGIN;
			else if  (ExpectedResult(result,(__FlashStringHelper *)AT_ERROR_RESPONSE)==RX_CHECK_OK)
			return AT_ERROR_FTP;

			else if  (ExpectedResult(result,F("length overflow"))==RX_CHECK_OK)
			return AT_ERROR_FTP_OVERFLOW;
			else
			return AT_ERROR_UNKNOW;
		}
		else
			return AT_ERROR_UNKNOW;
	}

	return false;


}


bool WebSockets::FtpGet(const char * file,int (*tcpreadcallback) (char * ,int ),uint8_t type)
{
	//<Dir&filename>
	//File direct and name. (file direct is compared with FTP root direct.?
	//<Type> data transfer mode:
	//1?ASCII?
	//2?Binary?
	//<Content or Info> is to specify what you need is content of the file or information of
	//the file( file direct):
	//1?get the file content
	//2?get the information of file or specified direct

	setTimeout(15000);
	//Lo envio con el sendrawdata para 'ahorrar ese tamaño en el buffer interno
	//ya que si user && pwd es largo(por ej. anonymous) se hace corto
	SendRawData(F("AT+FTPGET="));
	SendRawData(file);
	//AT+FTPGET=<dir&filename>,<type>,<Content or Info>
	uint8_t result=SendCommandCheckError(F(",%i,1"),F("+FTPGET:"),F("ERROR"),type);
	setTimeout(AT_DEFAULT_TIMEOUT);
	if (result==RX_CHECK_OK)
	{
		char *result=GetLastResponse();
		if (ExpectedResult(result,F("+FTPGET:"))==RX_CHECK_OK)
		{

			if (ExpectedResult(result,F("+FTPLOGIN:Error"))==RX_CHECK_OK)
				return AT_ERROR_FTP_NO_LOGIN;


			result+=8;
			if (ExpectedResult(result,F("AT Busy"))==RX_CHECK_OK)
				return AT_ERROR_FTP_BUSY;
			else if (ExpectedResult(result,F("Error"))==RX_CHECK_OK)
				return AT_ERROR_FTP;
			else if (FTPReceiveData(tcpreadcallback)!=RX_NO_DATA)
				return (WaitResponseResult(F("+FTPGET:OK"),(__FlashStringHelper *)AT_ERROR_RESPONSE,10000)==RX_CHECK_OK);
			else
				return false;

		}
		else
			return false;

	}
	else
	return false;
}


bool WebSockets::FtpDel(const char * file)
{
	//AT+FTPLOGIN=<ip>,<port>,<user>,<pwd>
	return FtpPut(file,1,3,0,NULL);
}


uint8_t  WebSockets::FTPReceiveData(int (*ftpreadcallback) (char * ,int ))
{
	return _ReceiveData(8,ftpreadcallback);
}
#endif

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//------------------------- Http Methods -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
#ifdef HTTP

bool WebSockets::HttpPutRequest(char * host,char * path,uint8_t msglength,void (*tcpwritecallback)(), int (*tcpreadcallback) (char * first,int length))
{

//#define  WWW_PUT_CONTENTLEN_FIXED 195 //A esto hay que sumarle lo que enviemos msglength
#define  WWW_PUT_LEN_FIXED  70 //A esto hay que sumar len(path) + len(host) + len(str(contentlength)) + msglength




//Obtenemos la ip desde el host
if (TCPOpenHost(host,80))
{
 
	 //
	 //PUT /put?var=value HTTP/1.1
	 //Host: postman-echo.com
	 //Content-type: text/html
	 //Content-length: 16
	 //
	 //<p>New File</p>

	//Le sumamos la cte del resto de ferralla
	uint16_t contentlength= msglength;
	uint16_t totallength = WWW_PUT_LEN_FIXED + strlen(host) + strlen(path) +msglength  ;
	//Tenemos que sumarle la length del content length
	memset(buffinternal,0,WS_BUFFER_SIZE);
	itoa(contentlength, buffinternal, 10);
	totallength+= strlen(buffinternal);
	
	SendCommandAsync((__FlashStringHelper *)WebSockets::AT_TCPSEND_COMMAND,totallength);
	if (CheckInputMode())
	{

			SendCommandAsync(F("PUT %s HTTP/1.1"),path);//PUT %s HTTP/1.1
			SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_HOST,host);//Host: host
			SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_CONTENT_TYPE_TEXT);
			//Content-type: text/html
			SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_CONTENT_LENGTH,contentlength);//Content-lenght: %i
			SendRawData((__FlashStringHelper *)AT_ENDLINE);


			//Llamamos a la funcion que escribira el contenido
			tcpwritecallback();
			SendRawData((__FlashStringHelper *)AT_ENDLINE);

	
			//Si no mandaba este no habia su tia , no se el motivo
			SendRawData(EXIT_INPUTMODE);

			//Recepcion y cierre de conexion
			if (TCPReceiveData(tcpreadcallback)!=RX_NO_DATA)
			{
				////Si leimos todo correctamente se cierra el link
				////+TCPCLOSE:1.
				return TCPCloseWait();
			}
			else
			{
				Log.Error(MSG_ERR_RESPOND);
				TCPClose();
				
			}
	}
	else
	{
		Log.Error(MSG_ERR_INPUT);
		TCPClose();
	}
}
else
	Log.Error(MSG_ERR_CONNECT);


			

	return false;

}

bool WebSockets::HttpPostFileResquest(char * host,char * path,uint8_t msglength,void (*tcpwritecallback)(), int (*tcpreadcallback) (char * first,int length))
{
#define  WWW_POST_CONTENTLEN_FIXED 195 //A esto hay que sumarle lo que enviemos msglength
#define  WWW_POST_LEN_FIXED  327 //A esto hay que sumar len(path) + len(host) + len(str(contentlength)) + msglength


//Obtenemos la ip desde el host
 

if (TCPOpenHost(host,80))
{
	//=====PETICION POST =========
	//POST  HTTP/1.1
	//Host:
	//Content-Type: multipart/form-data; boundary=---------------------------114782935826962
	//Content-Length:
	//
	//-----------------------------114782935826962
	//Content-Disposition: form-data; name="submitted"; filename="file.txt"
	//Content-Type: text/plain
	//
	//
	//-----------------------------114782935826962--

	//Le sumamos la cte del resto de ferralla
	uint16_t contentlength= WWW_POST_CONTENTLEN_FIXED + msglength;
	uint16_t totallength = WWW_POST_LEN_FIXED + strlen(host) + strlen(path) +msglength  ;
	//Tenemos que sumarle la length del content length
	memset(buffinternal,0,WS_BUFFER_SIZE);
	itoa(contentlength, buffinternal, 10);
	totallength+= strlen(buffinternal);
	
	
	SendCommandAsync((__FlashStringHelper *)WebSockets::AT_TCPSEND_COMMAND,totallength);
	if (CheckInputMode())
	{

			SendCommandAsync(F("POST %s HTTP/1.1"),path);
			SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_HOST,host);
			SendRawData(F("Content-Type: multipart/form-data; boundary="));
			SendCommandAsync((__FlashStringHelper *)HTTP_BOUNDARY);
			SendCommandAsync(F("Content-Length: %i"),contentlength);
			SendRawData((__FlashStringHelper *)AT_ENDLINE);

			SendRawData((__FlashStringHelper *)HTTP_TICKS);
			SendCommandAsync((__FlashStringHelper *)HTTP_BOUNDARY);
			//-----------------------------114782935826962

			//troceamos para que quepa
			SendRawData(F("Content-Disposition: form-data;"));
			SendCommandAsync(F(" name=\"submitted\"; filename=\"file.txt\""));
			SendCommandAsync(F("Content-Type: text/plain"));
			SendRawData((__FlashStringHelper *)AT_ENDLINE);

			//Llamamos a la funcion que escribira el contenido
			tcpwritecallback();

			//Fin
			SendRawData((__FlashStringHelper *)AT_ENDLINE);
			SendRawData((__FlashStringHelper *)HTTP_TICKS);
			SendRawData((__FlashStringHelper *)HTTP_BOUNDARY);
			SendRawData((__FlashStringHelper *)HTTP_TICKS);
			SendRawData((__FlashStringHelper *)AT_ENDLINE);
			//-----------------------------114782935826962--

			//Si no mandaba este no habia su tia , no se el motivo
			SendRawData(EXIT_INPUTMODE);

			//Recepcion y cierre de conexion
			if (TCPReceiveData(tcpreadcallback)!=RX_NO_DATA)
			{
				////Si leimos todo correctamente se cierra el link
				////+TCPCLOSE:1.
				return TCPCloseWait();
			}
			else
			{
				LOG_ERROR_B(MSG_ERR_RESPOND);
				TCPClose();
			}
	}
	else
	{
		Log.Error(MSG_ERR_INPUT);
		TCPClose();
	}
}
else
	LOG_ERROR_B(MSG_ERR_CONNECT);

	return false;
}

bool WebSockets::HttpGetRequest(char * host,char * path,int (*tcpreadcallback) (char * first,int length))
{
#define  WWW_GET_LEN_FIXED  25 //A esto hay que sumar len(path) + len(host) + len(str(contentlength)) + msglength

//Obtenemos la ip desde el host

if (TCPOpenHost(host,80))
{
	uint8_t length=WWW_GET_LEN_FIXED+strlen(host) + strlen(path);

	SendCommandAsync((__FlashStringHelper *)WebSockets::AT_TCPSEND_COMMAND,length);
	if (CheckInputMode())
	{

		SendCommandAsync(F("GET %s HTTP/1.1"),path);
		SendCommandAsync((__FlashStringHelper *)HTTP_HEADER_HOST,host);
		SendRawData((__FlashStringHelper *)AT_ENDLINE);

		SendRawData(EXIT_INPUTMODE);
	 
		if (TCPReceiveData(tcpreadcallback)!=RX_NO_DATA)
		{
			////Si leimos todo correctamente se cierra el link
			////+TCPCLOSE:1.
			return TCPCloseWait();
		}
		else
		{
			LOG_ERROR_B(MSG_ERR_RESPOND);
			TCPClose();
		}
	}
	else
	{
		LOG_ERROR_B(MSG_ERR_INPUT);
		TCPClose();
	}
}
else
	LOG_ERROR_B(MSG_ERR_CONNECT);



return false;
}

#endif 

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//-------------------------  Sms Methods -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//


bool WebSockets::SendSMS(uint8_t indexSIM,const __FlashStringHelper * msg)
{
	memset(buffinternal,0,WS_BUFFER_SIZE);
	if (GetSIMContact(indexSIM,buffinternal,NULL))
	{
		return _SendSMS(buffinternal,msg,NULL);
	}
	else
		return false;
}


bool WebSockets::SendSMS(char *phone,void  (*msgfunc)())
{
	return _SendSMS(phone,NULL,msgfunc);
}


bool WebSockets::SendSMS(char *phone,const __FlashStringHelper * msg)
{
	return _SendSMS(phone, msg,NULL);
}


bool WebSockets::_SendSMS(char *phone,const __FlashStringHelper* msg,void  (*msgfunc)())
{
if (CHECK_STRING(phone))
{
	if (SendCommandCheckOK(F("AT+CSCS=\"GSM\""))==RX_CHECK_OK)
	{

		if (SendCommandCheckOK(F("AT+CMGF=1"))==RX_CHECK_OK)
		{
			
			SendCommandAsync(F("AT+CMGS=\"%s\""),phone);

			if (CheckInputMode())    //Number to which you want to send the sms
			{
				if (msg!=NULL)
				SendRawData(msg);

				if (msgfunc!=NULL)
				msgfunc();

				ProcessResults(100);
				SendRawData(0x1A);  // command for send sms, code for CTRL-Z
				uint8_t result=WaitResponseResult(F("+CMGS:"),(__FlashStringHelper*)ATSerial::AT_ERROR_RESPONSE,15000);
				return  (result==RX_CHECK_OK);
			}
		}
	}
}

return false;
}


//----------------------------------------------------------------//
//----------------------------------------------------------------//
//-------------------------  TCP Methods -------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//

int WebSockets::TCPConfig(const char * apn,const char * username,const char * password)
{

	if (SendCommandCheckOK(F("AT+XISP=0"))==RX_CHECK_OK)
	{
		if (SendCommandCheckOK(F("AT+CGDCONT=1,\"IP\",\"%s\""),apn)==RX_CHECK_OK)
		{
			if (SendCommandCheckOK(F("AT+XGAUTH=1,0,\"%s\",\"%s\""),(username?username:"\0"),(password?password:"\0"))==RX_CHECK_OK)
			{

				if (SendCommandCheckOK(F("AT+XIIC=1"))==RX_CHECK_OK)
				{
					//Esperamos 1/2 seg
					delay(500);
					if (IsTCPConfig())
					{
						return AT_RESULT_OK;
					}
				}
			}
		}
		
	}
	

	return AT_ERROR_UNKNOW;
	

	
}

bool WebSockets::IsTCPConfig()
{
	
	return (SendCommandCheckError(F("AT+XIIC?"),F("+XIIC:    1"),F("+XIIC:    1, 0.0.0.0"))==RX_CHECK_OK);
}

bool WebSockets::IsTCPOpen()
{
	return (SendCommandCheckError(F("AT+IPSTATUS=1"),F("+IPSTATUS:1,CONNECT"),F("+IPSTATUS:1,DISCONNECT"))==RX_CHECK_OK);
}

bool WebSockets::TCPOpenHost(char* host,uint8_t port)
{


	if (SendCommandCheck(F("AT+DNS=\"%s\""),F("+DNS:"),host)==RX_CHECK_OK)
	{
		char *buffer=GetLastResponse();
		LOG_DEBUG_ARGS_B("IP=%s", buffer+5);
		memset(buffinternal,0,WS_BUFFER_SIZE);
		strcpy(buffinternal,buffer+5);
		return TCPOpenIP(buffinternal,port);
		
	}
	else
	return false;
}

bool WebSockets::TCPOpenIP(char* ip, uint8_t port)
{


	if (IsTCPOpen())
		TCPClose();

	SendRawData(F("AT+TCPSETUP=1"));
	if (SendCommandCheckOK(F(",%s,%i"),ip,port)==RX_CHECK_OK)
	{
		return (WaitResponseResult(F("+TCPSETUP:1,OK"),F("+TCPSETUP:1,FAIL"),5000)==RX_CHECK_OK);
	}
	else
		return false;
	

}

bool WebSockets::TCPClose()
{
	return (SendCommandCheck(F("AT+TCPCLOSE=1"),(__FlashStringHelper*) AT_TCPCLOSE_RESPONSE )==RX_CHECK_OK);
}

bool WebSockets::TCPCloseWait()
{

	if (WaitResponseResult((__FlashStringHelper*) AT_TCPCLOSE_RESPONSE ,(__FlashStringHelper*) AT_ERROR_RESPONSE,1000)==RX_CHECK_OK)
	return true;
	else if(TCPClose())
	return true;
	else
	return false;
}

uint8_t  WebSockets::TCPReceiveData(int (*tcpreadcallback) (char * first,int length))
{
	if (WaitResponseResult(F("+TCPRECV:1,"),(const __FlashStringHelper *)NULL,15000)==RX_CHECK_OK)
	{
		return _ReceiveData (11,tcpreadcallback);
	}
	else
	return RX_NO_DATA;
}
