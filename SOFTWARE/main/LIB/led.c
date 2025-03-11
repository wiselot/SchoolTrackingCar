#include "led.h"

void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PB14 和 PB15 为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15; // PB14 和 PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 速度 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 初始状态：关闭 LED（高电平）
    GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
}