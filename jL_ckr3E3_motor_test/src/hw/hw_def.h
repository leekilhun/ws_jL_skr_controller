/*
 * hw_def.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "bsp.h"


/****************************************************
   boot/ firmware memory information
 ****************************************************/
#define FLASH_ADDR_BOOT_VER         0x08000400
#define FLASH_ADDR_TAG              0x08005000
#define FLASH_ADDR_FW               0x08005400
#define FLASH_ADDR_FW_VER           0x08005800
/* ------------------------------------------------- */
//#define _USE_HW_RTOS  ->

#ifdef _USE_HW_RTOS
#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)

#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal//osPriorityRealtime
#define _HW_DEF_RTOS_THREAD_PRI_CLI           osPriorityNormal

#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CLI           _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#endif

//#d








#define _USE_HW_SD
#define _USE_HW_FATFS


#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         8
#define        _GPIO_SD_DETECT      0
#define        _GPIO_SD_CS          1
#define        _GPIO_MOT_X_ENABLE   2
#define        _GPIO_MOT_X_STEP     3
#define        _GPIO_MOT_X_DIR      4
#define        _GPIO_MOT_Y_ENABLE   5
#define        _GPIO_MOT_Y_STEP     6
#define        _GPIO_MOT_Y_DIR      7


#define _USE_HW_CDC
#define _USE_HW_USB
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0

#define _USE_HW_UART
#define      HW_UART_MAX_CH         4


#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    1024

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

//#define _USE_HW_CAN
#define      HW_CAN_MAX_CH          1
#define      HW_CAN_MSG_RX_BUF_MAX  16

#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          1
#define        _SPI_SDCARD          _DEF_SPI1
//#define        _SPI_SLAVE           _DEF_SPI2


#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          2
#define        _I2C_EEPROM          _DEF_I2C1
#define        _I2C_EXT_IO          _DEF_I2C2

#define _USE_HW_TIM
#ifdef  _USE_HW_TIM
#define      HW_TIM_MAX_CH          1
#define        _TIM_3               0
#endif
#endif /* SRC_HW_HW_DEF_H_ */
