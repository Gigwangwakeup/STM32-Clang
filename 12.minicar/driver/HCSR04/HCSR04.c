/*
 * HCSR04.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#include "HCSR04.h"
#include "../../common/delay/delay.h"
#include "stdbool.h"

void HCSR04_init(hcsr04_t *sr04, GPIO_TypeDef *t_port, uint16_t t_pin,
		GPIO_TypeDef *e_port, uint16_t e_pin,
		TIM_HandleTypeDef *htim)
{
	sr04->trig_port	= t_port;
	sr04->trig_pin	= t_pin;
	sr04->echo_port	= e_port;
	sr04->echo_pin	= e_pin;
	sr04->sr04_htim	= htim;

	delay_init();
	HAL_GPIO_WritePin(sr04->trig_port, sr04->trig_pin, GPIO_PIN_RESET);
	HAL_TIM_Base_Start(sr04->sr04_htim);
	sr04->state	= STATE_IDLE;
}

static bool echo_is_high(hcsr04_t *sr04)
{
	return (HAL_GPIO_ReadPin(sr04->echo_port, sr04->echo_pin) == GPIO_PIN_SET);
}

hcsr04_status_t HCSR04_start(hcsr04_t *sr04)
{
	if(sr04->state == STATE_WAIT_RISE || sr04->state == STATE_WAIT_FALL) {
		return HCSR04_ERR_BUSY;
	}
	if(echo_is_high(sr04)){
		return HCSR04_ERR_BUSY;
	}
	sr04->state = STATE_WAIT_RISE;
	sr04->start_ms = HAL_GetTick();

	HAL_GPIO_WritePin(sr04->trig_port, sr04->trig_pin, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(sr04->trig_port, sr04->trig_pin, GPIO_PIN_RESET);
	return HCSR04_IN_PROGRESS;
}

void HCSR04_echoISR(hcsr04_t *sr04)
{
	uint32_t cnt = __HAL_TIM_GET_COUNTER(sr04->sr04_htim);
	bool high = echo_is_high(sr04);

	if((sr04->state == STATE_WAIT_RISE) && high){
		sr04->rise_cnt = cnt;
		sr04->state = STATE_WAIT_FALL;
	}
	else if((sr04->state == STATE_WAIT_FALL) && !high){
		sr04->fall_cnt = cnt;
		sr04->state = STATE_DONE;
	}
}

static uint32_t timer_diff(hcsr04_t *sr04, uint32_t start, uint32_t end)
{
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(sr04->sr04_htim);

	if (end >= start){
		return end - start;
	}
	else {
		return (arr - start) + end + 1;
	}
}

hcsr04_status_t HCSR04_getResult(hcsr04_t *sr04, uint32_t *echo_us)
{
	uint32_t elapsed_ms = HAL_GetTick() - sr04->start_ms;

	switch (sr04->state)
	{
	case STATE_WAIT_RISE:
		if(elapsed_ms <= RISE_TIMEOUT_MS) {
			return HCSR04_IN_PROGRESS;
		}
		sr04->state = STATE_IDLE;
		return HCSR04_ERR_NO_ECHO;
	case STATE_WAIT_FALL:
		if(elapsed_ms <= FALL_TIMEOUT_MS) {
			return HCSR04_IN_PROGRESS;
		}
		sr04->state = STATE_IDLE;
		return HCSR04_ERR_NO_ECHO;
	case STATE_DONE:
		uint32_t us = timer_diff(sr04, sr04->rise_cnt, sr04->fall_cnt);
		sr04->state = STATE_IDLE;
		*echo_us = us;
		return HCSR04_OK;
	case STATE_IDLE:
	default:
		return HCSR04_ERR_NOT_STARTED;
	}
}
