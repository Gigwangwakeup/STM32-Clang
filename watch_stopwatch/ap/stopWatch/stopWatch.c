/*
 * stopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "stopWatch.h"
#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"


typedef enum{STOP, RUN, CLEAR}stopWatch_t;

static void StopWatch_stop();
static void StopWatch_run();
static void StopWatch_clear();
static void StopWatch_setState(stopWatch_t s);
static stopWatch_t StopWatch_getState(void);
static void StopWatch_incCounter(void);
static void StopWatch_clearCounter(void);
static void StopWatch_setCounter(uint16_t c);
static uint16_t StopWatch_getCounter(void);

static stopWatch_t stopWatch_state;
static uint16_t counter;

static led_t	led_stopwatch;
static led_t	led_run;
static led_t	led_stop;

static button_t btn_run_stop;
static button_t btn_clear;

extern TIM_HandleTypeDef htim3;

static void StopWatch_setState(stopWatch_t s)
{
	stopWatch_state = s;
}

static stopWatch_t StopWatch_getState(void)
{
	return stopWatch_state;
}

static void StopWatch_incCounter(void)
{
	counter++;
}

static void StopWatch_clearCounter(void)
{
	counter = 0;
}

static void StopWatch_setCounter(uint16_t c)
{
	counter = c;
}

static uint16_t StopWatch_getCounter(void)
{
	uint16_t min = counter / 60;
	uint16_t sec = counter % 60;
	uint16_t mmss = ((min*100)+sec);
	return mmss;
}

void StopWatch_init(void)
{
	stopWatch_state = STOP;
	counter = 0;

	led_init(&led_stopwatch, GPIOB, GPIO_PIN_14);
	led_init(&led_stop,	GPIOB, GPIO_PIN_13	);
	led_init(&led_run,	GPIOC, GPIO_PIN_4	);

	button_init(&btn_run_stop,	GPIOC, GPIO_PIN_1);
	button_init(&btn_clear,		GPIOC, GPIO_PIN_3);

	HAL_TIM_Base_Start_IT(&htim3);
}

void StopWatch_task(void)
{
	switch(stopWatch_state)
	{
	case STOP:
		StopWatch_stop();
		led_on(&led_stop);
		led_off(&led_run);
		break;
	case RUN:
		StopWatch_run();
		led_on(&led_run);
		led_off(&led_stop);
		break;
	case CLEAR:
		StopWatch_clear();
		break;
	default:
		StopWatch_stop();
		led_on(&led_stop);
		led_off(&led_run);
		break;
	}
}

static void StopWatch_stop()
{
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(RUN);
	}
	else if (button_getState(&btn_clear) == ACT_RELEASE){
		StopWatch_setState(CLEAR);
	}
	uint16_t num = StopWatch_getCounter();

	fnd_set_num(num);
}

static void StopWatch_run()
{
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(STOP);
	}

	static uint32_t prevTime = 0;
	uint32_t curTime = HAL_GetTick();
	if(curTime - prevTime < 1000){
		return;
	}
	prevTime = curTime;

	if(counter == 3599){
		StopWatch_clearCounter();
	}
	else {
		StopWatch_incCounter();
	}

	fnd_set_num(StopWatch_getCounter());
}

static void StopWatch_clear()
{
	uint16_t num;

	StopWatch_clearCounter();
	num = StopWatch_getCounter();

	fnd_set_num(num);
	StopWatch_setState(STOP);
}

void stopWatch_ledoff(void)
{
	led_off(&led_stopwatch);
	led_off(&led_run);
	led_off(&led_stop);
}

void stopWatch_ledon(void)
{
	led_on(&led_stopwatch);
}
