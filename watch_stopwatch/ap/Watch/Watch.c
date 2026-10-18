/*
 * Watch.c
 *
 *  Created on: 2026. 10. 18.
 *      Author: kccistc
 */

#include "Watch.h"
#include "../../driver/button/button.h"
#include "../../driver/fnd/fnd.h"
#include "../../driver/led/led.h"

static volatile uint16_t counter;
static led_t led_watch;


extern TIM_HandleTypeDef htim2;

static void Watch_incCounter(void) // 카운터 증가
{
	counter++;
}

static void Watch_clearCounter(void) // 카운터 증가
{
	counter = 0;
}

static void Watch_setCounter(uint16_t c) // 카운터 설정
{
	counter = c;
}

static uint16_t Watch_getCounter(void) // 카운터 값 읽기
{
	uint16_t min = counter / 60;
	uint16_t sec = counter % 60;
	uint16_t mmss = ((min*100)+sec);
	return mmss;
}

void Watch_tick(void)
{
	if(counter == 3599){
		Watch_clearCounter();
	}
	else {
		Watch_incCounter();
	}
}

void Watch_ledoff(void)
{
	led_off(&led_watch);
}

void Watch_ledon(void)
{
	led_on(&led_watch);
}

void Watch_init(void) // 워치 초기값
{
	counter = 0;

	led_init(&led_watch, GPIOB, GPIO_PIN_15);

	HAL_TIM_Base_Start_IT(&htim2);
}

void Watch_task(void)
{

	fnd_set_num(Watch_getCounter());
	Watch_ledon();
}
