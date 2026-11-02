/*
 * Ruler.c
 *
 *  Created on: 2026. 11. 2.
 *      Author: kccistc
 */

#include "stdio.h"
#include "stdbool.h"

#include "ruler.h"

#include "../../../driver/HCSR04/HCSR04.h"
#include "../../../driver/button/button.h"
#include "../../../driver/lcd/lcd.h"

extern TIM_HandleTypeDef htim10;

static uint32_t echo_us = 0;
static hcsr04_status_t hcsr04_status = HCSR04_ERR_NOT_STARTED;
static bool start_flag = false;
static button_t btn_ruler;

#define TRIG_PORT	GPIOC
#define TRIG_PIN	GPIO_PIN_5
#define ECHO_PORT	GPIOC
#define ECHO_PIN	GPIO_PIN_6
#define RULER_TIMER &htim10

#define BTN_RULER_PORT	GPIOC
#define BTN_RULER_PIN	GPIO_PIN_1

void ruler_init(void){
	HCSR04_init(TRIG_PORT, TRIG_PIN, ECHO_PORT, ECHO_PIN, RULER_TIMER);
	button_init(&btn_ruler, BTN_RULER_PORT, BTN_RULER_PIN);
}

static void ruler_request(void){
	start_flag = true;
	HCSR04_start();
}

void ruler_disp(uint16_t distance){
	char buff[30];
	sprintf(buff,"dist : %04dcm    ", distance);
	Lcd_writeStringXY(1, 0, buff);
}

void ruler_task(void){
	if(button_getState(&btn_ruler) == ACT_RELEASE) {
		ruler_request();
	}

	if(start_flag != true){
		return;
	}

	hcsr04_status = HCSR04_getResult(&echo_us);

	if(hcsr04_status != HCSR04_OK){
		return;
	}
	start_flag = false;
	uint16_t distance_cm = echo_us * 17 / 1000;
	ruler_disp(distance_cm);

}

void ruler_ISR(void){
	HCSR04_echoISR();
}

