#ifndef _TIME_H
#define _TIME_H

#include "stm32f10x.h"

void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_xms(u16 nms);
void delay_ms(u16 nms);

#endif