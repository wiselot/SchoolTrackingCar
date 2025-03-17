#include "ctl.h"
#include "74HC165.h"
#include "stdio.h"
#include "mg996r.h"
#include "encoder.h"
#include "time.h"
#include "motor.h"
#include "math.h"
#include "binay.h"
#include "led.h"

// 步骤
static uint8_t FLOW_STEP = 0;
// 步骤状态
static uint8_t STEP_STAT = 1;

// 挡位
#define STAT_N	0
#define STAT_D	1
#define STAT_R	-1
static int8_t motorStatus = STAT_N;
// 舵机转向
static uint8_t ServoAngle = 45;
// 电机转速设定
static uint16_t motorLeft=0,motorRight=0;
// 待调速度
static uint16_t expSpeed = 200;

#define CTL_LOOP_TICK	1

// 初始化
void Ctl_Init()
{
	
}

// 舵机和电机控制
void ServoMotorCtlLoop()
{
	if(motorStatus==STAT_N) return;
	
	SET_ANGLE(ServoAngle);
	
	if(motorStatus==STAT_D){
		Motor_Forward(1,motorLeft);
		Motor_Forward(0,motorRight);
	}
	else if(motorStatus==STAT_R){
		Motor_Backward(1,motorLeft);
		Motor_Backward(0,motorRight);
	}
	else if(motorStatus==STAT_N){
		Motor_Stop(0);
		Motor_Stop(1);
	}
	
}

static uint8_t getBitCount(uint8_t c,_Bool bit)
{
	uint8_t cru = 0;
	for(uint8_t i=0;i<8;i++)
		if(GET_BIT(c,i)==bit)
			cru++;
	return cru;
}

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			ServoMotorCtlLoop();
		}
}

void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		}
}

// 直弯岔道选择记忆
/* 
0: 待右转
1: 待在上方岔道进内环
2: 待在下方岔道右转出内环
3: 待越过停止线
4: 待右转
5: 待在上方岔道进外环
6: 待在下方岔道进外环
7: 待在停止线停车
*/
static uint8_t exp_trace = 0;
	
// 直行:-1,右转:0,左转:1,停车:2
static int8_t GetTraceWay(uint8_t tra)
{
	/*
	if(exp_trace%2==0){
		LED1_ON;
	}
	else{
		LED1_OFF;
	}
	*/
	// 0: 待右转,
	if(exp_trace==0 || exp_trace==4)
	{
		static _Bool tr_sl = 0;
		static _Bool tr_sr = 0;
		if(tr_sl==0){
			if(tra>>4==0 || tra==0xff){
				tr_sl=1;
				if(tra>>4==0) tr_sr = 1;
				return 0;
			}
			else{
				return -1;
			}
		}
		else{
			// 回正
			if(tr_sr==1){
				if(tra==0xff)	tr_sr = 0;
			}
			else{
				if(tra!=0xff){
					exp_trace++;
				}
			}
			return 0;
		}
	}
	// 1: 待在上方岔道进内环,5: 待在上方岔道进外环
	else if(exp_trace==1||exp_trace==5){
		// 上方岔道
		static _Bool tr_sax = 0; // 等待过岔路口
		if(tr_sax==0){
			int8_t i = 7; // 这里用int8_t,不然减到-1会出问题，检查好久居然是这有问题...
			uint8_t cnx = 0,cny = 0,cnz = 0;
			while(i>=0 && GET_BIT(tra,i--)==0) cnx++;
			i++;
			while(i>=0 && GET_BIT(tra,i--)==1) cny++;
			i++;
			while(i>=0 && GET_BIT(tra,i--)==0) cnz++;
			
			if(cnx>=2 && cny>=1 && cnz>=2){	
				tr_sax=1; // 待出岔道
				
				return exp_trace==1?0:-1;
			}
			else{
				// 直行
				return -1;
			}
		}
		else{
			// 待出岔道
			static _Bool wait_dx = 0;
			if(!wait_dx){
				if(exp_trace==1 && tra==0xff){
					wait_dx = 1;
				}
				else if(exp_trace==5 && tra==0xff){
					wait_dx = 1;
				}
			}
			else{
				if(exp_trace==1 && (tra&0x0f)==0x0f && (tra>>4)!=0x0f){
					exp_trace++;
					return 0;
				}
				else if(exp_trace==5){
					// 不做任何检测应该是能直接跑过去的
					exp_trace++;
					return -1;
				}
			}
			return exp_trace==1?0:-1;
		}
	}
	// 2: 待在下方岔道右转出内环
	else if(exp_trace==2){
		static _Bool truax = 0;
		if(!truax){
			if(tra==0x00){
				truax = 1;
				return 0;
			}
			return -1;
		}
		else{
			static _Bool trubx = 0;
			if(!trubx){
				if(tra==0xff){
					trubx = 1;
				}
				return 0;
			}
			else{
				if(getBitCount(tra,0)>=5){
					// 这里是回正判断
					// 测试的时候发现边缘有细黑线和外部环境影响巡线，并且差不多回正扫到停止线,故直接检测停止线
					LED1_ON;
					exp_trace++;
					return -1;
				}
				return 0;
			}
		}
	}
	// 3: 待越过停止线,7: 待在停止线停车
	else if(exp_trace==3 || exp_trace==6)
	{
		// 根据实际调一下哈
		if(exp_trace==3){
			// 上面已经扫到停止线了，直接越过停止线应该不会导致乱跑，所以直接直行好了。
			exp_trace++;
			return -1;
		}
		else{
			if(getBitCount(tra,0)>=5)
				// 停车
				return 2;
		}
	}
	// 6: 待在下方岔道进外环
	else if(exp_trace==4){
		static _Bool tr_s = 0;
		if(tr_s==0){
			if(GET_BIT(tra,7)==0)
				tr_s = 1;
		}
		else if(tr_s==1){
			if(GET_BIT(tra,7)!=0)
				exp_trace++;
		}
		return -1;
	}
}

static uint8_t TRA2Angle(uint8_t tra){
	// 偏到0xff,保证回正
	static _Bool warnTrace;
	if(tra==0xff)
		return warnTrace==0?0:90;
	else
		warnTrace = ((tra&0x0f) - (tra>>4))>0;
	uint16_t xsum = 0,xt=0;
	for(uint8_t i=0;i<8;i++){
		if(GET_BIT(tra,i)==0){
			xsum += ((i+1)*9*1.3);
			xt++;
		}
	}
	uint8_t angle = xsum/xt;
	if(angle>90)
		angle=90;
	// 急弯减速
	static _Bool speed_reg = 0;
	if(!speed_reg){
		if(angle>80 || angle<10){
			speed_reg = 1;
			expSpeed*=0.7;
		}
	}
	else if(speed_reg && angle>=80 && angle<=10){
		speed_reg = 0;
		expSpeed/=0.7;
	}
	return angle;
}


// 循迹流程控制
void TraceFlowCtlLoop()
{
	motorStatus = STAT_D;
	
	while(1)
	{

		uint8_t tra = Read_74HC165();
#if 0
		printf("%d\r\n",TRA2Angle(tra));
		//ServoAngle = TRA2Angle(tra);
		//ServoAngle = 90;
		//motorLeft = 1000;
		//motorRight = 1000;
		
		delay_ms(10);
#endif
		
#if 1
		int8_t way = GetTraceWay(tra);
		
		switch(way){
			case -1:
				// 直行
				ServoAngle = TRA2Angle(tra);
				float ratio = CAR_B*tan((double)(ServoAngle-45))/CAR_L;
				motorLeft = expSpeed + expSpeed*ratio/2;
				motorRight = expSpeed - expSpeed*ratio/2;
			break;
			case 0:
				// 右转
				ServoAngle = 90;
				motorLeft = 300;
				motorRight = 200;
			break;
			case 1:
				// 左转
				ServoAngle = 0;
				motorLeft = 200;
				motorRight = 300;
			break;
			case 2:
				// 停车
				motorStatus = STAT_N;
			break;
		}
#endif
		
	}
	
}