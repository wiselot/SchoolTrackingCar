#include "key.h"
#include "stdio.h"

KEY_CALLBACK intr_key0 = 0,intr_key1 = 0;

void Key_Init(KEY_CALLBACK key0,KEY_CALLBACK key1){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�� GPIOB ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // ���� PB12 �� PB13 Ϊ����ģʽ�������ڲ���������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ʹ�� AFIO ʱ�ӣ����� EXTI ���ã�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // �� PB12 �� PB13 ӳ�䵽 EXTI12 �� EXTI13
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    // ���� EXTI12 �� EXTI13 Ϊ�½��ش���
    EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ���� NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // EXTI12 �� EXTI13 ���ø��ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		// �жϻص�����
		intr_key0 = key0;
		intr_key1 = key1;
}

