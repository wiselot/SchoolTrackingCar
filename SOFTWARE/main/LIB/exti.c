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
        EXTI_ClearITPendingBit(EXTI_Line12); // ����жϱ�־
    }

    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
			  if(intr_key0)
					intr_key0();
				else
					printf("KEY0 Pressed");
        EXTI_ClearITPendingBit(EXTI_Line13); // ����жϱ�־
    }
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)  // �ж��Ƿ���PA14���ж�
    {
        EXTI_ClearITPendingBit(EXTI_Line14);  // ����жϱ�־λ
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15))  // ���PA15�ĵ�ƽ
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
    else if (EXTI_GetITStatus(EXTI_Line15) != RESET)  // �ж��Ƿ���PA15���ж�
    {
        EXTI_ClearITPendingBit(EXTI_Line15);  // ����жϱ�־λ
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14))  // ���PA14�ĵ�ƽ
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
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)  // �ж��Ƿ���PB0���ж�
    {
        EXTI_ClearITPendingBit(EXTI_Line0);  // ����жϱ�־λ
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))  // ���PA5�ĵ�ƽ
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
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)  // �ж��Ƿ���PA5���ж�
    {
        EXTI_ClearITPendingBit(EXTI_Line5);  // ����жϱ�־λ
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))  // ���PB0�ĵ�ƽ
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