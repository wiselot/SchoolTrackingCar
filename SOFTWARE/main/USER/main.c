#include "stm32f10x.h"

#include "uart.h"
#include "time.h"
#include "tim.h"
#include "motor.h"
#include "mg996r.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "test.h"
#include "esp8266.h"
#include "debug.h"
#include "ctl.h"
#include "encoder.h"
#include "74HC165.h"
#include "binay.h"

const char * _TAG = "MAIN";
// 0 wifi直接跳过初始化，省时间,1 初始化忽略报错,2 初始化报错退出
#define WIFI_INIT_LEVEL	0

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(72);
	
	UART1_Init(115200);
	UART3_Init(115200);
	
	_DEBUG_LOG_UART(_TAG,"Hardware Init...\r\n");
	
	// LED
	LED_Init();
	_DEBUG_LOG_UART(_TAG,"LED Done.\r\n");
	
	// BUZZER
	//Buzzer_Init();
	//_DEBUG_LOG_UART(_TAG,"BUZZER Done.\r\n");
	
	// KEY
#ifdef MG996R_TEST
	Key_Init(mg996r_foward10,mg996r_back10);
#elif defined MOTOR_TEST
	Key_Init(motor_test_forward,motor_test_back);
#elif defined ADC_TEST
	Key_Init(adc_test_start,adc_test_end);
#else
	Key_Init(NULL,NULL);
#endif
	_DEBUG_LOG_UART(_TAG,"KEY Done.\r\n");
	
	// CTL
	Ctl_Init();
	_DEBUG_LOG_UART(_TAG,"CTL Done.\r\n");
	// 74HC165
	_74HC165_Init();
	_DEBUG_LOG_UART(_TAG,"74HC165 Done.\r\n");
	
	// MG996R
	mg996r_Init();
	_DEBUG_LOG_UART(_TAG,"MG996R Done.\r\n");
	
	// MG310
	allMotorInit();
	_DEBUG_LOG_UART(_TAG,"MG310 Done.\r\n");
	
	//Encoder
	Encoder_Init();
	_DEBUG_LOG_UART(_TAG,"Encoder Done.\r\n");
	
	// ESP8266
	/* 呃，这个有点问题，连不上的话把连接断开再复位就可以了，当然手机热点先开一下 */
	if(WIFI_INIT_LEVEL){
		if(!ESP8266_Init())
		{
			_DEBUG_LOG_UART(_TAG,"ESP8266 Done.\r\n");
			_DEBUG_LOG_NET(_TAG,"o(*￣▽￣*)o ESP CONNECT");
#ifdef ADC_VAL_TEST
			adc_val_test_config(1);
#endif
		}
		else{
			if(WIFI_INIT_LEVEL==2)
			{
				_FATAL_LOG_UART(_TAG,"ESP8266 Failed.\r\n");
				while(1);
			}
			else{
				_WARN_LOG_UART(_TAG,"ESP8266 Failed.\r\n");
			}
#ifdef ADC_VAL_TEST
			adc_val_test_config(0);
#endif
		}
	}
	
	SET_ANGLE(45);
	delay_ms(100);
	
#if 0
	
	while(1)
	{
		uint8_t dat = Read_74HC165();
		printf("%d\n",dat);
		//ESP8266_SendData(buf,strlen((char*)buf));
		
		delay_ms(100);
	}
	
#endif
	//TraceFlowCtlLoop();
	while(1) delay_ms(10);

	return 0;
}
