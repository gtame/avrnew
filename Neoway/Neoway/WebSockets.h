/**
 * @file WebSockets.h
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

#ifndef WEBSOCKETS_H_
#define WEBSOCKETS_H_

 
#include <Arduino.h>
#include <Logger.h>
#include "ATSerial.h"
 

 #define  CHECK_STRING(x) (x!=NULL && strlen(x)>0)
 #define  CHECK_STRING_P(x) (x!=NULL && strlen_P(x)>0)
 #define WS_BUFFER_SIZE 25

//atmega328p has only 2KB ram!
#define WEBSOCKETS_MAX_DATA_SIZE  (128)
// moves all Header strings to Flash

#define WEBSOCKETS_TCP_TIMEOUT    (2000)


// max size of the WS Message Header
#define WEBSOCKETS_MAX_HEADER_SIZE  (14)
#define  WS_BUFFER_PAYLOAD (WS_BUFFER_SIZE-WEBSOCKETS_MAX_HEADER_SIZE)

#ifdef WSOCKETS
typedef enum {
    WSC_NOT_CONNECTED,
    WSC_HEADER,
    WSC_CONNECTED
} WSclientsStatus_t;

typedef enum {
    WStype_ERROR,
    WStype_DISCONNECTED,
    WStype_CONNECTED,
    WStype_TEXT,
    WStype_BIN,
	WStype_FRAGMENT_TEXT_START,
	WStype_FRAGMENT_BIN_START,
	WStype_FRAGMENT,
	WStype_FRAGMENT_FIN,
} WStype_t;

typedef enum {
    WSop_continuation = 0x00, ///< %x0 denotes a continuation frame
    WSop_text = 0x01,         ///< %x1 denotes a text frame
    WSop_binary = 0x02,       ///< %x2 denotes a binary frame
                              ///< %x3-7 are reserved for further non-control frames
    WSop_close = 0x08,        ///< %x8 denotes a connection close
    WSop_ping = 0x09,         ///< %x9 denotes a ping
    WSop_pong = 0x0A          ///< %xA denotes a pong
                              ///< %xB-F are reserved for further control frames
} WSopcode_t;

typedef struct {

        bool fin;
        bool rsv1;
        bool rsv2;
        bool rsv3;

        WSopcode_t opCode;
        bool mask;

        size_t payloadLen;

        uint8_t * maskKey;
} WSMessageHeader_t;

#endif

#define AT_ERROR_INIT -1000
#define AT_ERROR_UNKNOW  AT_ERROR_INIT+1 //ERROR
#define AT_ERROR_CME  AT_ERROR_INIT+2 //ERROR CME
#define AT_NO_ARRANCADO AT_ERROR_INIT+3
#define AT_NO_REGISTRADO AT_ERROR_INIT+4
#define AT_NO_APAGADO AT_ERROR_INIT+5
#define AT_NO_READY AT_ERROR_INIT+6
#define AT_NO_SIMREADY AT_ERROR_INIT+7
#define AT_NO_FOUND_CONTACT AT_ERROR_INIT+8
#define AT_SIM_PIN AT_ERROR_INIT+9
#define AT_SIM_PUK  AT_ERROR_INIT+10
#define  AT_ERROR_FTP AT_ERROR_INIT+ 11
#define  AT_ERROR_FTP_BUSY AT_ERROR_INIT+12
#define  AT_ERROR_FTP_NO_LOGIN AT_ERROR_INIT+13
#define  AT_ERROR_FTP_OVERFLOW AT_ERROR_INIT+14


#define AT_RESULT_OK true




class WebSockets: public  ATSerial {

	public:
		WebSockets(Stream *stream);

		//Funciones WebSockets
#ifdef WSOCKETS
		bool clientConnect(char * host,char * urlpath);
		bool clientIsConnected();
		void clientDisconnect( uint16_t code, char * reason, size_t reasonLen);
		void clientDisconnect( uint16_t code);
		bool sendTXT(const char *txt);//uint8_t * payload, size_t length, bool headerToPayload);
#endif


		//Funciones Ip
		char* GetIp( const char * host);
		bool IsIP( char * ip);

		//Funciones TCP
		bool TCPOpenHost(char* host,uint8_t port);
		bool TCPOpenIP(char* IP, uint8_t port);
		bool IsTCPConfig();
		bool IsTCPOpen();
		bool TCPClose();
		bool TCPCloseWait();
		int TCPConfig(const char * apn,const char * username,const char * password);
		uint8_t  TCPReceiveData(int (*tcpreadcallback) (char * ,int ));

		uint8_t  _ReceiveData( uint8_t length, int (*tcpreadcallback) (char * ,int ));


		//Funciones SMS
		bool SendSMS(char *phone,const __FlashStringHelper * msg);
		bool SendSMS(uint8_t simIndex,const __FlashStringHelper * msg);
		bool SendSMS(char *phone, void (*msg)());
		bool _SendSMS(char *phone,const __FlashStringHelper*  msg,void  (*msgfunc)());
		//Obtener sms desde la memoria


		//Funcion SIM
		bool GetSIMContact(uint8_t position, char *phone_number,char *contact_name);



#ifdef FTP
		//Ftp Functions
		bool FtpIsConnected();
		bool FtpLogout();
		bool FtpLogin(const char * ip,uint8_t port,const char * user,const char *pwd);
		bool FtpPut(const char * file,uint8_t type,uint8_t mode,uint16_t length, void (*funcion)());
		bool FtpGet(const char * file,int (*tcpreadcallback) (char * ,int ),uint8_t mode=1);
		bool FtpDel(const char * file);
		uint8_t  FTPReceiveData(int (*ftpreadcallback) (char * ,int ));
#endif

#ifdef HTTP
		//Http Functions Get-Post 
		bool HttpPostFileResquest(char * host,char * path,uint8_t msglength,void (*tcpwritecallback)(),int (*tcpreadcallback) (char * ,int ));
		bool HttpGetRequest(char * host,char * path,int (*tcpreadcallback) (char * ,int ));
		bool HttpPutRequest(char * host,char * path,uint8_t msglength,void (*tcpwritecallback)(), int (*tcpreadcallback) (char * first,int length));

 #endif


		
    protected:
		
#ifdef WSOCKETS
		virtual bool OnProcessResult( char * ) override;
		bool sendFrame(WSopcode_t opcode, uint8_t * payload, size_t length);
		bool sendFrame(WSopcode_t opcode, uint8_t * payload, size_t length, bool mask);
		bool sendFrame( WSopcode_t opcode, uint8_t * payload, size_t length, bool mask, bool fin, bool headerToPayload) ;
		void handleWebsocketCb(uint8_t * buffer) ;
		void handleWebsocketPayloadCb( bool ok, uint8_t * payload);
		size_t write( const char *out) ;
		size_t write( uint8_t *out, size_t n);
#endif

	private:
#ifdef WSOCKETS
		WSMessageHeader_t *header;
		bool connectedWS=false;
		bool isDisconnecting=false;
#endif
	
	
	char buffinternal[WS_BUFFER_SIZE];

	static const  char HTTP_BOUNDARY[] PROGMEM;
	static const  char HTTP_TICKS[] PROGMEM;
	static const  char HTTP_HEADER_CONTENT_TYPE_TEXT[] PROGMEM;
	static const  char HTTP_HEADER_HOST[] PROGMEM;
	static const  char HTTP_HEADER_CONTENT_LENGTH[] PROGMEM;
	static const  char AT_TCPSEND_COMMAND[] PROGMEM;
	
	static const  char AT_TCPCLOSE_RESPONSE [] PROGMEM;


	static const  char MSG_INFO_CONNECT[] PROGMEM	;
	static const  char MSG_ERR_RESPOND[] PROGMEM	;
	static const  char MSG_ERR_INPUT[] PROGMEM	;
	static const  char MSG_ERR_CONNECT[] PROGMEM;


};

#endif /* WEBSOCKETS_H_ */
