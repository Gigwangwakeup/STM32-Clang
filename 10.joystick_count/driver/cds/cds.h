/*
 * cds.h
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_CDS_CDS_H_
#define SRC_DRIVER_CDS_CDS_H_

#include "stdint.h"
#include "stm32f4xx_hal.h"

void cds_init(void);
uint16_t cds_getValue(uint8_t ch);

//uint16_t cds_getValue3(void);
//uint16_t cds_getValue4(void);

#endif /* SRC_DRIVER_CDS_CDS_H_ */
