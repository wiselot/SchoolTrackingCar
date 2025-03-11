#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED0_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_15);
#define LED0_OFF GPIO_SetBits(GPIOB,GPIO_Pin_15);
#define LED1_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_14);
#define LED1_OFF GPIO_SetBits(GPIOB,GPIO_Pin_14);

void LED_Init(void);

#endif