/*
 * Watch.h
 *
 *  Created on: 2026. 10. 18.
 *      Author: kccistc
 */

#ifndef SRC_AP_WATCH_WATCH_H_
#define SRC_AP_WATCH_WATCH_H_

#include "stm32f4xx_hal.h"

void Watch_init(void);
void Watch_task(void);
void Watch_tick(void);
void Watch_ledoff(void);
void Watch_ledon(void);

#endif /* SRC_AP_WATCH_WATCH_H_ */
