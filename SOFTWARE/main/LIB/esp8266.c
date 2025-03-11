#include "esp8266.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "uart.h"

uint8_t esp8266_buf[128];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;

void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		UART3_SendString((char *)data);		//发送设备连接请求数据
	}

}


unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}

_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	UART3_SendString((char *)cmd);
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

_Bool ESP8266_Init(void)
{
	ESP8266_Clear();
	
	uint8_t times;
	
	puts("1. AT\r\n");
	for(times=CMD_TRY_TIMES;times && ESP8266_SendCmd("AT\r\n", "OK");times--)
		delay_ms(500);
	if(!times) goto failed;
	
	puts("2. CWMODE\r\n");
	for(times=CMD_TRY_TIMES;times && ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK");times--)
		delay_ms(500);
	if(!times) goto failed;
	
	puts("3. CWJAP\r\n");
	for(times=CMD_TRY_TIMES;times && ESP8266_SendCmd(ESP8266_WIFI_INFO, "WIFI GOT IP");times--)
		delay_ms(5000);
	if(!times) goto failed;
	
	delay_ms(2000);
	
	puts("4. CIPSTART\r\n");
	for(times=CMD_TRY_TIMES;times && ESP8266_SendCmd(ESP8266_TCP_INFO, "OK");times--)
		delay_ms(500);
	if(!times) goto failed;
	
	puts("5. ESP8266 Init OK\r\n");
	return 0;
failed:
	puts("Failed!\r\n");
	return 1;
	
}

void USART3_IRQHandler(void) {
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收中断
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //防止串口被刷爆
		esp8266_buf[esp8266_cnt++] = USART3->DR;
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}