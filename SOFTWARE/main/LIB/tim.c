#include "stm32f10x.h"
#include "tim.h"

// TIM3初始化：CH1用于1ms定时器，CH2用于PWM输出
void TIM3_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能定时器和GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 配置TIM3为PWM模式
    TIM_TimeBaseStructure.TIM_Period = 999; // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 配置TIM3_CH2为PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // 配置1ms定时器中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 启用更新事件中断

    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器TIM3
    TIM_Cmd(TIM3, ENABLE);
}

// TIM2初始化：CH1用于1ms定时器，CH2用于PWM输出
void TIM2_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能定时器和GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置TIM2为PWM模式
    TIM_TimeBaseStructure.TIM_Period = 999; // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置TIM2_CH2为PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 配置1ms定时器中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 启用更新事件中断

    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器TIM2
    TIM_Cmd(TIM2, ENABLE);
}

/* TIM1CH4用于mg996r舵机
 * PWM CH4
 */

void TIM1_PWM_Init(u16 arr, u16 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能 GPIOA 和 TIM1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    // 配置 PA11 为复用推挽输出 (TIM1_CH4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化 TIM1
    TIM_TimeBaseStructure.TIM_Period = arr;         // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;      // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 初始化 TIM1 的 CH4 PWM 模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;       // PWM 模式 1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 0;                      // 初始占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // 使能 TIM1
    TIM_Cmd(TIM1, ENABLE);

    // 使能 TIM1 的主输出（高级定时器需要）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
