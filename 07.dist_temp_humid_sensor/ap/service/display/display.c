/*
 * display.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#include "display.h"
#include "../../../driver/lcd/lcd.h"
#include "../../../driver/HCSR04/HCSR04.h"
#include "../temp_humid/temp_humid.h"
#include "stdio.h"

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim10;
void display_init(void)
{
	HCSR04_init(GPIOC, GPIO_PIN_5, GPIOC, GPIO_PIN_6,&htim10);
//	char buff[30];
	Lcd_init(&hi2c1);
//	sprintf(buff, "Hello STM32!");
//	Lcd_writeStringXY(0, 0, buff);
}

void display_task(void)
{
    static uint32_t prevTime = 0;
    char TH_buf[30];
    char dist_buf[30];
    dht11_data_t thData;
    uint32_t e_us = 0;

    if (HAL_GetTick() - prevTime < 1000) {
        return;
    }
    prevTime = HAL_GetTick();

    if (HCSR04_getResult(&e_us) == HCSR04_OK) {
        sprintf(dist_buf, "Dist:%3lucm", e_us / 58);
        Lcd_writeStringXY(0, 0, dist_buf);
    }

    HCSR04_start();

    TH_getTempHumid(&thData);
    sprintf(TH_buf, "T:%2u.%uC H:%2u.%u%%  ",thData.t_int, thData.t_dec, thData.h_int, thData.h_dec);
    Lcd_writeStringXY(1, 0, TH_buf);
}
