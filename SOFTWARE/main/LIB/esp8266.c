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

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		UART3_SendString((char *)data);		//�����豸������������
	}

}


unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	UART3_SendString((char *)cmd);
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //��ֹ���ڱ�ˢ��
		esp8266_buf[esp8266_cnt++] = USART3->DR;
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}