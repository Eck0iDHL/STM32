/*
 * SPIDrv.h
 *
 *  Created on: 22 kwi 2023
 *      Author: Dell
 *
 *******************************************************************************
 * 			SPIDrv header file
 *******************************************************************************
 */

#ifndef SPIDRV_H_
#define SPIDRV_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"

typedef struct {
    SPI_HandleTypeDef *hspi;
    SemaphoreHandle_t sem;
} SPIDrv;

void SPIDrv_Init(SPIDrv *this, SPI_HandleTypeDef *hspi);

void SPIDrv_Trans(SPIDrv *this, void (*csFun)(FunctionalState), uint8_t *data_buff, uint32_t number_of_trans);

void SPIDrv_Deinit(SPIDrv *this);

#endif /* SPIDRV_H_ */
