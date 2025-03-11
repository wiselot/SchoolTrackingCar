#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"

#define BUZZER_OFF GPIO_SetBits(GPIOA, GPIO_Pin_8);
#define BUZZER_ON GPIO_ResetBits(GPIOA, GPIO_Pin_8);

void Buzzer_Init(void);
void playTwinkle();

#endif