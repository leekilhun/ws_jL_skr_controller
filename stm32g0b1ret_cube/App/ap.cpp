/*
 * ap.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: gns2l
 */


#include "ap.hpp"


void apInit(void)
{
	delay(10);

}


void apMain(void)
{
	uint32_t pre_main_ms = millis();

	while (1)
	{

		if (millis() - pre_main_ms >= 500)
		{
			pre_main_ms = millis();

		}
	}
}


