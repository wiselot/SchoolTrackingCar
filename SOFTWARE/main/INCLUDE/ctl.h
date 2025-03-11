#ifndef _CTL_H
#define _CTL_H

#include "stm32f10x.h"

// ADC����
/* ���Ҵ�������ֵ(D_AD_VALUE=Left_AD-Right_AD) */
#define D_AD_VALUE			-24
/* �󴫸�����ֵ(LEFT_MAX)*/
#define LEFT_MAX				3114
/* �Ҵ�������ֵ(RIGHT_MAX)*/
#define RIGHT_MAX				3129
/* ����ֵ��(LEFT_THREASH) */
#define LEFT_THREASH		996
/* ����ֵ��(RIGHT_THREASH) */
#define RIGHT_THREASH		1167
/* ����Ծ�� */
#define LEFT_SPAN				3569
/* ����Ծ�� */
#define RIGHT_SPAN			-3410

int GetTraceDate(void);
void GetParament(void);
int Control_Servor_Angle(void);
void dat_test();

#endif