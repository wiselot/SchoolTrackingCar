#ifndef _TIM_H
#define _TIM_H

#include "stm32f10x.h"

void TIM4_Encoder_Init(void);
void TIM3_PWM_Init(void);
void TIM2_PWM_Init(void);
void TIM1_PWM_Init(u16 arr, u16 psc);

#endif