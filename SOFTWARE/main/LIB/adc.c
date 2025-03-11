#include "adc.h"
#include "time.h"

void Board_ADC_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOA �� ADC1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    // ���� PA0��PA1��PA2 Ϊģ������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ģ������ģʽ
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure; 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��GPIOA,ADC1ͨ��ʱ��
  
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6ʱ��Ϊ72M/6=12MHz

		ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
 
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
		ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
		ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

		ADC_ResetCalibration(ADC1);	//����ָ����ADC1�ĸ�λ�Ĵ���

			while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�

		ADC_StartCalibration(ADC1);	 //

		while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
}

uint16_t ADC_GetValue(uint8_t ch) {
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC1,ADCͨ��3,��һ��ת��,����ʱ��Ϊ239.5����	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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