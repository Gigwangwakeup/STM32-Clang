/*
 * ap_main.c
 *
 *  Created on: 2026. 11. 2.
 *      Author: kccistc
 */
//application
#include "ap_main.h"
//service
#include "service/timeClock/timeClock.h"
#include "service/tempHumid/tempHumid.h"
#include "service/ruler/ruler.h"
#include "service/stopWatch/stopWatch.h"
#include "service/que/que.h"
#include "service/inputEvent/inputEvent.h"
//driver
//#include "../driver/button/button.h"

typedef enum{
	S_CALENDAR = 0,
	S_TEMP_HUMID,
	S_RULER,
	S_STOPWATCH
}smartWatch_state_t;

smartWatch_state_t smartWatch_state = S_CALENDAR;
event_btn_act_t btn_mode = ACT_PUSHED;

//button_t btnMode;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_6) {
		ruler_ISR();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		stopWatch_ISR();
	}
}

//static bool ap_getModeEvent(void){ // que delay test code
//	static uint32_t prevTime = 0;
//
//	if(HAL_GetTick() - prevTime < 1000) return false;   // 1초 안 지났으면 안 꺼냄
//	if(que_isEmptyMode() == true) return false;
//	prevTime = HAL_GetTick();
//
//	que_getMode(&btn_mode);
//	return (btn_mode == ACT_RELEASED);
//}

void ap_init(void){
	timeClock_init();
	timeClock_setDateFirstFlag();
	tempHumid_init();
	//	button_init(&btnMode, GPIOC, GPIO_PIN_0);
	ruler_init();
	stopWatch_init();
	que_init();
	inputEvent_init();
}

int ap_main(void)
{
	ap_init();
	while(1)
	{
		timeClock_task();
		inputEvent_task();
		switch(smartWatch_state)
		{
		case S_CALENDAR :
			if(que_isEmptyMode() == false){
				que_getMode(&btn_mode);
				if(btn_mode == ACT_RELEASED){
					smartWatch_state = S_TEMP_HUMID;
				}
			}
//			if(ap_getModeEvent()){ // que delay test code
//				smartWatch_state = S_TEMP_HUMID;
//			}
			timeClock_calendar_task();
			break;
		case S_TEMP_HUMID:
			if(que_isEmptyMode() == false){
				que_getMode(&btn_mode);
				if(btn_mode == ACT_RELEASED){
					smartWatch_state = S_RULER;
					ruler_disp(0);
				}
			}
//			if(ap_getModeEvent()){
//				smartWatch_state = S_RULER;
//				ruler_disp(0);
//			}
			tempHumid_task();
			break;
		case S_RULER:
			if(que_isEmptyMode() == false){
				que_getMode(&btn_mode);
				if(btn_mode == ACT_RELEASED){
					smartWatch_state = S_STOPWATCH;
					stopWatch_disp();
				}
			}
//			if(ap_getModeEvent()){
//				smartWatch_state = S_STOPWATCH;
//				stopWatch_disp();
//			}
			ruler_task();
			break;
		case S_STOPWATCH:
			if(que_isEmptyMode() == false){
				que_getMode(&btn_mode);
				if(btn_mode == ACT_RELEASED){
					timeClock_setDateFirstFlag();
					smartWatch_state = S_CALENDAR;
				}
			}
//			if(ap_getModeEvent()){
//				smartWatch_state = S_CALENDAR;
//				timeClock_setDateFirstFlag();
//			}
			stopWatch_task();
			break;
		default:
			timeClock_calendar_task();
			break;
		}
	}
	return 0;
}
