/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "ap_main.h"
#include "stopWatch/stopWatch.h"
#include "Watch/Watch.h" // 폴더 및 .c.h 생성 필요
#include "../driver/fnd/fnd.h"
#include "../driver/button/button.h"


typedef enum {Watch, stopWatch}ap_mode_t;
static ap_mode_t watch_stopwatch_mode;
static button_t btn_mode;

void mode_s(ap_mode_t s){
	watch_stopwatch_mode = s;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3){
		fnd_show_number();
	}
	else if(htim->Instance == TIM2){
		Watch_tick();
	}
}
int ap_init(void)
{
	StopWatch_init();
	Watch_init();

	button_init(&btn_mode,	GPIOC, GPIO_PIN_0);
}
int ap_main(void)
{
	ap_init();
	while(1)
	{
		if((watch_stopwatch_mode == stopWatch) &&(button_getState(&btn_mode)== ACT_PUSH))
		{
			mode_s(Watch);
		}
		else if((watch_stopwatch_mode == Watch) && (button_getState(&btn_mode)== ACT_PUSH))
		{
			mode_s(stopWatch);
		}

		switch(watch_stopwatch_mode)
		{
		case stopWatch:
			StopWatch_task();
			stopWatch_ledon();
			Watch_ledoff();
			break;
		case Watch:
			Watch_task();
			Watch_ledon();
			stopWatch_ledoff();
			break;
		default:
			StopWatch_task();
			stopWatch_ledon();
			Watch_ledoff();
			break;
		}
	}
	return 0;
}
