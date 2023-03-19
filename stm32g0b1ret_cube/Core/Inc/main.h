/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_crs.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWR_DET_Pin GPIO_PIN_12
#define PWR_DET_GPIO_Port GPIOC
#define POWER_SUPPLY_ONOFF_Pin GPIO_PIN_13
#define POWER_SUPPLY_ONOFF_GPIO_Port GPIOC
#define E0_STOP_Pin GPIO_PIN_15
#define E0_STOP_GPIO_Port GPIOC
#define X_STOP_Pin GPIO_PIN_0
#define X_STOP_GPIO_Port GPIOC
#define Y_STOP_Pin GPIO_PIN_1
#define Y_STOP_GPIO_Port GPIOC
#define Z_STOP_Pin GPIO_PIN_2
#define Z_STOP_GPIO_Port GPIOC
#define SD_DET_Pin GPIO_PIN_3
#define SD_DET_GPIO_Port GPIOC
#define BL_TOUCH_Pin GPIO_PIN_1
#define BL_TOUCH_GPIO_Port GPIOA
#define CD_CS_Pin GPIO_PIN_4
#define CD_CS_GPIO_Port GPIOA
#define Z_STEP_Pin GPIO_PIN_0
#define Z_STEP_GPIO_Port GPIOB
#define Z_ENABLE_Pin GPIO_PIN_1
#define Z_ENABLE_GPIO_Port GPIOB
#define Y_DIR_Pin GPIO_PIN_2
#define Y_DIR_GPIO_Port GPIOB
#define Y_STEP_Pin GPIO_PIN_10
#define Y_STEP_GPIO_Port GPIOB
#define Y_ENABLE_Pin GPIO_PIN_11
#define Y_ENABLE_GPIO_Port GPIOB
#define X_DIR_Pin GPIO_PIN_12
#define X_DIR_GPIO_Port GPIOB
#define X_STEP_Pin GPIO_PIN_13
#define X_STEP_GPIO_Port GPIOB
#define X_ENABLE_Pin GPIO_PIN_14
#define X_ENABLE_GPIO_Port GPIOB
#define FAN1_PWM_Pin GPIO_PIN_15
#define FAN1_PWM_GPIO_Port GPIOB
#define NEOPIXEL_Pin GPIO_PIN_8
#define NEOPIXEL_GPIO_Port GPIOA
#define FAN0_PWM_Pin GPIO_PIN_6
#define FAN0_PWM_GPIO_Port GPIOC
#define FAN_PWM_Pin GPIO_PIN_7
#define FAN_PWM_GPIO_Port GPIOC
#define STATUS_Pin GPIO_PIN_8
#define STATUS_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_9
#define SPI1_CS_GPIO_Port GPIOD
#define HE0_PWM_Pin GPIO_PIN_8
#define HE0_PWM_GPIO_Port GPIOC
#define BED_PWM_Pin GPIO_PIN_9
#define BED_PWM_GPIO_Port GPIOC
#define E0_ENABLE_Pin GPIO_PIN_1
#define E0_ENABLE_GPIO_Port GPIOD
#define LCD_ENABLE_Pin GPIO_PIN_6
#define LCD_ENABLE_GPIO_Port GPIOD
#define E0_STEP_Pin GPIO_PIN_3
#define E0_STEP_GPIO_Port GPIOB
#define E0_DIR_Pin GPIO_PIN_4
#define E0_DIR_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_5
#define BEEP_GPIO_Port GPIOB
#define A3_Pin GPIO_PIN_8
#define A3_GPIO_Port GPIOB
#define A1_Pin GPIO_PIN_9
#define A1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
