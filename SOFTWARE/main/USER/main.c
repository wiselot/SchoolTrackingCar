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
// 0 wifiֱ��������ʼ����ʡʱ��,1 ��ʼ�����Ա���,2 ��ʼ�������˳�
#define WIFI_INIT_LEVEL	0

static uint8_t STEP = 0; // ����
static uint8_t STEP_WH = 0; // �������״̬
#define STEP_WHOLE	5

/* ·����˳ʱ���һСȦ���ڶ���Ȧ
	step=0 Ԥ��
	step=1 ֱ��
*/

int DAT_2_ANGLE(uint8_t dat)
{
	int ax=0,bx=0;
	uint8_t ai=0,bi=0;
	if(GET_LINE_B0(dat)){
		ai++;
		ax+=16;
	}
	if(GET_LINE_B2(dat)){
		ai++;
		ax-=16;
	}
	if(GET_LINE_B1(dat)){
		ai++;
	}
	if(GET_LINE_A0(dat)){
		bx+=24;
		bi++;
	}
	if(GET_LINE_A1(dat)){
		bi++;
		bx+=12;
	}
	if(GET_LINE_A2(dat)){
		bi++;
	}
	if(GET_LINE_A3(dat)){
		bi++;
		bx-=12;
	}
	if(GET_LINE_A4(dat)){
		bi++;
		bx-=24;
	}
	int angle = 45 + tan((double)((float)(bx-bi)-(float)(ax/ai))/5);
	if(angle<0) angle = 0;
	if(angle>90) angle = 90;
	return angle;
}

// ���ֵ���pid�ص�(����pwm��1ms��ʱ)
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		}
}

void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			
			
		}
}

static void key0_start()
{
	STEP = 1;
}

static void key1_stop()
{
	STEP = 0;
}

// ��Ѳ�ߵ���
#define SINGLE_TRACE(pwm) \
do{ \
	int angle = DAT_2_ANGLE(dat); \
	float ratio = CAR_B * tan(angle-45) / CAR_L / 2; \
	SET_ANGLE(angle); \
	Motor_Forward(0,pwm - ratio*pwm); \
	Motor_Forward(1,pwm + ratio*pwm); \
}while(0)

// ������ͣ�����˹���
#define STOP_BACK_LINE_TRACE(stop_ms,back_ms,back_pwm,forward_pwm,left) \
do{ \
	Motor_Stop(0); \
	Motor_Stop(1); \
	delay_ms(stop_ms); \
	SET_ANGLE(left?0:90); \
	Motor_Backward(!left,back_pwm); \
	delay_ms(back_ms); \
	Motor_Forward(0,forward_pwm); \
	Motor_Stop(1); \
}while(0)

// �������
#define GO_LINE_TRACE(forward_pwm) \
do{ \
	if(GET_BUS_B(dat)!=0){ \
		Motor_Forward(0,forward_pwm); \
		Motor_Forward(1,forward_pwm); \
		if(GET_BUS_B(dat) == B010) \
				break; \
		if(GET_LINE_B0(dat)==1){ \
				SET_ANGLE(90); \
		} \
		else if(GET_LINE_B2(dat)==1){ \
				SET_ANGLE(0); \
		} \
		else{ \
		} \
	} \
}while(0)	

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
	
	/*
	// ADC
	Board_ADC_Init();
	_DEBUG_LOG_UART(_TAG,"ADC Done.\r\n");
	*/
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
	/* ��������е����⣬�����ϵĻ������ӶϿ��ٸ�λ�Ϳ����ˣ���Ȼ�ֻ��ȵ��ȿ�һ�� */
	if(WIFI_INIT_LEVEL){
		if(!ESP8266_Init())
		{
			_DEBUG_LOG_UART(_TAG,"ESP8266 Done.\r\n");
			_DEBUG_LOG_NET(_TAG,"o(*������*)o ESP CONNECT");
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
	
	uint8_t buf[256] = {0};
	
#if 1
	
	while(1)
	{
		uint8_t dat = Read_74HC165();
		snprintf((char *)buf,255,"A:%d,%d,%d,%d,%d    B:%d,%d,%d\r\n",GET_LINE_A0(dat),GET_LINE_A1(dat),GET_LINE_A2(dat),GET_LINE_A3(dat),GET_LINE_A4(dat),\
																								GET_LINE_B0(dat),GET_LINE_B1(dat),GET_LINE_B2(dat));
		puts((char*)buf);
		//ESP8266_SendData(buf,strlen((char*)buf));
		
		delay_ms(100);
	}
	
#endif
	
	// STEP 1
	// ֱ��->��ֱ����
	uint8_t dat;
	_Bool round = 0; // Ȧ��
	
#if 0
	
_TRACE_STEP_1:
	
	STEP_WH = 0;
	while(1){
		dat = Read_74HC165();
		// ��⵽ֱ����
		if(STEP_WH==0){
			if(GET_LINE_A0(dat)==1){
				STOP_BACK_LINE_TRACE(500,500,200,1000,0);
				STEP_WH = 1; // �ظ�ֱ��
			}
			else{
				// ֱ��
				SINGLE_TRACE(500);
			}
		}
		else if(STEP_WH==1){
			// �ظ�ֱ��
			GO_LINE_TRACE(250);
		}
		delay_ms(5);
	}
	
	// STEP 2
	// ֱ��->Բ��->�������->�ֲ��
_TRACE_STEP_2:
	
	STEP_WH = 0;
	while(1)
	{
		dat = Read_74HC165();
		if(STEP_WH==0){
			if(GET_BUS_B(dat) == B111){
				// �ֲ�ڽ�ǰ
				if(round==0){
					STOP_BACK_LINE_TRACE(300,300,200,1000,0);
					STEP_WH = 1;
				}
				else{
					// �ڶ�Ȧ
					STEP_WH = 2;
				}
			}
			else if(GET_BUS_A(dat) == B11110 || GET_BUS_A(dat) == B01111)
			{
				// �ֲ�ڽϺ�
				if(round==0){
					STOP_BACK_LINE_TRACE(200,200,250,1000,0);
					STEP_WH = 1;
				}
				else{
					// �ڶ�Ȧ
					STEP_WH = 2;
				}
			}
			else{
				SINGLE_TRACE(300);
			}
		}
		else if(STEP_WH==1){
			// ���˲�·����
			GO_LINE_TRACE(250);
		}
		else if(STEP_WH==2){
			// �ڶ�Ȧ
			while((GET_BUS_B(dat) == B111) || (GET_BUS_A(dat) == B11110 || GET_BUS_A(dat) == B01111)){
				delay_ms(5);
			}
			goto _TRACE_STEP_3;
		}
		delay_ms(5);
	}

	// STEP 3
	// ֱ��->�ֲ��ֱ��(�ڶ�Ȧֱ��ͣ��)
_TRACE_STEP_3:
	STEP_WH = 0;
	while(1){
		dat = Read_74HC165();
		if(STEP_WH==0){
			// ���ֲ��
			if(GET_BUS_B(dat) == B111)
			{
				if(round==0){
					STOP_BACK_LINE_TRACE(500,500,200,1000,1);
					STEP_WH = 1; // �ظ�ֱ��
				}
				else{
					// �ڶ�Ȧͣ��
					goto _TRACE_END_STEP;
				}
			}
			else if(GET_BUS_A(dat)==B01111 || GET_BUS_A(dat)==B11110 || GET_BUS_A(dat)==B11111){
				// �ڶ�Ȧͣ��
				goto _TRACE_END_STEP;
			}
			else{
				// ֱ��
				SINGLE_TRACE(500);
			}
		}
		else if(STEP_WH==1){
			// �ظ�ֱ��
			GO_LINE_TRACE(250);
		}
		delay_ms(5);
	}
	
	// ���˵�һȦ���Ͻ���
	round = 1;
	//goto _TRACE_STEP_1; // �ڶ�Ȧ
	
_TRACE_END_STEP:
	SET_ANGLE(45);
	Motor_Stop(0);
	Motor_Stop(1);
	// DNF���������أ����ˣ���������������
#endif	

	return 0;
}
