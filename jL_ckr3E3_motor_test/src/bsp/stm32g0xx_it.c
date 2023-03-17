/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "stm32g0xx_it.h"

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
  while (1)
  {
  }
}

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

