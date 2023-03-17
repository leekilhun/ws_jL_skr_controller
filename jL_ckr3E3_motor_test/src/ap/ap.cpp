/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.hpp"

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif

static void apSdUpdate (void);

MOTOR::enStepMotor motor;

void apInit(void)
{

	MOTOR::enStepMotor::cfg_s cfg{};
	cfg.motor_id = 0;
	cfg.ptr_timer = timGetData(_DEF_TIM1);
	motor.Init(cfg);

	if (usbInit())
	{
		usbBegin(USB_CDC_MODE);
	}

	cliOpen(_DEF_UART1, 115200);   // USB CDC

#ifdef _USE_HW_CLI
cliAdd("app", cliApp);
#endif
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



#ifdef _USE_HW_CLI
void cliApp(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "info") == true)
    {
    	cliPrintf( "motor info \n");
      ret = true;

    }
    else if (args->isStr(0, "test") == true)
    {
    	motor.Enable();
    	motor.test_stepper_constant_accel();
    	motor.Disable();
    	cliPrintf( "motor Test Start \n");
      ret = true;

    }
  }
  else if (args->argc == 2)
  {
    uint8_t axis_id = (uint8_t)args->getData(1);
    if (args->isStr(0, "on") == true)
    {
    	motor.Enable();
      cliPrintf( "motor Axis[%d] On \n", axis_id);
      ret = true;
    }
    else if (args->isStr(0, "off") == true)
    {
    	motor.Disable();
      cliPrintf( "motor Axis[%d] Off \n", axis_id);
      ret = true;
    }

  }
  else if (args->argc == 4)
  {
  	uint8_t id = (uint8_t)args->getData(1);
  	uint32_t speed = (uint32_t)args->getData(2);
  	int step_pulse = (uint32_t)args->getData(3);
  	if (args->isStr(0, "run") == true)
  	{
  		motor.SetStep(step_pulse, speed);
  		motor.RunAndWait();
  		{
  			cliPrintf("ID[%d] run step[%d] speed[%d] : OK\n",id, step_pulse, speed);
  			ret = true;
  		}
  	}
  	if (args->isStr(0, "rel") == true)
  	{
  		motor.SetRel(step_pulse, speed);
  		motor.RunAndWait();
  		{
  			cliPrintf("ID[%d] rel step[%d] speed[%d] : OK\n",id, step_pulse, speed);
  			ret = true;
  		}
  	}


  }


  if (ret == false)
  {
    cliPrintf( "app info \n");
    cliPrintf( "app test \n");
    cliPrintf( "app on [axis] \n");
    cliPrintf( "app off [axis] \n");
    cliPrintf( "app run [axis][100:0 speed][step] \n");
    cliPrintf( "app rel [axis][100:0 speed][step] \n");

  }


}
#endif








