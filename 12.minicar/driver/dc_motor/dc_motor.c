/*
 * dc_motor.c
 *
 *  Created on: 2026. 11. 11.
 *      Author: kccistc
 */
#include "dc_motor.h"

static int16_t motor_left_speed = 0;
static int16_t motor_right_speed = 0;
static int16_t motor_left_slowspeed = 0;
static int16_t motor_right_slowspeed = 0;

void motor_init(motor_t *motor, TIM_HandleTypeDef *htim, uint32_t Ch, GPIO_TypeDef* GPIO1, uint16_t GPIO1_Pin, GPIO_TypeDef* GPIO2, uint16_t GPIO2_Pin)
{
	motor->htim = htim;
	motor->Ch = Ch;
	motor->GPIO1 = GPIO1;
	motor->GPIO1_Pin = GPIO1_Pin;
	motor->GPIO2 = GPIO2;
	motor->GPIO2_Pin = GPIO2_Pin;
}

void motor_set_speed(int16_t left_speed, int16_t right_speed)
{
	   if(left_speed >= 1000) {
		   left_speed = 999;
	   }
	   else if(left_speed < 0){
		   left_speed = 0;
	   }

	   motor_left_speed = left_speed;
	   motor_left_slowspeed = (motor_left_speed > 300) ? (motor_left_speed - 300) : 100;

	   if(right_speed >= 1000) {
		   right_speed = 999;
	   }
	   else if(right_speed < 0){
		   right_speed = 0;
	   }

	   motor_right_speed = right_speed;
	   motor_right_slowspeed = (motor_right_speed > 300) ? (motor_right_speed - 300) : 100;
}

void motor_stop(motor_t *motor_left, motor_t *motor_right)
{
	HAL_TIM_PWM_Stop(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Stop(motor_right->htim, motor_right->Ch);
}

void motor_forward(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, motor_left_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_SET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, motor_right_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_SET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}

void motor_forward_left(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, 200);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_SET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, motor_right_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_SET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}

void motor_forward_right(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, motor_left_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_SET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, 200);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_SET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}

void motor_backward(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, motor_left_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_RESET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_SET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, motor_right_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_RESET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_SET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}

void motor_left(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, motor_left_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_RESET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, motor_right_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_SET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}

void motor_right(motor_t *motor_left, motor_t *motor_right)
{
	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Ch, motor_left_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_SET); // PB High
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Ch, motor_right_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_RESET); // PA HIGH
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET); // PA LOW

	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Ch);
	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Ch);
}
