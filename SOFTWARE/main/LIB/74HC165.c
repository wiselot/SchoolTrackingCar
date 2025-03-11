#include "74HC165.h"
#include "stm32f10x.h"
#include "time.h"

void _74HC165_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ���� PA0 Ϊ����ģʽ��DAT��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� PA1 Ϊ���ģʽ��CLK��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� PA2 Ϊ���ģʽ��LD��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��ʼ��ʱ���� LD �� CLK ��Ϊ�͵�ƽ
    GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}
uint8_t Read_74HC165(void)
{
  uint8_t i;
	uint8_t temp = 0;

	_74HC165_LD = 0; //PL���ͣ���ȡ����״̬
	_74HC165_LD = 1; //PL���ߣ�ֹͣ������ȡ

	if(_74HC165_DAT == 1)        //����1����������D7���ŵİ���������
			temp |= 0x01;         //���λ��1��ʾ���������£���֮��û����������
	for(i = 0; i < 7; i ++)  //��Ϊ���λ������λ���ɶ�ȡ����ѭ��(16-1)�Σ����ΰѴθ�λ�Ƶ����λ����һ��74HC165���ȶ�ȡ
	{
		temp = temp << 1;

		_74HC165_CLK = 0; //HC165_CLK = 0 ����> HC165_CLK = 1 ���������أ��θ�λD(n-1)�Ƶ���λD(n)
		delay_us(10);
		_74HC165_CLK = 1;

		if(_74HC165_DAT == 1)
			temp |= 0x01;
	}

	return temp;
}