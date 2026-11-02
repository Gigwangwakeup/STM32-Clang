/*
 * queue.h
 *
 *  Created on: 2026. 11. 2.
 *      Author: kccistc
 */

#ifndef SRC_AP_MAIN_SERVICE_QUE_QUE_H_
#define SRC_AP_MAIN_SERVICE_QUE_QUE_H_

#include "stdbool.h"

typedef enum {
	BTN_FUCNT = 0,
	BTN_CLEAR
} event_btn_t;

typedef enum {
	ACT_PUSHED = 0,
	ACT_RELEASED
} event_btn_act_t;

typedef struct {
	event_btn_t evt_btn;
	event_btn_act_t evt_btn_act;
} btn_event_t;

void que_init(void);

void que_putMode(event_btn_act_t act);
void que_getMode(event_btn_act_t *act);
bool que_isEmptyMode(void);
bool que_isFullMode(void);

void que_putFunct(btn_event_t btn_evt);
void que_getFunct(btn_event_t *btn_evt);
bool que_isEmptyFunct(void);
bool que_isFullFunct(void);

#endif /* SRC_AP_MAIN_SERVICE_QUE_QUE_H_ */
