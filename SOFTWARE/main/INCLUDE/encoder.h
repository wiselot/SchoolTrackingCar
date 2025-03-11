#ifndef _ENCODER_H
#define _ENCODER_H

#include "stm32f10x.h"

typedef struct
{
	_Bool encoderA,encoderB; // ����ת
	uint16_t cnt1,cnt2;
}_ENCODER_DAT;

// ���߽Ӵ��ˡ�����
void Encoder_Init();
uint16_t readEncoder1();
uint16_t readEncoder2();

// ��̥�ܳ�(mm)
#define TYRE_CIRCLE		(3.14*78)
// ÿȦ������
#define PULSE_PER_CIRCLE 1040
// �����ٶ�
#define CNT2SPEED(CNT,TIME) ((float)(CNT)*TYRE_CIRCLE/(TIME)/PULSE_PER_CIRCLE)

#endif
