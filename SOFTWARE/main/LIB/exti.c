#include "stm32f10x.h"
#include "key.h"
#include "encoder.h"
#include "stdio.h"

extern KEY_CALLBACK intr_key0 ,intr_key1;
extern _ENCODER_DAT encoder_dat;

void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
				if(intr_key1)
					intr_key1();
				else
					printf("KEY1 Pressed");
        EXTI_ClearITPendingBit(EXTI_Line12); // 清除中断标志
    }

    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
			  if(intr_key0)
					intr_key0();
				else
					printf("KEY0 Pressed");
        EXTI_ClearITPendingBit(EXTI_Line13); // 清除中断标志
    }
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)  // 判断是否是PA14的中断
    {
        EXTI_ClearITPendingBit(EXTI_Line14);  // 清除中断标志位
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15))  // 检查PA15的电平
        {
            encoder_dat.encoderA = 1;
						encoder_dat.cnt1++;
        }
        else
        {
            encoder_dat.encoderA = 0;
					encoder_dat.cnt1++;
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line15) != RESET)  // 判断是否是PA15的中断
    {
        EXTI_ClearITPendingBit(EXTI_Line15);  // 清除中断标志位
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14))  // 检查PA14的电平
        {
            encoder_dat.encoderA = 0;
						encoder_dat.cnt1++;
        }
        else
        {
            encoder_dat.encoderA = 1;
						encoder_dat.cnt1++;
        }
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)  // 判断是否是PB0的中断
    {
        EXTI_ClearITPendingBit(EXTI_Line0);  // 清除中断标志位
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))  // 检查PA5的电平
        {
            encoder_dat.encoderB = 1;
						encoder_dat.cnt2++;
        }
        else
        {
            encoder_dat.encoderB = 0;
						encoder_dat.cnt2++;
        }
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)  // 判断是否是PA5的中断
    {
        EXTI_ClearITPendingBit(EXTI_Line5);  // 清除中断标志位
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))  // 检查PB0的电平
        {
            encoder_dat.encoderB = 0;
						encoder_dat.cnt2++;
        }
        else
        {
            encoder_dat.encoderB = 1;
						encoder_dat.cnt2++;
        }
    }
}