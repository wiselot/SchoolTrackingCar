#include "encoder.h"
#include "stm32f10x.h"

_ENCODER_DAT encoder_dat;

void Encoder_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 启用GPIOA、GPIOB和AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 2. 配置PA14和PA15为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置PB0和PA5为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4. 配置PA14、PA15、PB0和PA5与外部中断线的映射关系
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);  // PA14映射到EXTI14
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  // PA15映射到EXTI15
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);   // PB0映射到EXTI0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);   // PA5映射到EXTI5

    // 5. 初始化外部中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line14 | EXTI_Line15 | EXTI_Line0 | EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  // 上升沿和下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 6. 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  // PA14和PA15中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  // PB0中断
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  // PA5中断
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
