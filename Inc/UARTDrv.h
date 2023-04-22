/*
 * UARTDrv.h
 *
 *  Created on: 22 kwi 2023
 *      Author: Dell
 */

#ifndef UARTDRV_H_
#define UARTDRV_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"

typedef struct {
UART_HandleTypeDef *huart;
SemaphoreHandle_t sem;
QueueHandle_t rx_queue;
uint8_t *rx_buff;
uint16_t rx_size;
uint16_t rx_count;
} USARTDrv;

void USARTDrv_Init(USARTDrv *this, UART_HandleTypeDef *huart, uint8_t *rx_buff, uint16_t rx_size);
void USARTDrv_Transmit(USARTDrv *this, uint8_t *pData, uint8_t dataSize);
void USARTDrv_Receive_DMA(USARTDrv *this);
void USARTDrv_WriteString(USARTDrv *this, const char *str);
uint8_t USARTDrv_Read(USARTDrv *this);
uint16_t USARTDrv_GetRxCount(USARTDrv *this);
void USARTDrv_DeInit(USARTDrv *this);

#endif /* UARTDRV_H_ */
