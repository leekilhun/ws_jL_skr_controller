/*
 * tim.c
 *
 *  Created on: 2023. 2. 28.
 *      Author: gns2l
 */



#include "tim.h"
#include "cli.h"


#ifdef _USE_HW_TIM

TIM_HandleTypeDef htim3;

static TIM_HandleTypeDef *p_tim_handle[TIM_MAX_CH] =
    {
        &htim3
    };




tim_tbl_t  tim_tbl[TIM_MAX_CH];


#ifdef _USE_HW_CLI
static void cliTimer(cli_args_t *args);
static void timerTestISR(void);
#endif

bool timInit(void)
{
  for (int i=0; i<TIM_MAX_CH; i++)
  {
    tim_tbl[i].pHandle = NULL;
    tim_tbl[i].func_callback = NULL;
    tim_tbl[i].func_cb = NULL;
    tim_tbl[i].obj = NULL;
    tim_tbl[i].act_ch_bit = 0x00;
    tim_tbl[i].is_start = false;
    tim_tbl[i].sConfig.OCFastMode = 0;
    tim_tbl[i].sConfig.OCIdleState = 0;
    tim_tbl[i].sConfig.OCMode = 0;
    tim_tbl[i].sConfig.OCNIdleState = 0;
    tim_tbl[i].sConfig.OCNPolarity = 0;
    tim_tbl[i].sConfig.Pulse = 0;
    tim_tbl[i].channel[0] = TIM_CHANNEL_1;
    tim_tbl[i].channel[1] = TIM_CHANNEL_2;
    tim_tbl[i].channel[2] = TIM_CHANNEL_3;
    tim_tbl[i].channel[3] = TIM_CHANNEL_4;

  }

#ifdef _USE_HW_CLI
cliAdd("timer", cliTimer);
#endif

  return true;
}


tim_tbl_t * timGetData(uint8_t ch)
{
  return &tim_tbl[ch];
}


bool timStart(uint8_t ch)
{
  bool ret = true;
  tim_tbl_t *p_tim;

  p_tim           = &tim_tbl[ch];
  p_tim->pHandle  = p_tim_handle[ch];

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  switch (ch)
  {
    case _DEF_TIM1:
      /* timer clock interval 1us, initial event time 1ms  */
      p_tim->pHandle->Instance                = TIM3;
      p_tim->pHandle->Init.Prescaler          = (uint32_t)(SystemCoreClock/1000000)-1;
      p_tim->pHandle->Init.CounterMode        = TIM_COUNTERMODE_UP;
      p_tim->pHandle->Init.Period             = (1000-1);
      p_tim->pHandle->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
      p_tim->pHandle->Init.AutoReloadPreload  = TIM_AUTORELOAD_PRELOAD_ENABLE;
      if (HAL_TIM_Base_Init(p_tim->pHandle) != HAL_OK)
      {
        ret = false;
      }

      sMasterConfig.MasterOutputTrigger       = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode           = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(p_tim->pHandle, &sMasterConfig) != HAL_OK)
      {
        ret = false;
      }

      break;




  }


  p_tim->is_start = true;
  return ret;
}


void timAttachCallBackFunc(uint8_t ch, void (*func)())
{
  tim_tbl[ch].func_callback = func;
}

void timAttachCB (uint8_t ch, void* obj, void (*func_cb)(void*, void*, void*))
{
	tim_tbl[ch].obj = obj;
	tim_tbl[ch].func_cb = func_cb;
}

void timEnableISR(uint8_t ch)
{

  if (tim_tbl[ch].is_start == false)
    return;

  switch (ch)
  {
    case _DEF_TIM1:
      if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
      {
        /* Error */
      }
      break;
  }
}

void timDisableISR(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  switch (ch)
  {
    case _DEF_TIM1:
      if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK)
      {
        /* Error */
      }
      break;
  }
}


void timResetCnt(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  switch (ch)
  {
    case _DEF_TIM1:
      htim3.Instance->CR1 |= 1;
      htim3.Instance->CNT = 0;
      break;
  }
}


bool timChangePrescale(uint8_t ch,uint32_t psc)
{
  bool ret = true;
  if (tim_tbl[ch].is_start == false)
    return false;

  switch (ch)
  {
    case _DEF_TIM1:
      timDisableISR(_DEF_TIM1);
      htim3.Instance->PSC = psc;
      timEnableISR(_DEF_TIM1);
      break;

    default:
      ret = false;
      break;
  }

  return ret;
}


bool timChangeAutoReloadRegister(uint8_t ch,uint32_t arr)
{
  bool ret = true;
  if (tim_tbl[ch].is_start == false)
    return false;

  switch (ch)
  {
    case _DEF_TIM1:
      timDisableISR(_DEF_TIM1);
      htim3.Instance->ARR = arr;
      timEnableISR(_DEF_TIM1);
      break;


    default:
      ret = false;
      break;
  }

  return ret;
}



/**
  * @brief This function handles TIM2 global interrupt.
  */

void TIM3_TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

#ifdef _USE_HW_RTOS
  HAL_IncTick();
#endif

  if(htim->Instance==TIM3)
  {
  	if (tim_tbl[_DEF_TIM1].func_callback != NULL )
  	{
  		(*tim_tbl[_DEF_TIM1].func_callback)();
  	}
  	if (tim_tbl[_DEF_TIM1].func_cb != NULL )
  	{
  		(*tim_tbl[_DEF_TIM1].func_cb)(tim_tbl[_DEF_TIM1].obj, NULL, NULL);
  	}
  }

}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_TIM4_IRQn);
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_TIM4_IRQn);
  }

}




#ifdef _USE_HW_CLI


void timerTestGpio(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void timerTestISR(void)
{
  LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_0);
}



void cliTimer(cli_args_t *args)
{
  bool ret = true;
  uint8_t  ch;
  uint32_t regist_val;

  if (args->argc == 3)
  {
    ch  = (uint8_t)args->getData(1);
    regist_val = (uint16_t)args->getData(2);

    ch = constrain(ch, 0, TIM_MAX_CH);

    if(args->isStr(0, "set_PSC"))
    {
      timChangePrescale(ch, regist_val);
      cliPrintf("set PSC ch%d %d\n", ch, regist_val);
    }
    else if(args->isStr(0, "set_ARR"))
    {
      timChangeAutoReloadRegister(ch, regist_val);
      cliPrintf("set ARR ch%d %d\n", ch, regist_val);
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 2)
  {
    ch = (uint8_t)args->getData(1);

    if(args->isStr(0, "start"))
    {
      timStart(ch);
      timAttachCallBackFunc(ch,timerTestISR);
      cliPrintf("timer ch%d start\n", ch);
    }
    else if(args->isStr(0, "enableISR"))
    {
      timerTestGpio();
      timEnableISR(ch);
      cliPrintf("timer ch%d ISR Enable\n", ch);
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 1)
  {
    if(args->isStr(0, "disableISR"))
    {
      timDisableISR(_DEF_TIM1);
      timDisableISR(_DEF_TIM2);
      timDisableISR(_DEF_TIM3);
      timDisableISR(_DEF_TIM4);
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "timer start 0~%d \n", TIM_MAX_CH-1);
    cliPrintf( "timer enableISR 0~%d \n", TIM_MAX_CH-1);
    cliPrintf( "timer set_PSC 0~%d data[1 ~ 65534] \n", TIM_MAX_CH-1);
    cliPrintf( "timer set_ARR 0~%d data[1 ~ 65534] \n", TIM_MAX_CH-1);
    cliPrintf( "timer disableISR\n");
  }

}
#endif

#endif /*_USE_HW_TIM*/
