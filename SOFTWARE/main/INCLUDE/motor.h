#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"

#define MOTOR_A_IN2		PBout(5)
#define MOTOR_A_IN1		PBout(4)

#define MOTOR_B_IN1		PAout(6)
#define MOTOR_B_IN2		PBout(1)

// 1ms的满速计数器值
#define FULL_SPEED_VAL	17

// 轴距(mm)
#define CAR_L 197
// 轮距
#define CAR_B	167

void allMotorInit();
void Motor_Forward(u8 left,u16 pwm_value);
void Motor_Backward(u8 left,u16 pwm_value);
void Motor_Stop(u8 left);

#endif