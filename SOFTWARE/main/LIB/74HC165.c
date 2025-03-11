#include "74HC165.h"
#include "stm32f10x.h"
#include "time.h"

void _74HC165_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA0 为输入模式（DAT）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 PA1 为输出模式（CLK）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 PA2 为输出模式（LD）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化时，将 LD 和 CLK 置为低电平
    GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}
uint8_t Read_74HC165(void)
{
  uint8_t i;
	uint8_t temp = 0;

	_74HC165_LD = 0; //PL拉低，读取按键状态
	_74HC165_LD = 1; //PL拉高，停止按键读取

	if(_74HC165_DAT == 1)        //等于1即并行输入D7引脚的按键被按下
			temp |= 0x01;         //最低位置1表示按键被按下，反之则没按键被按下
	for(i = 0; i < 7; i ++)  //因为最高位不用移位即可读取，故循环(16-1)次，依次把次高位移到最高位，第一级74HC165优先读取
	{
		temp = temp << 1;

		_74HC165_CLK = 0; //HC165_CLK = 0 ——> HC165_CLK = 1 产生上升沿，次高位D(n-1)移到高位D(n)
		delay_us(10);
		_74HC165_CLK = 1;

		if(_74HC165_DAT == 1)
			temp |= 0x01;
	}

	return temp;
}