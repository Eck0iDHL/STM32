/**
  ******************************************************************************
  * @file			Src/main.c
  * @author		Wojciech Obrebski
  * @brief		Program entry point.
  ******************************************************************************
  * @par			In entry point system clock will be configure, main devices
	* structures will be initialized.
	* Additionally some helper functions will be defined.
	*
  * @bug ...
  *
  ******************************************************************************
  */

////////////////////////////////// Includes ////////////////////////////////////
#include "MainDev.h"

#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_utils.h"
/////////////////////////// Definitions (in module) ////////////////////////////
///////////////// Extern variables and functions declaration  //////////////////
//////////////// Private variables definitions (&declarations) /////////////////
MainDev MainDevOb;
/////////////////////// Private functions declarations /////////////////////////
static void setUpClock(void);
void Error_Handler(uint8_t *file, uint32_t line);
//////////////////////////// Functions definitions /////////////////////////////

int main(void)
{
	setUpClock();

	MainDev_Init(&MainDevOb);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

/**
  * @brief  This function is executed when FreeRTOS entered to Idle
  * @retval None
  */
void vApplicationIdleHook( void )
{
#ifndef DEBUG
	__WFI(); //set core in sleep
#endif
}

/**
  * @brief  This function is executed in system tick
  * @retval None
  */
void vApplicationTickHook( void )
{
	MainDevOb.TickCnt++;
	if(MainDevOb.TickCnt > 0x7FFFFFF)
		MainDevOb.TickCnt = 0;
}

/**
  * @brief  This function is executed in case malloc error.
  * @retval None
  */
void vApplicationMallocFailedHook(void)
{
#ifdef DEBUG
	__ASM volatile("BKPT #01");
#else
#endif
}

/**
  * @brief  This function is executed in case task stack overflow error.
  * @retval None
  */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
#ifdef DEBUG
	__ASM volatile("BKPT #01");
#else
#endif
}

/**
  * @brief  Configure clocks
  * @retval None
  */

static void setUpClock(void)
{
	/* Enable HSE oscillator */
	LL_RCC_HSE_EnableBypass();
	LL_RCC_HSE_Enable();
	while(LL_RCC_HSE_IsReady() != 1)
	{
	};

	/* Set FLASH latency */
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1)
	{
	};

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	};

	/* Set APB1 & APB2 prescaler */
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);


	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	SystemCoreClockUpdate();

	/* Set systick to 1ms */
	SysTick_Config(SystemCoreClock / 1000);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(uint8_t *file, uint32_t line)
{
#ifdef DEBUG
	__ASM volatile("BKPT #01");
#else
#endif
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

#ifdef DEBUG
	__ASM volatile("BKPT #01");
#else
#endif
}
#endif /*USE_FULL_ASSERT*/
