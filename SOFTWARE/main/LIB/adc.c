#include "adc.h"
#include "time.h"

void Board_ADC_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOA 和 ADC1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    // 配置 PA0、PA1、PA2 为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure; 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能GPIOA,ADC1通道时钟
  
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz

		ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
 
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
		ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
		ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

		ADC_ResetCalibration(ADC1);	//重置指定的ADC1的复位寄存器

			while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待

		ADC_StartCalibration(ADC1);	 //

		while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
}

uint16_t ADC_GetValue(uint8_t ch) {
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC1,ADC通道3,第一个转换,采样时间为239.5周期	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

uint16_t ADC_GetValue_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=ADC_GetValue(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 