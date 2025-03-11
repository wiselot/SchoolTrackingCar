#include "stm32f10x.h"
#include "tim.h"

// TIM3��ʼ����CH1����1ms��ʱ����CH2����PWM���
void TIM3_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ʹ�ܶ�ʱ����GPIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // ����TIM3ΪPWMģʽ
    TIM_TimeBaseStructure.TIM_Period = 999; // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ����TIM3_CH2ΪPWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // ��ʼռ�ձ�Ϊ0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // ����1ms��ʱ���ж�
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ���ø����¼��ж�

    // ����NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ������ʱ��TIM3
    TIM_Cmd(TIM3, ENABLE);
}

// TIM2��ʼ����CH1����1ms��ʱ����CH2����PWM���
void TIM2_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ʹ�ܶ�ʱ����GPIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // ����TIM2ΪPWMģʽ
    TIM_TimeBaseStructure.TIM_Period = 999; // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ����TIM2_CH2ΪPWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // ��ʼռ�ձ�Ϊ0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ����1ms��ʱ���ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // ���ø����¼��ж�

    // ����NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ������ʱ��TIM2
    TIM_Cmd(TIM2, ENABLE);
}

/* TIM1CH4����mg996r���
 * PWM CH4
 */

void TIM1_PWM_Init(u16 arr, u16 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ʹ�� GPIOA �� TIM1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    // ���� PA11 Ϊ����������� (TIM1_CH4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��ʼ�� TIM1
    TIM_TimeBaseStructure.TIM_Period = arr;         // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;      // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    // ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ��ʼ�� TIM1 �� CH4 PWM ģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;       // PWM ģʽ 1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ʹ�����
    TIM_OCInitStructure.TIM_Pulse = 0;                      // ��ʼռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // �ߵ�ƽ��Ч
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // ʹ�� TIM1
    TIM_Cmd(TIM1, ENABLE);

    // ʹ�� TIM1 ����������߼���ʱ����Ҫ��
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
