/*
 * cds.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

#include "cds.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_CH_COUNT	4

volatile uint16_t value[ADC_CH_COUNT] = {0};
//uint16_t pout[4];
//uint16_t vout[4];

void cds_init(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)value, ADC_CH_COUNT);
}

uint16_t cds_getValue(uint8_t ch){
	return value[ch];
}


//uint16_t cds_getValue3(void){
//	return value[3];
//}
//uint16_t cds_getValue4(void){
//	return value[4];
//}

//void cds_task(void){
//
//}
