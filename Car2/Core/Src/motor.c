/*
 * motor.c
 *
 *  Created on: Oct 22, 2025
 *      Author: AloysYan
 */


#include "motor.h"
#include "main.h"
#include "ax_ps2.h"

extern TIM_HandleTypeDef htim4;

#define DEAD_ZONE 15  	// 摇杆中间死区范围（防止微抖动）

// 初始化L298N控制引脚

void MOTOR_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能 GPIOA 和 GPIOB 时钟//
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置控制引脚为输出模式
    GPIO_InitStruct.Pin = MOTOR_F_IN1_PIN | MOTOR_F_IN2_PIN | MOTOR_F_IN3_PIN | MOTOR_F_IN4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MOTOR_GPIOB_PORT, &GPIO_InitStruct);
    HAL_GPIO_Init(MOTOR_GPIOA_PORT, &GPIO_InitStruct);//

}

// 向前移动
void MOTOR_Forward(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN4_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN4_PIN, GPIO_PIN_RESET);
}

// 向后移动
void MOTOR_Backward(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN4_PIN, GPIO_PIN_SET);
}

// 右转弯
void MOTOR_TurnRight(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN4_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN4_PIN, GPIO_PIN_RESET);
}

// 左转弯
void MOTOR_TurnLeft(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN4_PIN, GPIO_PIN_SET);
}

// 刹车
void MOTOR_Stop(void)
{
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_F_IN4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MOTOR_B_IN4_PIN, GPIO_PIN_SET);
}

// 使用按键控制（数字模式）
void MOTOR_ControlWithPS2(JOYSTICK_TypeDef *ps2)
{
    // 根据按键控制前进和后退
	if (ps2->btn1 & 0x10) {  	  // 如果按下 ↑（上方向键）
        MOTOR_Forward();      	  	  // 前进
    }
    else if (ps2->btn1 & 0x40) {  	  // 如果按下 ↓（下方向键）
        MOTOR_Backward();         	  // 后退
    }
    else if (ps2->btn1 & 0x80) {  	  // 如果按下 ←（左方向键）
        MOTOR_TurnLeft();     	  	  // 左转
    }
    else if (ps2->btn1 & 0x20) {  	  // 如果按下 →（右方向键）
        MOTOR_TurnRight();    	      // 右转
    }
    else if (ps2->btn2 & 0x10) {  	  // 按下三角形键（△）
    	for (int i=140; i<=250; i+=5) {
    		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, i);
    		HAL_Delay(100);
    	}

    }
    else if (ps2->btn2 & 0x40) { 	  // 按下X键（×）
    	for (int i=170; i>=50; i-=5) {
    	    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, i);
    	    HAL_Delay(100);
    	}
    }
    else {
        MOTOR_Stop();         	  	  // 没有按下任何方向键，停止马达
    }
}
