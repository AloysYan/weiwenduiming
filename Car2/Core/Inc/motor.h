/*
 * motor.h
 *
 *  Created on: Oct 22, 2025
 *      Author: AloysYan
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f1xx_hal.h"
#include "ax_ps2.h"

// 定义L298N控制引脚
#define MOTOR_F_IN1_PIN   GPIO_PIN_12
#define MOTOR_F_IN2_PIN   GPIO_PIN_13
#define MOTOR_F_IN3_PIN   GPIO_PIN_14
#define MOTOR_F_IN4_PIN   GPIO_PIN_15

#define MOTOR_B_IN1_PIN   GPIO_PIN_4
#define MOTOR_B_IN2_PIN   GPIO_PIN_5
#define MOTOR_B_IN3_PIN   GPIO_PIN_6
#define MOTOR_B_IN4_PIN   GPIO_PIN_7

#define MOTOR_GPIOB_PORT   GPIOB
#define MOTOR_GPIOA_PORT   GPIOA

// 函数声明

// 初始化电机控制引脚和 PWM 定时器
void MOTOR_Init(void);

// 基本的电机方向控制函数
void MOTOR_Forward(void);      // 电机前进
void MOTOR_Backward(void);     // 电机后退
void MOTOR_TurnLeft(void);     // 电机左转
void MOTOR_TurnRight(void);    // 电机右转
void MOTOR_Stop(void);         // 电机停止

// 控制电机的方向和速度（根据 PS2 摇杆输入）
//void MOTOR_ControlAnalog(JOYSTICK_TypeDef *ps2);

// 控制电机的方向和速度（根据 PS2 按键输入）
void MOTOR_ControlWithPS2(JOYSTICK_TypeDef *ps2);

#endif /* INC_MOTOR_H_ */
