#ifndef _MG996R_H
#define _MG996R_H

#include "stm32f10x.h"

void mg996r_Init();

#define SET_ANGLE(angle) TIM_SetCompare4(TIM1,(0.5 + (angle / 180.0) * (2.5 - 0.5))/20.0 * 199);

#endif