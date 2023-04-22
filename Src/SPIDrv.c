/*
 * SPIDrv.c
 *
 *  Created on: 22 kwi 2023
 *      Author: Dell
 *
 *******************************************************************************
 * 			SPI Driver
 *******************************************************************************
 */



#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

typedef struct {
    SPI_HandleTypeDef *hspi;
    SemaphoreHandle_t sem;
} SPIDrv;

void SPIDrv_Init(SPIDrv *this, SPI_HandleTypeDef *hspi) {
    this->hspi = hspi;
    this->sem = xSemaphoreCreateMutex();
}

void SPIDrv_Trans(SPIDrv *this, void (*csFun)(FunctionalState), uint8_t *data_buff, uint32_t number_of_trans) {
    HAL_StatusTypeDef status;
    uint32_t timeout = pdMS_TO_TICKS(100);

    xSemaphoreTake(this->sem, portMAX_DELAY);
    csFun(ENABLE);
    status = HAL_SPI_Transmit_DMA(this->hspi, data_buff, number_of_trans);
    xSemaphoreGive(this->sem);

    if (status != HAL_OK) {
        csFun(DISABLE);
        return;
    }

    if (xSemaphoreTake(this->sem, timeout) == pdTRUE) {
        csFun(DISABLE);
        xSemaphoreGive(this->sem);
    } else {
        HAL_SPI_DMAStop(this->hspi);
        csFun(DISABLE);
    }
}

void SPIDrv_Deinit(SPIDrv *this) {
vSemaphoreDelete(this->sem);
}

void SPIDrv_Receive(SPIDrv *this, void (*csFun)(FunctionalState), uint8_t *rx_buff, uint32_t number_of_trans) {
    HAL_StatusTypeDef status;
    uint32_t timeout = pdMS_TO_TICKS(100);

    xSemaphoreTake(this->sem, portMAX_DELAY);
    csFun(ENABLE);
    status = HAL_SPI_Receive_DMA(this->hspi, rx_buff, number_of_trans);
    xSemaphoreGive(this->sem);

    if (status != HAL_OK) {
        csFun(DISABLE);
        return;
    }

    if (xSemaphoreTake(this->sem, timeout) == pdTRUE) {
        csFun(DISABLE);
        xSemaphoreGive(this->sem);
    } else {
        HAL_SPI_DMAStop(this->hspi);
        csFun(DISABLE);
    }
}




