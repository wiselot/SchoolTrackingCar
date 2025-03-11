#include "led.h"

void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOB ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // ���� PB14 �� PB15 Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15; // PB14 �� PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // �ٶ� 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ��ʼ״̬���ر� LED���ߵ�ƽ��
    GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
}