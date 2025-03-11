#ifndef _CTL_H
#define _CTL_H

#include "stm32f10x.h"

// ADC参数
/* 左右传感器差值(D_AD_VALUE=Left_AD-Right_AD) */
#define D_AD_VALUE			-24
/* 左传感器峰值(LEFT_MAX)*/
#define LEFT_MAX				3114
/* 右传感器峰值(RIGHT_MAX)*/
#define RIGHT_MAX				3129
/* 左阈值点(LEFT_THREASH) */
#define LEFT_THREASH		996
/* 右阈值点(RIGHT_THREASH) */
#define RIGHT_THREASH		1167
/* 左跳跃点 */
#define LEFT_SPAN				3569
/* 右跳跃点 */
#define RIGHT_SPAN			-3410

int GetTraceDate(void);
void GetParament(void);
int Control_Servor_Angle(void);
void dat_test();

#endif