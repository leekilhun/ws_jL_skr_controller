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
#define tmc2209_rx4_Pin GPIO_PIN_11
#define tmc2209_rx4_GPIO_Port GPIOC
#define j7_3_pwr_det_Pin GPIO_PIN_12
#define j7_3_pwr_det_GPIO_Port GPIOC
#define j6_2_ps_on_Pin GPIO_PIN_13
#define j6_2_ps_on_GPIO_Port GPIOC
#define probe_Pin GPIO_PIN_14
#define probe_GPIO_Port GPIOC
#define e0_stop_Pin GPIO_PIN_15
#define e0_stop_GPIO_Port GPIOC
#define x_stop_Pin GPIO_PIN_0
#define x_stop_GPIO_Port GPIOC
#define y_stop_Pin GPIO_PIN_1
#define y_stop_GPIO_Port GPIOC
#define z_stop_Pin GPIO_PIN_2
#define z_stop_GPIO_Port GPIOC
#define sd_det_Pin GPIO_PIN_3
#define sd_det_GPIO_Port GPIOC
#define th0_pwm_Pin GPIO_PIN_0
#define th0_pwm_GPIO_Port GPIOA
#define servos_Pin GPIO_PIN_1
#define servos_GPIO_Port GPIOA
#define p3_3_tx2_Pin GPIO_PIN_2
#define p3_3_tx2_GPIO_Port GPIOA
#define p3_2_rx2_Pin GPIO_PIN_3
#define p3_2_rx2_GPIO_Port GPIOA
#define sdcard_cs_Pin GPIO_PIN_4
#define sdcard_cs_GPIO_Port GPIOA
#define spi1_sck_Pin GPIO_PIN_5
#define spi1_sck_GPIO_Port GPIOA
#define spi1_miso_Pin GPIO_PIN_6
#define spi1_miso_GPIO_Port GPIOA
#define spi1_mosi_Pin GPIO_PIN_7
#define spi1_mosi_GPIO_Port GPIOA
#define z_dir_Pin GPIO_PIN_5
#define z_dir_GPIO_Port GPIOC
#define z_step_Pin GPIO_PIN_0
#define z_step_GPIO_Port GPIOB
#define z_en_Pin GPIO_PIN_1
#define z_en_GPIO_Port GPIOB
#define y_dir_Pin GPIO_PIN_2
#define y_dir_GPIO_Port GPIOB
#define y_step_Pin GPIO_PIN_10
#define y_step_GPIO_Port GPIOB
#define y_en_Pin GPIO_PIN_11
#define y_en_GPIO_Port GPIOB
#define x_dir_Pin GPIO_PIN_12
#define x_dir_GPIO_Port GPIOB
#define x_step_Pin GPIO_PIN_13
#define x_step_GPIO_Port GPIOB
#define x_en_Pin GPIO_PIN_14
#define x_en_GPIO_Port GPIOB
#define fan1_pwm_Pin GPIO_PIN_15
#define fan1_pwm_GPIO_Port GPIOB
#define neo_tim_Pin GPIO_PIN_8
#define neo_tim_GPIO_Port GPIOA
#define j10_8_tx1_Pin GPIO_PIN_9
#define j10_8_tx1_GPIO_Port GPIOA
#define fan0_pwm_Pin GPIO_PIN_6
#define fan0_pwm_GPIO_Port GPIOC
#define fan_pwm_Pin GPIO_PIN_7
#define fan_pwm_GPIO_Port GPIOC
#define status_Pin GPIO_PIN_8
#define status_GPIO_Port GPIOD
#define spi1_cs_Pin GPIO_PIN_9
#define spi1_cs_GPIO_Port GPIOD
#define j10_6_rx1_Pin GPIO_PIN_10
#define j10_6_rx1_GPIO_Port GPIOA
#define j10_9_btn_enc_Pin GPIO_PIN_15
#define j10_9_btn_enc_GPIO_Port GPIOA
#define he0_pwm_Pin GPIO_PIN_8
#define he0_pwm_GPIO_Port GPIOC
#define bed_pwm_Pin GPIO_PIN_9
#define bed_pwm_GPIO_Port GPIOC
#define j13_5_out_Pin GPIO_PIN_0
#define j13_5_out_GPIO_Port GPIOD
#define e0_en_i2c2_int_Pin GPIO_PIN_1
#define e0_en_i2c2_int_GPIO_Port GPIOD
#define j13_4_rx5_Pin GPIO_PIN_2
#define j13_4_rx5_GPIO_Port GPIOD
#define j13_3_tx5_Pin GPIO_PIN_3
#define j13_3_tx5_GPIO_Port GPIOD
#define j13_2_out_Pin GPIO_PIN_4
#define j13_2_out_GPIO_Port GPIOD
#define j13_1_out_Pin GPIO_PIN_5
#define j13_1_out_GPIO_Port GPIOD
#define j10_3_lcd_en_Pin GPIO_PIN_6
#define j10_3_lcd_en_GPIO_Port GPIOD
#define e0stp_i2c2_scl_Pin GPIO_PIN_3
#define e0stp_i2c2_scl_GPIO_Port GPIOB
#define e0dir_i2c2_sda_Pin GPIO_PIN_4
#define e0dir_i2c2_sda_GPIO_Port GPIOB
#define j10_10_beeper_Pin GPIO_PIN_5
#define j10_10_beeper_GPIO_Port GPIOB
#define j10_4_tx6_Pin GPIO_PIN_8
#define j10_4_tx6_GPIO_Port GPIOB
#define j10_5_rx6_Pin GPIO_PIN_9
#define j10_5_rx6_GPIO_Port GPIOB
#define tmc2209_tx4_Pin GPIO_PIN_10
#define tmc2209_tx4_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
