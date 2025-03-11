#ifndef _DEBUG_H
#define _DEBUG_H

#include "stm32f10x.h"
#include "esp8266.h"
#include "stdio.h"

/* UARTÊä³ö */

// FOR DEBUG
#define _DEBUG_LOG_UART(tag,msg) \
do{	\
	printf("[DEBUG]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
}while(0)

// FOR WARN
#define _WARN_LOG_UART(tag,msg) \
do{	\
	printf("[WARN]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
}while(0)

// FOR FATAL
#define _FATAL_LOG_UART(tag,msg) \
do{	\
	printf("[FATAL]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
}while(0)

/* ÍøÂçÊä³ö */

#define LEN_NETLOG_BUF	128
extern uint8_t netlogBuf[LEN_NETLOG_BUF];

// FOR DEBUG
#define _DEBUG_LOG_NET(tag,msg) \
do{	\
	snprintf((char*)netlogBuf,LEN_NETLOG_BUF-1,"[DEBUG]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
	ESP8266_SendData(netlogBuf,strlen((char*)netlogBuf));\
}while(0)

// FOR WARN
#define _WARN_LOG_NET(tag,msg) \
do{	\
	snprintf((char*)netlogBuf,LEN_NETLOG_BUF-1,"[WARN]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
	ESP8266_SendData(netlogBuf,strlen(char*)netlogBuf));\
}while(0)

// FOR FATAL
#define _FATAL_LOG_NET(tag,msg) \
do{	\
	snprintf((char*)netlogBuf,LEN_NETLOG_BUF-1,"[FATAL]%s:%s\r\n",tag?tag:"Null",msg?msg:"Null");\
	ESP8266_SendData(netlogBuf,strlen((char*)netlogBuf));\
}while(0)

#endif
