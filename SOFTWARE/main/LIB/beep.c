#include "beep.h"
#include "time.h"

void Buzzer_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ���� PA8 Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       // PA8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // �ٶ� 50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��ʼ״̬���رշ��������ߵ�ƽ��
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
}

void playTwinkle()
{
	BUZZER_ON
  delay_ms(500);
	BUZZER_OFF
  delay_ms(500);

  BUZZER_ON
  delay_ms(500);
  BUZZER_OFF
  delay_ms(500);

  BUZZER_ON
  delay_ms(1000);
  BUZZER_OFF
  delay_ms(500);
}