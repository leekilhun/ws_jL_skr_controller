/*
 * hw.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "hw.h"

__attribute__((section(".noinit"))) fault_log_t fault_log;





bool hwInit(void)
{
  bool ret = true;

#ifndef _USE_HW_RTOS
  ret &= bspInit();
#endif

#ifdef _USE_HW_CLI
  ret &= cliInit();
#endif

  ret &= ledInit();

#ifdef _USE_HW_GPIO
  ret &= gpioInit();
#endif

#ifdef _USE_HW_TIM
  ret &= timInit();
#endif

#ifdef _USE_HW_CODE_IDSW
  ret &= codeIDsw_Init();
#endif

#ifdef _USE_HW_BUZZER
  ret &= buzzerInit();
#endif

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif
#ifdef _USE_HW_LOG
  ret &= logInit();
  logOpen(HW_LOG_CH, 115200);

  logPrintf("[ fw Begin... ]\r\n");

#else // use small size log func
  uartOpen(HW_LOG_CH, 115200);
  uartPrintf(HW_LOG_CH, "[ fw Begin... ]\r\n");
#endif



#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif


#ifdef _USE_HW_CAN
  ret &= canInit();
#endif

#ifdef _USE_HW_SPI
  ret = spiInit();
  if (!ret)
  {
    Error_Handler();
  }
#endif

#ifdef _USE_HW_SD
  if (sdInit() == true)
  {
    ret = fatfsInit();
    if (!ret)
    {
      Error_Handler();
    }
  }
#endif



  if (fault_log.magic_number == 0x5555AAAA)
  {
    fault_log.magic_number = 0;

    logPrintf("Fault Message\n");
    logPrintf("  Type : %d\n",     fault_log.type);
    logPrintf("  R0   : 0x%08X\n", fault_log.REG_R0);
    logPrintf("  R1   : 0x%08X\n", fault_log.REG_R1);
    logPrintf("  R2   : 0x%08X\n", fault_log.REG_R2);
    logPrintf("  R3   : 0x%08X\n", fault_log.REG_R3);
    logPrintf("  R12  : 0x%08X\n", fault_log.REG_R12);
    logPrintf("  LR   : 0x%08X\n", fault_log.REG_LR);
    logPrintf("  PC   : 0x%08X\n", fault_log.REG_PC);
    logPrintf("  PSR  : 0x%08X\n", fault_log.REG_PSR);
  }

  return ret;
}



