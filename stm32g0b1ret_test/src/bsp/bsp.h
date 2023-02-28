/*
 * bsp.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

#include "stm32g0xx_hal.h"


  typedef struct
  {
    uint32_t magic_number;
    uint32_t type;

    uint32_t REG_R0;
    uint32_t REG_R1;
    uint32_t REG_R2;
    uint32_t REG_R3;
    uint32_t REG_R12;
    uint32_t REG_LR;
    uint32_t REG_PC;
    uint32_t REG_PSR;

  } fault_log_t;


#ifdef _USE_HW_RTOS
#include "rtos.h"
#endif

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"


	bool bspInit(void);
	void bspDeInit(void);
	void delay(uint32_t ms);
	uint32_t millis(void);
	void Error_Handler(void);
	void logPrintf(const char *fmt, ...);
	void delayUs(uint32_t us);

#if 0
#define LOG_ERROR_(fmt, ...) \
		logPrintf("%s:%d:error: " fmt "%s\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOG_ERROR_(__VA_ARGS__, "")
#else
#define LOG_ERROR(...)
#endif


#ifdef __cplusplus
}
#endif


#endif /* SRC_BSP_BSP_H_ */
