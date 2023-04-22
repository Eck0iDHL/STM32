/**
  ******************************************************************************
  * @file			Inc/MainDev.h
  * @author		Wojciech Obrebski
  * @brief		Main Device.
  ******************************************************************************
  * @par			In this module is declared structure and its initialization
  * function which integrate all functions of this device.
  ******************************************************************************
  */
#ifndef MAINDEV_H_
#define MAINDEV_H_

////////////////////////////////// Includes ////////////////////////////////////
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
////////////////////////////// Public definitions //////////////////////////////
////////////////// Public typedefs and structures declarations /////////////////
//////////////////////// Module structure declaration //////////////////////////

/** @brief Main device structure declaration.
* Contains all device functions structures object.
*/
typedef struct _MainDev{
//public:
	uint32_t	TickCnt;	///<counter incremented on every system tick
//privat:
	xTaskHandle	xTask;		///< handle to main working task
	uint8_t 	killTask;	///< set to 1 to kill working task
}MainDev;

///////////////////// Module public methods declaration ////////////////////////
void MainDev_Init(MainDev* this);
void MainDev_DeInit(MainDev* this);

#endif /*MAINDEV_H_*/
