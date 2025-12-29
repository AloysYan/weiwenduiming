/*
 * motor.c
 *
 *  Created on: Oct 22, 2025
 *      Author: AloysYan
 */


#include "motor.h"
#include "main.h"
#include "ax_ps2.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

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
    HAL_GPIO_Init(MOTOR_GPIOA_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN4_PIN, GPIO_PIN_RESET);
}
/*
// 设置马达的速度（PWM控制）
void MOTOR_SetSpeed(uint8_t speed)
{
	// 映射 speed 为 PWM 占空比（0~255）
	uint16_t pwm_value = (uint16_t)((speed / 255.0f) * __HAL_TIM_GET_AUTORELOAD(&htim1));

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_value);  	// 设置马达FL的PWM占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_value);  	// 设置马达FR的PWM占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_value);  	// 设置马达BL的PWM占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm_value);  	// 设置马达BR的PWM占空比
}
*/
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

// 涡轮启动（参数待调）
void MG_ON(void) {
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 50);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 50);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN4_PIN, GPIO_PIN_RESET);
}

// 涡轮关闭（应为滑行状态）
void MG_OFF(void) {
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_GPIOB_PORT, MG_IN4_PIN, GPIO_PIN_RESET);
}

// 使用按键控制（数字模式）
void MOTOR_ControlWithPS2(JOYSTICK_TypeDef *ps2)
{
    // 根据按键控制前进和后退
	if (ps2->btn1 & 0x10) {  	      // 如果按下 ↑（上方向键）
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
    	MG_ON();
    }
    else if (ps2->btn2 & 0x40) { 	  // 按下X键（×）
    	MG_OFF();
    }
    else {
        MOTOR_Stop();         	  	  // 没有按下任何方向键，停止马达
    }
}
