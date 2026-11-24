/*
 * distance_task.h
 *
 *  Created on: 2026. 11. 13.
 *      Author: kccistc
 */

#ifndef CORE_SRC_AP_TASK_DISTANCE_TASK_DISTANCE_TASK_H_
#define CORE_SRC_AP_TASK_DISTANCE_TASK_DISTANCE_TASK_H_

#include "../../../driver/HCSR04/HCSR04.h"
#include <stdbool.h>

typedef enum {DISTANCE_OK, DISTANCE_IN_PROGRESS, DISTANCE_NOT_READY} distance_status_t;

extern uint16_t cm_center, cm_right, cm_left;

void distance_task_init(void);
void distance_task(void);
void distance_request(hcsr04_t *hcsr04, bool *start_flag);
distance_status_t distance_get(uint32_t echo_us, hcsr04_status_t *hcsr04_status,
							   bool *start_flag, uint16_t *distance_cm);

#endif /* CORE_SRC_AP_TASK_DISTANCE_TASK_DISTANCE_TASK_H_ */
