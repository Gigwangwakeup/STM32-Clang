/*
 * Display.h
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

#ifndef SRC_AP_DISPLAY_DISPLAY_H_
#define SRC_AP_DISPLAY_DISPLAY_H_

#include "stdint.h"
#include "stdio.h"

typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
}stopWatchTime_t;

typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
}TimeClockTime_t;

typedef struct{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t day;
}timeClockDate_t;

void Disp_init(void);
void Disp_setMode(int mode);
void Disp_TimeClockTime(void);
void Disp_setTimeClockTime(TimeClockTime_t *pTime);
void Disp_stopWatchTime(void);
void Disp_setStopWatchTime(stopWatchTime_t *pTime);
void Disp_setStopWatchState(int state);
void Disp_ISR(void);

void Disp_TimeClockLcd(void);
void Disp_stopWatchLcd(void);
void Disp_TimeClockDate();
void Disp_setTimeClockDate(timeClockDate_t *pDate);

#endif /* SRC_AP_DISPLAY_DISPLAY_H_ */
