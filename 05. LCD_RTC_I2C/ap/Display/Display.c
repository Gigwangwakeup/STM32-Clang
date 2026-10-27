/*
 * Display.c
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

#include "Display.h"
#include "../../driver/fnd/fnd.h"
#include "../../driver/led/led.h"
#include "../../driver/lcd/lcd.h"
#include "stdio.h"
#include "string.h"

enum {TIMECLOCK_MODE, STOPWATCH_MODE};
enum {STOPWATCH_RUN, STOPWATCH_STOP};

extern I2C_HandleTypeDef hi2c1;


static stopWatchTime_t *pStopWatchTime;
static TimeClockTime_t *pTimeClockTime;
static timeClockDate_t *pTimeClockDate;

static led_t ledTimeClock, ledStopWatch;
static led_t ledStopWatchRun, ledStopWatchStop;

void Disp_init(void){
	led_init(&ledTimeClock, GPIOB, GPIO_PIN_15);
	led_init(&ledStopWatch, GPIOB, GPIO_PIN_14);
	led_init(&ledStopWatchStop, GPIOB, GPIO_PIN_13);
	led_init(&ledStopWatchRun, GPIOC, GPIO_PIN_4);
	Lcd_init(&hi2c1);
}

void Disp_setMode(int mode){
	if(mode == TIMECLOCK_MODE){
		led_on(&ledTimeClock);
		led_off(&ledStopWatch);
		led_off(&ledStopWatchStop);
		led_off(&ledStopWatchRun);
	}
	else {
		led_off(&ledTimeClock);
		led_on(&ledStopWatch);
	}
}
void Disp_setTimeClockTime(TimeClockTime_t *pTime){
	pTimeClockTime = pTime;
	Disp_TimeClockTime();
}

void Disp_TimeClockTime(void){
	uint16_t num;
	num = (pTimeClockTime->min * 100) + pTimeClockTime->sec;
	fnd_set_num(num);
	if (pTimeClockTime->msec < 50) {
		fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	}
	fnd_set_dp(FND_DIGIT_1, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);
}

void Disp_TimeClockLcd(void){
	uint8_t hour = pTimeClockTime->hour;
	uint8_t min = pTimeClockTime->min;
	uint8_t sec = pTimeClockTime->sec;
	uint8_t msec = pTimeClockTime->msec;

	char TimeClockBuff[30];
	sprintf(TimeClockBuff,"Time:%02d:%02d:%02d:%02d",hour,min,sec,msec);
	Lcd_writeStringXY(0, 0, TimeClockBuff);
}

void Disp_setStopWatchTime(stopWatchTime_t *pTime){
	pStopWatchTime = pTime;
	Disp_stopWatchTime();
	Disp_stopWatchLcd();
}

void Disp_stopWatchTime(void){
	uint16_t num;
	num = ((pStopWatchTime->min % 10) * 1000)+(pStopWatchTime->sec * 10) + pStopWatchTime->msec / 10 %10;
	fnd_set_num(num);
	if (pStopWatchTime->msec < 50) {
		fnd_set_dp(FND_DIGIT_3, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	}
	fnd_set_dp(FND_DIGIT_1, FND_DP_ON);
	fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);

}

void Disp_stopWatchLcd(void){
	uint8_t hour = pStopWatchTime->hour;
	uint8_t min = pStopWatchTime->min;
	uint8_t sec = pStopWatchTime->sec;
	uint8_t msec = pStopWatchTime->msec;

	char TimeClockBuff[64];
	sprintf(TimeClockBuff,"SW  :%02d:%02d:%02d:%02d",hour,min,sec,msec);
	Lcd_writeStringXY(1, 0, TimeClockBuff);
}

void Disp_setStopWatchState(int state){
	if(state == STOPWATCH_RUN){
		led_on(&ledStopWatchRun);
		led_off(&ledStopWatchStop);
	}
	else {
		led_off(&ledStopWatchRun);
		led_on(&ledStopWatchStop);
	}
}

void Disp_TimeClockDate() {
	char datestr[30];
	const char *strDay[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	sprintf(datestr,"20%02d-%02d-%02d [%s]",
			pTimeClockDate->year, pTimeClockDate->month,
			pTimeClockDate->date, strDay[pTimeClockDate->day-1]);
	Lcd_writeStringXY(1, 0, datestr);
}

void Disp_setTimeClockDate(timeClockDate_t *pDate){
	pTimeClockDate = pDate;
	Disp_TimeClockDate();
}

void Disp_ISR(void){
	fnd_show_number();
}
