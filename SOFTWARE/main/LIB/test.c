#include "test.h"
#include "stdio.h"
#include "debug.h"
#include "uart.h"

uint8_t netlogBuf[LEN_NETLOG_BUF] = {0};

#ifdef MG996R_TEST

#include "mg996r.h"

static uint8_t angle = 0;

void mg996r_foward10()
{
	angle+=10;
	if(angle>90) angle=0;
	SET_ANGLE(angle);
	printf("Foward 10");
}

void mg996r_back10()
{
	if(angle!=0){
		angle-=10;
		SET_ANGLE(angle);
		printf("Back 10");
	}
}

#endif

#ifdef MOTOR_TEST

#include "motor.h"

uint16_t pwm_value = 800;

void motor_test_forward()
{
	Motor_Forward(0,pwm_value);
	Motor_Forward(1,pwm_value);
	printf("Motor roll");
}

void motor_test_back()
{
	Motor_Stop(0);
	Motor_Stop(1);
	printf("Motor stop");
}

#endif

#ifdef ADC_TEST

void adc_test_start()
{
	adc_test_ = 1;
}

void adc_test_end()
{
	adc_test_ = 0;
}

#endif

#ifdef ADC_TEST

void adc_env_test()
{

	printf("Start ADC Test\n");

	uint16_t adc0,adc1,adc2;
	
	/*(1)确定左右传感器差值(D_AD_VALUE)：
确定方法：将传感器放到白色纸上，将左右两个传感器的数据通过串口发送到电脑上，获取左右两个传感器差值，D_AD_VALUE=Left_AD-Right_AD
	*/
	printf("Now press key0 to start D_AD_VALUE test\n");
	while(adc_test_!=1);
	printf("Testing...\n");
	uint32_t count = 0;
	int32_t tmp;
	while(adc_test_!=0)
	{
		adc0 = ADC_GetValue_Average(ADC_Channel_0,10);
		adc2 = ADC_GetValue_Average(ADC_Channel_2,10);
		tmp+=adc0-adc2;
		count++;
	}
	D_AD_VALUE = tmp/count;
	printf("D_AD_VALUE Test Done.\n");
	
	
	/* (2)确定左传感器峰值点(LEFT_MAX)和右传感器峰值点(RIGHT_MAX)：
确定方法：将左红外传感器和右红外传感器分别从左到右移动，将数据通过串口发送到电脑上观察数据获取最大值，一般当传感器移动到黑线中间是获得数据最大值。 */
	printf("Now press key0 to start LEFT_MAX & RIGHT_MAX test\n");
	while(adc_test_!=1);
	printf("Testing...\n");
	uint16_t left_max_tmp=0,right_max_tmp=0;
	while(adc_test_!=0)
	{
		adc0 = ADC_GetValue_Average(ADC_Channel_0,20);
		adc2 = ADC_GetValue_Average(ADC_Channel_2,20);
		if(adc0>left_max_tmp) left_max_tmp = adc0;
		if(adc2>right_max_tmp) right_max_tmp = adc2;
	}
	LEFT_MAX = left_max_tmp;
	RIGHT_MAX = right_max_tmp;
	printf("LEFT_MAX & RIGHT_MAX Test Done.\n");

	/* (3)确定左阈值点(LEFT_THREASH)和右阈值点(RIGHT_THREASH):
确定方法：当达到左传感器达到峰值时，观察中间红外传感器数值，此时中间红外传感器数值即为左阈值点。同样当达到右传感器达到峰值时，观察中间红外传感器数值，此时中间红外传感器数值即为右阈值点。 */
	printf("Now press key0 to start LEFT_THREASH & RIGHT_THREASH test\n");
	while(adc_test_!=1);
	printf("Testing...\n");
	
	while(adc_test_!=0)
	{
		adc0 = ADC_GetValue_Average(ADC_Channel_0,20);
		adc1 = ADC_GetValue_Average(ADC_Channel_1,20);
		adc2 = ADC_GetValue_Average(ADC_Channel_2,20);
		if(fabs((double)(adc0-LEFT_MAX))/LEFT_MAX<=0.1)
			LEFT_THREASH = adc1;
		if(fabs((double)(adc2-RIGHT_MAX))/RIGHT_MAX<=0.1)
			RIGHT_THREASH = adc1;
	}
	printf("LEFT_THREASH & RIGHT_THREASH Test Done.\n");
	
	
	printf("Test done.D_AD_VALUE=%d,LEFT_MAX=%d,RIGHT_MAX=%d,LEFT_THREASH=%d,RIGHT_THREASH=%d\n",\
										D_AD_VALUE,LEFT_MAX,RIGHT_MAX,LEFT_THREASH,RIGHT_THREASH);
										
}
#endif

#ifdef ADC_VAL_TEST

#include "adc.h"
#include "stm32f10x.h"
#include "debug.h"
#include "esp8266.h"
#include "string.h"

const char * TAG = "ADC_VAL_TEST";
static _Bool netDebug = 0;

void adc_val_test_config(_Bool isNet)
{
	netDebug = isNet;
}

void adc_val_test_loop(){
	uint16_t adc0,adc1,adc2;
	adc0 = ADC_GetValue_Average(ADC_Channel_0,20);
	adc1 = ADC_GetValue_Average(ADC_Channel_1,20);
	adc2 = ADC_GetValue_Average(ADC_Channel_2,20);
	
	char buf[64]={0};
	sprintf(buf,"#%d,%d,%d#",adc0,adc1,adc2);
	
	//uart
	UART1_SendString(buf);
	// net
	if(netDebug)
		ESP8266_SendData((uint8_t *)buf,strlen(buf));
	
}

#endif		
		
#ifdef BUZZER_TEST

	playTwinkle();

#endif		

#ifdef LED_TEST
		LED0_ON
		LED1_OFF
		delay_ms(500);
		LED0_OFF
		LED1_ON
		delay_ms(500);
	
#endif
		
#ifdef TRACE_TEST
			
		adc_left = ADC_GetValue_Average(ADC_Channel_0,3);
		adc_mid = ADC_GetValue_Average(ADC_Channel_1,3);
		adc_right = ADC_GetValue_Average(ADC_Channel_2,3);
		
		data_out=(adc_left-adc_right+D_AD_VALUE);
		if((data_out>0)&&(adc_mid<LEFT_THREASH))
		{	
			data_out=(2*LEFT_MAX-adc_left)*2;
		} 
		else if((data_out<0)&&(adc_mid<RIGHT_THREASH))
		{
			data_out=(adc_right-2*RIGHT_MAX)*2;
		}
		
		printf("data_out:%d\n",data_out);
		
		error = data_out;  // 当前误差

    integral += error;  // 误差累积
    derivative = error - last_error;  // 误差变化率

    // 计算PID输出
    int output = (int)(Kp * error + Ki * integral + Kd * derivative);

    // 更新上一次误差
    last_error = error;
		
		output= 45-output;
		if(output<0) output=0;
		if(output>90) output=90;
		
		printf("angle:%d\n",output);
		
		SET_ANGLE(output);
		
		
#endif

#ifdef WIFI_TEST

#include "uart.h"
#include "time.h"

#define ESP_OK 0

void wifi_test()
{
	delay_ms(1500);

	uint8_t esp_err;
		/*
	// AT Test
	esp_err = ESP8266_SendCommand("AT","OK",1000);
	if(esp_err!=ESP_OK)
		printf("Test AT Failed!");
	else
		printf("AT done\n");
	// Station mode
	esp_err = ESP8266_SendCommand("AT+CWMODE=1","OK",1000);
	if(esp_err!=ESP_OK)
		printf("Set station mode Failed!");
	else
		printf("CWMODE done\n");
	// RST
	esp_err = ESP8266_SendCommand("AT+RST","ready",1000);
	if(esp_err!=ESP_OK)
		printf("Reset Failed!");
	else
		printf("RST done\n");
	delay_ms(1500);
	// Connect wifi
	esp_err = ESP8266_SendCommand("AT+CWJAP_DEF=\"TP-LINK_ACF5\",\"13967187659\"","OK",5000);
	if(esp_err!=ESP_OK)
		printf("Connect wifi Failed!");
	else
		printf("CWJAP_DEF done\n");
	// 单路链接
	esp_err = ESP8266_SendCommand("AT+CIPMUX=0","OK",1000);
	if(esp_err!=ESP_OK)
		printf("Set CIPMUX Failed!");
	else
		printf("CIPMUX done\n");
	// tcp
	esp_err = ESP8266_SendCommand("AT+CIPSTART=\"TCP\",\"192.168.0.101\",8081","OK",5000);
	if(esp_err!=ESP_OK)
		printf("Connect server Failed!");
	else
		printf("CIPSTART done\n");
		*/
	// send test
	esp_err = ESP8266_SendCommand("AT+CIPSEND=5\r\n","OK",1000);
	if(esp_err!=ESP_OK)
		printf("Send test Failed!");
	else
		UART3_SendString("ESPOK");
	printf("ESP Conf done.");
}

#endif

#ifdef ENCODER_TEST
	
#endif