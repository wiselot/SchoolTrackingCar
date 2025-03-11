#include "key.h"
#include "stdio.h"

KEY_CALLBACK intr_key0 = 0,intr_key1 = 0;

void Key_Init(KEY_CALLBACK key0,KEY_CALLBACK key1){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PB12 和 PB13 为输入模式，启用内部上拉电阻
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 输入上拉模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 使能 AFIO 时钟（用于 EXTI 配置）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 将 PB12 和 PB13 映射到 EXTI12 和 EXTI13
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    // 配置 EXTI12 和 EXTI13 为下降沿触发
    EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // EXTI12 和 EXTI13 共用该中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		// 中断回调函数
		intr_key0 = key0;
		intr_key1 = key1;
}

