/*
 * motor_task.c
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

#include "motor_task.h"
#include "../../../driver/dc_motor/dc_motor.h"
#include "../../../driver/lcd/lcd.h"
#include "../../message.h"
#include "../distance_task/distance_task.h"

extern TIM_HandleTypeDef htim3;

typedef enum {MODE_STICK, MODE_AUTO} drive_mode_t;
typedef enum {STOP, FORWARD, FW_LEFT, FW_RIGHT, LEFT, RIGHT, BACKWARD} motor_state_t;

static motor_t motor_l, motor_r;
static drive_mode_t drive_mode = MODE_STICK;
static motor_state_t motor_state = STOP;

static int lcd_id = -1; // debugging

//static btn_status_t btn_status = BTN_NO;
//static comm_status_t comm_status = COMM_NO;
static comm_data_t comm_data_dummy = {0};
static comm_data_t *pcomm_data;

void motor_task_init(void)
{
	motor_init(&motor_r, &htim3, TIM_CHANNEL_3, GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6);
	motor_init(&motor_l, &htim3, TIM_CHANNEL_1, GPIOB, GPIO_PIN_6, GPIOA, GPIO_PIN_7);
	motor_set_speed(900, 900);
}

static void stick_motor_controller(void)
{
	switch(motor_state)
	{
	case STOP:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != STOP) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = STOP;
			Lcd_writeStringXY(0, 0, "Motor State STOP");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_stop(&motor_l, &motor_r);
		break;
	case FORWARD:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != FORWARD) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = FORWARD;
			Lcd_writeStringXY(0, 0, "Motor State FW  ");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_forward(&motor_l, &motor_r);
		break;
	case FW_LEFT:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != FW_LEFT) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = FW_LEFT;
			Lcd_writeStringXY(0, 0, "Motor State FL  ");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_forward_left(&motor_l, &motor_r);
		break;
	case FW_RIGHT:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != FW_RIGHT) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = FW_RIGHT;
			Lcd_writeStringXY(0, 0, "Motor State FR  ");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_forward_right(&motor_l, &motor_r);
		break;
	case LEFT:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != LEFT) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = LEFT;
			Lcd_writeStringXY(0, 0, "Motor State LEFT");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_left(&motor_l, &motor_r);
		break;
	case RIGHT:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_FWLEFT)){
			motor_state = FW_LEFT;
		}
		else if((pcomm_data->status == COMM_FWRIGHT)){
			motor_state = FW_RIGHT;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != RIGHT) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = RIGHT;
			Lcd_writeStringXY(0, 0, "Motor State RGT ");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_right(&motor_l, &motor_r);
		break;
	case BACKWARD:
		if((pcomm_data->status == COMM_STOP)){
			motor_state = STOP;
		}
		else if((pcomm_data->status == COMM_FORWARD)){
			motor_state = FORWARD;
		}
		else if((pcomm_data->status == COMM_LEFT)){
			motor_state = LEFT;
		}
		else if((pcomm_data->status == COMM_RIGHT)){
			motor_state = RIGHT;
		}
		else if((pcomm_data->status == COMM_BACKWARD)){
			motor_state = BACKWARD;
		}
		if((lcd_id != BACKWARD) && (osMutexWait(lcdMutexHandle, osWaitForever) == osOK)){
			lcd_id = BACKWARD;
			Lcd_writeStringXY(0, 0, "Motor State BW  ");
			osMutexRelease(lcdMutexHandle); //unlock
		}
		motor_backward(&motor_l, &motor_r);
		break;
	default:
		motor_stop(&motor_l, &motor_r);
		break;
	}
}

static void auto_motor_controller(void)
{
   // ===== 튜닝값 =====
   const uint16_t base        = 400;   // 기본 속도 (최대 499)
   const uint16_t deadband    = 7;     // 이 이하의 좌우 차이는 센서 잡음으로 보고 무시
   const uint16_t side_full   = 30;    // 잡음을 뺀 좌우 차이가 이만큼(cm)이면 최대로 꺾음
   const uint16_t front_start = 60;    // 앞 벽이 이 거리(cm)부터 꺾기 시작
   const uint16_t front_full  = 35;    // 앞 벽이 이 거리(cm)면 최대로 꺾음
   const uint16_t far_limit   = 300;   // 이 이상은 코스에서 나올 수 없는 값 = 벽에 붙음

   uint16_t diff;    // 좌우 차이
   uint16_t steer;   // 꺾는 세기 (0 ~ base)
   static uint16_t turn_right = 1;   // 1: 오른쪽으로 꺾음, 0: 왼쪽으로 꺾음 (값이 유지됨)
   // 0-1. 앞이 벽에 붙음 -> 정지
   if(cm_center >= far_limit){
      motor_stop(&motor_l, &motor_r);
      return;
   }

   // 0-2. 왼쪽이 벽에 붙음 -> 오른쪽으로 최대
   if(cm_left >= far_limit){
      motor_set_speed(0, base * 2);
      motor_forward(&motor_l, &motor_r);
      return;
   }

   // 0-3. 오른쪽이 벽에 붙음 -> 왼쪽으로 최대
   if(cm_right >= far_limit){
      motor_set_speed(base * 2, 0);
      motor_forward(&motor_l, &motor_r);
      return;
   }

   // 1. 좌우 차이 (큰 쪽 - 작은 쪽)
   if(cm_right >= cm_left){
      diff = cm_right - cm_left;
   }
   else{
      diff = cm_left - cm_right;
   }

   // 2. 잡음만큼 빼기 + 차이가 확실할 때만 방향 결정
   if(diff > deadband){
      diff = diff - deadband;
      if(cm_right >= cm_left){
         turn_right = 1;
      }
      else{
         turn_right = 0;
      }
   }
   else{
      diff = 0;   // 방향(turn_right)은 이전 값 그대로
   }

   // 3. 좌우 차이로 꺾기
   if(diff >= side_full){
      steer = base;                              // 최대
   }
   else{
      steer = base * diff / side_full;           // 차이에 비례
   }

   // 4. 앞 벽이 가까우면 더 꺾기
   if(cm_center <= front_full){
      steer = base;                              // 최대
   }
   else if(cm_center < front_start){
      steer = steer + base * (front_start - cm_center) / (front_start - front_full);
   }

   // 5. 최대를 넘지 않게
   if(steer > base){
      steer = base;
   }

   // 6. 정해진 방향으로 꺾기
   if(turn_right == 1){
      motor_set_speed(base - steer, base + steer);   // 오른쪽으로
   }
   else{
      motor_set_speed(base + steer, base - steer);   // 왼쪽으로
   }
   motor_forward(&motor_l, &motor_r);
}

void motor_control_task(void)
{
//	osEvent btn_evt;
//	btn_evt = osMessageGet(btnQueHandle, 0);
//	if(btn_evt.status == osEventMessage){
//		btn_status = btn_evt.value.v;
//	}
//	else {
//		btn_status = BTN_NO;
//	}

	osEvent comm_evt;
	comm_evt = osMessageGet(commQueueHandle, 0); // wait time is 0 -> nonblocking method
	if(comm_evt.status == osEventMessage){
		pcomm_data = comm_evt.value.p;
	}
	else {
		pcomm_data = &comm_data_dummy;
	}

	if(pcomm_data->status == COMM_SPEED){
		motor_set_speed(pcomm_data->speed, pcomm_data->speed);
	}

	if(pcomm_data->status == COMM_AUTO){
		drive_mode = MODE_AUTO;
		motor_state = STOP;
		motor_stop(&motor_l, &motor_r);
	}
	else if(pcomm_data->status == COMM_STICK){
		drive_mode = MODE_STICK;
		motor_state = STOP;
		motor_stop(&motor_l, &motor_r);
	}

	switch(drive_mode)
	{
	case MODE_STICK:
		stick_motor_controller();
		break;
	case MODE_AUTO:
		auto_motor_controller();
		break;
	}

}
