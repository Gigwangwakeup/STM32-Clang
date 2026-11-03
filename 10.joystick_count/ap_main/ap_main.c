/*
 * ap_main.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

#include "stdint.h"
#include "stdio.h"
#include "ap_main.h"
#include "../driver/cds/cds.h"
#include "../driver/lcd/lcd.h"

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;

volatile uint8_t adcCpltFlag = 0;
//volatile uint16_t value[ADC_CH_COUNT] = {0};
//volatile uint8_t adcIndex = 0;

uint16_t x= 0;
uint16_t y= 0;
int x_count = 0;
int y_count = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	adcCpltFlag = 1;
}

void ap_init(void){
	cds_init();
	Lcd_init(&hi2c1);
}

void x_count_upDown(void){
	static uint32_t prevXtime = 0;

	if ((x <= 1000)  && ((HAL_GetTick() - prevXtime) > 100)){
		prevXtime = HAL_GetTick();
		x_count --;
	} else if((1000 < x) && (x < 2000)  && ((HAL_GetTick() - prevXtime) > 1000)){
		prevXtime = HAL_GetTick();
		x_count --;
	} else if((2100 < x) && (x < 3100)  && ((HAL_GetTick() - prevXtime) > 1000)){
		prevXtime = HAL_GetTick();
		x_count ++;
	} else if ((3100 <= x) && ((HAL_GetTick() - prevXtime) > 100)){
		prevXtime = HAL_GetTick();
		x_count ++;
	}
}

void y_count_upDown(void){
	static uint32_t prevYtime = 0;

	if ((y <= 1000)  && ((HAL_GetTick() - prevYtime) > 100)){
		prevYtime = HAL_GetTick();
		y_count ++;
	} else if((1000 < y) && (y < 2000)  && ((HAL_GetTick() - prevYtime) > 1000)){
		prevYtime = HAL_GetTick();
		y_count ++;
	} else if((2100 < y) && (y < 3100)  && ((HAL_GetTick() - prevYtime) > 1000)){
		prevYtime = HAL_GetTick();
		y_count --;
	} else if ((3100 <= y) && ((HAL_GetTick() - prevYtime) > 100)){
		prevYtime = HAL_GetTick();
		y_count --;
	}
}

int ap_main (void){

	ap_init();
	char x_buff[30];
	char y_buff[30];

	while(1){
		if(adcCpltFlag == 1){
			adcCpltFlag = 0;
			x = cds_getValue(2);
			y = cds_getValue(3);
			x_count_upDown();
			y_count_upDown();
			sprintf(x_buff,"x:%04d",x_count);
			sprintf(y_buff,"y:%04d",y_count);
		}
		Lcd_writeStringXY(0, 0, x_buff);
		Lcd_writeStringXY(1, 0, y_buff);
//		HAL_Delay(1000);
	}
	return 0;
}
