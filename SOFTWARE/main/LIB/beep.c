#include "beep.h"
#include "time.h"

void Buzzer_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA8 为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       // PA8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度 50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始状态：关闭蜂鸣器（高电平）
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