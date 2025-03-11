#ifndef _ENCODER_H
#define _ENCODER_H

#include "stm32f10x.h"

typedef struct
{
	_Bool encoderA,encoderB; // 正反转
	uint16_t cnt1,cnt2;
}_ENCODER_DAT;

// 接线接错了。。。
void Encoder_Init();
uint16_t readEncoder1();
uint16_t readEncoder2();

// 轮胎周长(mm)
#define TYRE_CIRCLE		(3.14*78)
// 每圈脉冲数
#define PULSE_PER_CIRCLE 1040
// 计算速度
#define CNT2SPEED(CNT,TIME) ((float)(CNT)*TYRE_CIRCLE/(TIME)/PULSE_PER_CIRCLE)

#endif
