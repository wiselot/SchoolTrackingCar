#include "encoder.h"
#include "stm32f10x.h"

_ENCODER_DAT encoder_dat;

void Encoder_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. ����GPIOA��GPIOB��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 2. ����PA14��PA15Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ����PB0��PA5Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4. ����PA14��PA15��PB0��PA5���ⲿ�ж��ߵ�ӳ���ϵ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);  // PA14ӳ�䵽EXTI14
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  // PA15ӳ�䵽EXTI15
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);   // PB0ӳ�䵽EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);   // PA5ӳ�䵽EXTI5

    // 5. ��ʼ���ⲿ�ж�
    EXTI_InitStructure.EXTI_Line = EXTI_Line14 | EXTI_Line15 | EXTI_Line0 | EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  // �����غ��½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 6. �����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  // PA14��PA15�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  // PB0�ж�
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  // PA5�ж�
    NVIC_Init(&NVIC_InitStructure);
}

uint16_t readEncoder1()
{
	uint16_t cnt = encoder_dat.cnt1;
	encoder_dat.cnt1 = 0;
	return cnt;
}

uint16_t readEncoder2()
{
	uint16_t cnt = encoder_dat.cnt2;
	encoder_dat.cnt2 = 0;
	return cnt;
}
