/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"

static void apSdUpdate (void);

void apInit(void)
{

  if (usbInit())
  {
    usbBegin(USB_CDC_MODE);
  }

  cliOpen(_DEF_UART1, 115200);   // USB CDC
}


void apMain(void)
{
	uint32_t pre_main_ms = millis();

	while (1)
	{

		if (millis() - pre_main_ms >= 500)
		{
		  pre_main_ms = millis();

			ledToggle(_DEF_LED1);
		}

		cliMain();
		apSdUpdate();
	}

}


void apSdUpdate (void)
{
  sd_state_t sd_state;

    sd_state = sdUpdate();
    if (sd_state == SDCARD_CONNECTED)
    {
      logPrintf("sdCard connected\r\n");
    }
    if (sd_state == SDCARD_DISCONNECTED)
    {
      logPrintf("sdCard disconnected\r\n");
    }

}






