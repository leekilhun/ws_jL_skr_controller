/*
 * led.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */




#include "led.h"


typedef struct
{
  GPIO_TypeDef  *port;
  uint16_t       pin;
  GPIO_PinState  on_state;
  GPIO_PinState  off_state;
} led_tbl_t;



static const led_tbl_t led_tbl[LED_MAX_CH] =
    {
        {GPIOD, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET},
    };





bool ledInit(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure = {0,};


  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;

  for (int i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStructure.Pin = led_tbl[i].pin;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStructure);

    ledOff(i);
  }

  return true;
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}
