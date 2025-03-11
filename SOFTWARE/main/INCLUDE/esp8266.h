#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"wiselot\",\"IOTESPOK356\"\r\n"

#define ESP8266_TCP_INFO		"AT+CIPSTART=\"TCP\",\"192.168.69.62\",8081\r\n"

#define REV_WAIT 	1
#define REV_OK 		0

#define CMD_TRY_TIMES		3

void ESP8266_Clear(void);
_Bool ESP8266_WaitRecive(void);
_Bool ESP8266_SendCmd(char *cmd, char *res);
_Bool ESP8266_Init(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);



#endif