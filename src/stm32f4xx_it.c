/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @date    21/04/2015 20:01:31
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c3;
extern SPI_HandleTypeDef hspi2;
extern ADC_HandleTypeDef hadc1;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}
/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles RCC global interrupt.
*/
void RCC_IRQHandler(void){}

/**
* @brief This function handles I2C2 error interrupt.
*/
//void I2C3_ER_IRQHandler(void)
//{
//  HAL_I2C_ER_IRQHandler(&hi2c3);
//}
//
///**
//* @brief This function handles I2C1 event interrupt.
//*/
//void I2C3_EV_IRQHandler(void)
//{
//   HAL_I2C_EV_IRQHandler(&hi2c3);
//}
//
//void SPI2_IRQHandler	(void)
//{
//	HAL_SPI_IRQHandler(&hspi2);
//
//}
//
//void ADC_IRQHandler(void)
//{
//  HAL_ADC_IRQHandler(&hadc1);
//}
//
//


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
