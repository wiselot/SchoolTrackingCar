#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x.h"

void Board_ADC_Init(void);
uint16_t ADC_GetValue(uint8_t ch);
uint16_t ADC_GetValue_Average(u8 ch,u8 times);

#endif