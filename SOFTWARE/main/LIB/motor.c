#include "stm32f10x.h"
#include "motor.h"
#include "tim.h"

void allMotorInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // 使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);                 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //使能SWD 禁用JTAG
	
  // 配置电机1的PWM引脚（PB3, TIM2 CH2）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // 配置电机1的AIN1（PB5）和AIN2（PB4）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*
  // 配置电机1的编码器引脚（PA14, PA15）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/

  // 配置电机2的PWM引脚（PA7, TIM3 CH2）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // 配置电机2的BIN1（PA6）和AIN2（PB1）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*
  // 配置电机2的编码器引脚（PB0, PA5）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/
	
	TIM3_PWM_Init();
	TIM2_PWM_Init();
	
}

// 控制电机正转
void Motor_Forward(u8 left,u16 pwm_value)
{
	if(left){
		MOTOR_A_IN2 = 1;
		MOTOR_A_IN1 = 0;
		TIM_SetCompare2(TIM2,pwm_value);
	}
	else{
		MOTOR_B_IN2 = 1;
		MOTOR_B_IN1 = 0;
		TIM_SetCompare2(TIM3,pwm_value);
	}
}

// 控制电机反转
void Motor_Backward(u8 left,u16 pwm_value)
{
  if(left){
		MOTOR_A_IN1 = 1;
		MOTOR_A_IN2 = 0;
		TIM_SetCompare2(TIM2,pwm_value);
	}
	else{
		MOTOR_B_IN1 = 1;
		MOTOR_B_IN2 = 0;
		TIM_SetCompare2(TIM3,pwm_value);
	}
}

// 控制电机停止
void Motor_Stop(u8 left)
{
  if(left){
		MOTOR_A_IN1 = 0;
		MOTOR_A_IN2 = 0;
	}
	else{
		MOTOR_B_IN1 = 0;
		MOTOR_B_IN2 = 0;
	}
}

