/*
 * UARTDrv.c
 *
 *  Created on: 22 kwi 2023
 *      Author: Dell
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"
#include <string.h>

typedef struct {
    UART_HandleTypeDef *huart;
    SemaphoreHandle_t sem;
    QueueHandle_t rx_queue;
    uint8_t *rx_buff;
    uint16_t rx_size;
    uint16_t rx_count;
} USARTDrv;

void USARTDrv_Init(USARTDrv *this, UART_HandleTypeDef *huart, uint8_t *rx_buff, uint16_t rx_size) {
    this->huart = huart;
    this->sem = xSemaphoreCreateMutex();
    this->rx_queue = xQueueCreate(rx_size, sizeof(uint8_t));
    this->rx_buff = rx_buff;
    this->rx_size = rx_size;
    this->rx_count = 0;
}

void USARTDrv_Transmit(USARTDrv *this, uint8_t *pData, uint8_t dataSize) {
    HAL_StatusTypeDef status;
    uint32_t timeout = pdMS_TO_TICKS(100);

    xSemaphoreTake(this->sem, portMAX_DELAY);
    status = HAL_UART_Transmit_DMA(this->huart, pData, dataSize);
    xSemaphoreGive(this->sem);

    if (status != HAL_OK) {
        return;
    }

    if (xSemaphoreTake(this->sem, timeout) == pdTRUE) {
        xSemaphoreGive(this->sem);
    } else {
        HAL_UART_DMAStop(this->huart);
    }
}

void USARTDrv_Receive_DMA(USARTDrv *this) {
    xSemaphoreTake(this->sem, portMAX_DELAY);
    HAL_UART_Receive_DMA(this->huart, this->rx_buff, this->rx_size);
    xSemaphoreGive(this->sem);
}

uint16_t USARTDrv_GetRxCount(USARTDrv *this) {
    return this->rx_count;
}

uint8_t USARTDrv_Read(USARTDrv *this) {
    uint8_t data = 0;

    if (xQueueReceive(this->rx_queue, &data, 0) == pdTRUE) {
        this->rx_count--;
    }

    return data;
}

void USARTDrv_WriteString(USARTDrv *this, const char *str) {
    USARTDrv_Transmit(this, (uint8_t*)str, strlen(str));
}

void USARTDrv_DeInit(USARTDrv *this) {
HAL_UART_DMAStop(this->huart);
HAL_UART_AbortReceive_IT(this->huart);

vSemaphoreDelete(this->sem);
vQueueDelete(this->rx_queue);

this->huart = NULL;
this->rx_buff = NULL;
this->rx_size = 0;
this->rx_count = 0;
}

