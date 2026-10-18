/*
 * stopWatch.h
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#ifndef SRC_AP_STOPWATCH_STOPWATCH_H_
#define SRC_AP_STOPWATCH_STOPWATCH_H_

#include "stm32f4xx_hal.h"

void StopWatch_init(void);
void StopWatch_task(void);
void stopWatch_ledoff(void);
void stopWatch_ledon(void);
#endif /* SRC_AP_STOPWATCH_STOPWATCH_H_ */
