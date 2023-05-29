/*
 * hw.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#include "gpio.h"
#include "led.h"

#include "cdc.h"
#include "usb.h"

#include "uart.h"
#include "cli.h"
#include "i2c.h"
#include "log.h"
#include "spi.h"
#include "sd.h"
#include "fatfs.h"
#include "tim.h"


bool hwInit(void);

#ifdef __cplusplus
}
#endif
