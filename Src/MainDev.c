/**
  ******************************************************************************
  * @file		src/MainDev.c
  * @author		Wojciech Obrebski
  * @brief		Main Device - methods implementation.
  ******************************************************************************
  * @par			...
  *
  ******************************************************************************
  */

////////////////////////////////// Includes ////////////////////////////////////
#include "MainDev.h"
#include <stddef.h>

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
/////////////////////////// Definitions (in module) ////////////////////////////
///////////////// Extern variables and functions declaration  //////////////////
void Error_Handler(uint8_t *file, uint32_t line);
//////////////// Private variables definitions (&declarations) /////////////////
/////////////////////// Private functions declarations /////////////////////////
static void prvTask(void *pvParameters);
static void prvTask2(void *pvParameters);
static void prvTask3(void *pvParameters);
static void initGPIO(void);
static void deinitGPIO(void);
//////////////////////////// Functions definitions /////////////////////////////

/**
  * @brief		MainDev initialization function.
  * @param[in]  this: pointer to \ref MainDev object;
  * @retval 	None
  */
void MainDev_Init(MainDev* this)
{
	this->TickCnt = 0;
	this->killTask = 0;
	xTaskCreate(prvTask, (const char*)"MDevTask", FRTOS_CFG_MAINDEV_SDEPTH, (void*)this, FRTOS_CFG_MAINDEV_PRIORITY, &this->xTask);
	if(this->xTask == NULL)
		Error_Handler((uint8_t*)__FILE__, __LINE__);
	xTaskCreate(prvTask2, (const char*)"MDevTask", FRTOS_CFG_MAINDEV_SDEPTH, (void*)this, 1, &this->xTask);
	if(this->xTask == NULL)
		Error_Handler((uint8_t*)__FILE__, __LINE__);
	xTaskCreate(prvTask3, (const char*)"MDevTask", FRTOS_CFG_MAINDEV_SDEPTH, (void*)this, 1, &this->xTask);
	if(this->xTask == NULL)
		Error_Handler((uint8_t*)__FILE__, __LINE__);
}


/**
  * @brief		MainDev deinitialization function.
	* For MainDev it is only template because it will be never call
  * @param[in]  this: pointer to \ref MainDev object;
  * @retval 	None
  */
void MainDev_DeInit(MainDev* this)
{
	this->killTask = 1;
	vTaskDelay(200 / portTICK_RATE_MS);
	if(this->xTask)
		vTaskDelete(this->xTask);
}


/**
  * @brief  		Main task function.
  * @param[in]  pvParameters: pointer to parent;
  * @retval 		None
  */
static void prvTask(void *pvParameters)
{
	MainDev* this = (MainDev*) pvParameters;
	initGPIO();

	while(!this->killTask)
	{
		vTaskDelay(1000 / portTICK_RATE_MS);
		//LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
		LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_7);
		//LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_14);
	}
	deinitGPIO();
	this->xTask = NULL;
	vTaskDelete( NULL );
}

static void prvTask2(void *pvParameters)
{
	MainDev* this = (MainDev*) pvParameters;
	initGPIO();

	while(!this->killTask)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
	}
	deinitGPIO();
	this->xTask = NULL;
	vTaskDelete( NULL );
}

static void prvTask3(void *pvParameters)
{
	MainDev* this = (MainDev*) pvParameters;
	initGPIO();

	while(!this->killTask)
	{
		vTaskDelay(1500 / portTICK_RATE_MS);
		LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_14);
	}
	deinitGPIO();
	this->xTask = NULL;
	vTaskDelete( NULL );
}

static void initGPIO(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_0);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);

	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_0);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);

	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_14, LL_GPIO_AF_0);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_14, LL_GPIO_PULL_NO);

}
static void deinitGPIO(void)
{
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_ANALOG);
}
