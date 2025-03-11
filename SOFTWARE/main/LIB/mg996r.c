#include "stm32f10x.h"
#include "mg996r.h"
#include "tim.h"

void mg996r_Init()
{
	TIM1_PWM_Init(199,7199);
}

