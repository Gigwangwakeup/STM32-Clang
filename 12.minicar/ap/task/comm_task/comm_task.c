/*
 * comm_task.c
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

#include "comm_task.h"
#include "usart.h"
#include "../../message.h"
#include "../../../driver/lcd/lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

comm_data_t comm_data;

static uint8_t rx_data = 0;
static uint8_t rx_index = 0;
static uint8_t rx_cplt_flag = 0;

static char rx_buff[32];
static comm_data_t spd_data, mode_data;
static uint8_t auto_on = 0;	// [ADD] current mode for B0 toggle (0 = MANUAL, 1 = AUTO)

void comm_task_init(void)
{
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	HAL_UART_Receive_IT(&huart6, &rx_data, 1);
}

void comm_task(void)
{
	char buff[30];

	if(rx_cplt_flag){
		rx_cplt_flag = 0;
		char cmd[32];
		strcpy(cmd, rx_buff);

		if(cmd[0] == 'J' && cmd[2] == ':'){
			char *p = &cmd[3];
			int   rot = (int)strtol(p, &p, 10);                    // 0~360
			float mag = (*p == ',') ? strtof(p + 1, NULL) : 0.0f;  // 0~1
			comm_status_t dir;

			if(mag < 0.2f)                    dir = COMM_STOP;     // dead zone / release
			else if(rot >= 67  && rot < 113)  dir = COMM_FORWARD;
			else if(rot >= 22  && rot < 67)   dir = COMM_FWRIGHT;
			else if(rot >= 113 && rot < 158)  dir = COMM_FWLEFT;
			else if(rot >= 158 && rot < 203)  dir = COMM_LEFT;
			else if(rot >= 203 && rot < 338)  dir = COMM_BACKWARD;
			else                              dir = COMM_RIGHT;    // 338~360, 0~21

			spd_data.status = COMM_SPEED;
			spd_data.speed  = (int)(mag * 999);                   // magnitude -> speed
			osMessagePut(commQueueHandle, (uint32_t)&spd_data, 10);

			comm_data.status = dir;
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);

			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				sprintf(buff, "J %3d %3d%%      ", rot, (int)(mag * 100));
				Lcd_writeStringXY(1, 0, buff);
				osMutexRelease(lcdMutexHandle); //unlock
			}
			rx_index = 0;
			return;
		}

		if(cmd[0] == 'T' && cmd[2] == ':'){
			mode_data.status = (cmd[3] == '1') ? COMM_AUTO : COMM_STICK;
			auto_on = (mode_data.status == COMM_AUTO);	// [ADD] keep B0 toggle in sync
			osMessagePut(commQueueHandle, (uint32_t)&mode_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, (cmd[3] == '1') ? "MODE AUTO       " : "MODE MANUAL     ");
				osMutexRelease(lcdMutexHandle); //unlock
			}
			rx_index = 0;
			return;
		}
		/* [ADD] Button "B0:D" (press) toggles AUTO <-> MANUAL, "B0:U" (release) ignored */
		if(cmd[0] == 'B' && cmd[1] == '0' && cmd[2] == ':'){
			if(cmd[3] == 'D'){
				auto_on = !auto_on;
				mode_data.status = auto_on ? COMM_AUTO : COMM_STICK;
				osMessagePut(commQueueHandle, (uint32_t)&mode_data, 10);
				if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
					Lcd_writeStringXY(1, 0, auto_on ? "MODE AUTO       " : "MODE MANUAL     ");
					osMutexRelease(lcdMutexHandle); //unlock
				}
			}
			rx_index = 0;
			return;
		}
		/* [ADD] end ----------------------------------------------------------- */
		// Stick mode
		if((strcmp(rx_buff, "AUTO") == 0) || (strcmp(rx_buff, "MANUAL") == 0)) {
			mode_data.status = (rx_buff[0] == 'A') ? COMM_AUTO : COMM_STICK;
			auto_on = (mode_data.status == COMM_AUTO);	// [ADD] keep B0 toggle in sync
			osMessagePut(commQueueHandle, (uint32_t)&mode_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				sprintf(buff, "MODE %-11.11s", rx_buff);
				Lcd_writeStringXY(1, 0, buff);
				osMutexRelease(lcdMutexHandle);
			}
			rx_index = 0;
			return;
		}
		// Use ComportMaster
		// if same, return 0 else not 0(normal 1 or -1)
		if(strncmp(rx_buff, "FORWARD", 3) == 0){
			comm_data.status = COMM_FORWARD;
			//osMessagePut(commQueueHandle, COMM_RUN, 10);
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task FW");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "FW_LEFT", 7) == 0){
			comm_data.status = COMM_FWLEFT;
			//osMessagePut(commQueueHandle, COMM_RUN, 10);
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task FWL");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "FW_RIGHT", 8) == 0){
			comm_data.status = COMM_FWRIGHT;
			//osMessagePut(commQueueHandle, COMM_RUN, 10);
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task FWR");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "STOP", 4) == 0){
			comm_data.status = COMM_STOP;
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task STOP");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "LEFT", 4) == 0){
			comm_data.status = COMM_LEFT;
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task LEFT");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "RIGHT", 5) == 0){
			comm_data.status = COMM_RIGHT;
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task RIGHT");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "BACKWARD", 8) == 0){
			comm_data.status = COMM_BACKWARD;
			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task BW");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else if(strncmp(rx_buff, "SPD", 3) == 0){
			comm_data.status = COMM_SPEED;
			comm_data.speed = atoi(&rx_buff[3]);

			osMessagePut(commQueueHandle, (uint32_t)&comm_data, 10);

			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				sprintf(buff, "CommT SPD %d", comm_data.speed);
				Lcd_writeStringXY(1, 0, buff);
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		else{
			//osMessagePut(commQueueHandle, COMM_NO, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(1, 0, "Comm Task NO");
				osMutexRelease(lcdMutexHandle); //unlock
			}
		}
		rx_index = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// Use comportMaster
	if(huart->Instance == USART2){
		if(rx_data == '*'){
			rx_buff[rx_index++] = '\0'; // \0 => null char and 0 => null
			rx_cplt_flag = 1;
		}
		else{
			rx_buff[rx_index++] = rx_data;
		}

		HAL_UART_Receive_IT(&huart2, &rx_data, 1); // 1->1byte
	}

	// Use Bluetooth
	if(huart->Instance == USART6){
		if(rx_data == '\n'){
			if(rx_index > 0){
				rx_buff[rx_index] = '\0';
				rx_cplt_flag = 1;
			}
			rx_index = 0;
		}
		else if(rx_data != '\r' && rx_index < sizeof(rx_buff) - 1){
			rx_buff[rx_index++] = rx_data;
		}
		HAL_UART_Receive_IT(&huart6, &rx_data, 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	}
	else if(huart->Instance == USART6){
		rx_index = 0;
		HAL_UART_Receive_IT(&huart6, &rx_data, 1);
	}
}
