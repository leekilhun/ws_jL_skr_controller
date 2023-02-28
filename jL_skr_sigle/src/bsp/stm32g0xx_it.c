/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "stm32g0xx_it.h"

/* External variables --------------------------------------------------------*/
/*
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim1;
*/

//extern PCD_HandleTypeDef hpcd_USB_DRD_FS;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

extern fault_log_t fault_log;


/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}




/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler_C(uint32_t *p_stack)
{

  fault_log.magic_number = 0x5555AAAA;
  fault_log.type = 0;

  fault_log.REG_R0  = p_stack[0];  // Register R0
  fault_log.REG_R1  = p_stack[1];  // Register R1
  fault_log.REG_R2  = p_stack[2];  // Register R2
  fault_log.REG_R3  = p_stack[3];  // Register R3
  fault_log.REG_R12 = p_stack[4];  // Register R12
  fault_log.REG_LR  = p_stack[5];  // Link register LR
  fault_log.REG_PC  = p_stack[6];  // Program counter PC
  fault_log.REG_PSR = p_stack[7];  // Program status word PSR

  NVIC_SystemReset();

  while (1)
  {
  }
}


/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
#ifndef  _USE_HW_RTOS  //rtos
void SVC_Handler(void)
{
}
#endif
/**
  * @brief This function handles Pendable request for system service.
  */
#ifndef  _USE_HW_RTOS //rtos
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{


#ifndef  _USE_HW_RTOS //rtos
  HAL_IncTick();

#else
  osSystickHandler();
//  swTimerISR();
#endif
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/


void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
#ifdef _USE_HW_UART_1_DMA
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
#endif
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
# ifdef _USE_HW_UART_1_DMA
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
#endif
# ifdef _USE_HW_UART_3_DMA
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
#endif
}







#if 0
/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */

  /* USER CODE END TIM17_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23.
  */
void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  /* USER CODE END I2C1_IRQn 0 */
  if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c1);
  } else {
    HAL_I2C_EV_IRQHandler(&hi2c1);
  }
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 and USART4 interrupts.
  */
void USART3_4_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_4_IRQn 0 */

  /* USER CODE END USART3_4_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN USART3_4_IRQn 1 */

  /* USER CODE END USART3_4_IRQn 1 */
}

#endif
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

