/*
 * i2c.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */

#include "i2c.h"
#include "cli.h"


#ifdef _USE_HW_I2C

#ifdef _USE_HW_CLI
static void cliI2C(cli_args_t *args);
#endif


#ifdef _USE_HW_RTOS
#define LOCK_BEGIN(x) osMutexWait(mutex_lock[x], osWaitForever)
#define LOCK_END(x)   osMutexRelease(mutex_lock[x])
#define PRIORITY_ORDER   3
#else
#define LOCK_BEGIN(x)
#define LOCK_END(x)
#define PRIORITY_ORDER   0
#endif


static uint32_t i2c_errcount[I2C_MAX_CH];
static i2c_freq_t i2c_freq[I2C_MAX_CH];

static bool is_init = false;
static bool is_open[I2C_MAX_CH];

#ifdef _USE_HW_RTOS
static osMutexId mutex_lock[I2C_MAX_CH];
static const osMutexDef_t mutex_lock_def[I2C_MAX_CH];
#endif


I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;


typedef struct
{
  I2C_HandleTypeDef *p_hi2c;

  GPIO_TypeDef *scl_port;
  int           scl_pin;

  GPIO_TypeDef *sda_port;
  int           sda_pin;
} i2c_tbl_t;

static i2c_tbl_t i2c_tbl[I2C_MAX_CH] =
    {
        { &hi2c1, GPIOB, GPIO_PIN_3,  GPIOB, GPIO_PIN_4},
        { &hi2c2, GPIOB, GPIO_PIN_6,  GPIOB, GPIO_PIN_7}
    };

static const uint32_t i2c_freq_tbl[] =
    {
        0x10707DBC, // 100Khz standard mode
        0x00602173, // 400Khz fast mode
        0x00300B29, // 1000Khz fast plus mode
    };





bool i2cInit(void)
{
  uint32_t i;


  for (i=0; i<I2C_MAX_CH; i++)
  {
    i2c_errcount[i] = 0;
    is_open[i] = false;
#ifdef _USE_HW_RTOS
    mutex_lock[i] = osMutexCreate(&mutex_lock_def[i]);
#endif
  }

#ifdef _USE_HW_CLI
  cliAdd("i2c", cliI2C);
#endif

  is_init = true;
  return true;
}

bool i2cIsInit(void)
{
  return is_init;
}

bool i2cBegin(uint8_t ch, i2c_freq_t freq_khz)
{
  bool ret = false;

  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }


  switch(ch)
  {
    case _DEF_I2C1:
    {
      i2c_freq[ch] = freq_khz;
      is_open[ch] = false;

      p_handle->Instance             = I2C1;
      p_handle->Init.Timing          = i2c_freq_tbl[freq_khz];
      p_handle->Init.OwnAddress1     = 0x00;
      p_handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
      p_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      p_handle->Init.OwnAddress2     = 0x00;
      p_handle->Init.OwnAddress2Masks= I2C_OA2_NOMASK;
      p_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      p_handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

      i2cReset(ch);

      HAL_I2C_DeInit(p_handle);
      if(HAL_I2C_Init(p_handle) == HAL_OK)
      {
        /* Enable the Analog I2C Filter */
        HAL_I2CEx_ConfigAnalogFilter(p_handle, I2C_ANALOGFILTER_ENABLE);

        /* Configure Digital filter */
        HAL_I2CEx_ConfigDigitalFilter(p_handle, 0);

        ret = true;
        is_open[ch] = true;
      }
    }
    break;
    case _DEF_I2C2:
    {
      i2c_freq[ch] = freq_khz;
      is_open[ch] = false;

      p_handle->Instance             = I2C2;
      p_handle->Init.Timing          = i2c_freq_tbl[freq_khz];
      p_handle->Init.OwnAddress1     = 0x00;
      p_handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
      p_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      p_handle->Init.OwnAddress2Masks= I2C_OA2_NOMASK;
      p_handle->Init.OwnAddress2     = 0x00;
      p_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      p_handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

      i2cReset(ch);

      HAL_I2C_DeInit(p_handle);
      if(HAL_I2C_Init(p_handle) == HAL_OK)
      {
        /* Enable the Analog I2C Filter */
        HAL_I2CEx_ConfigAnalogFilter(p_handle,I2C_ANALOGFILTER_ENABLE);

        /* Configure Digital filter */
        HAL_I2CEx_ConfigDigitalFilter(p_handle, 0);

        ret = true;
        is_open[ch] = true;
      }
    }
    break;
  }

  return ret;
}

bool i2cIsOpen(uint8_t ch)
{
  return is_open[ch];
}

void i2cReset(uint8_t ch)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  i2c_tbl_t *p_pin = &i2c_tbl[ch];


  GPIO_InitStruct.Pin       = p_pin->scl_pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(p_pin->scl_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = p_pin->sda_pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(p_pin->sda_port, &GPIO_InitStruct);


  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
  delayUs(5);

  for (int i = 0; i < 9; i++)
  {

    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
    delayUs(5);
    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
    delayUs(5);
  }

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
  delayUs(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_RESET);
  delayUs(5);

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  delayUs(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
}

bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;
  bool ret = false;

  LOCK_BEGIN(ch);
  if (HAL_I2C_IsDeviceReady(p_handle, dev_addr << 1, 10, 10) == HAL_OK)
  {
  	ret = true;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cRecovery(uint8_t ch)
{
  bool ret;

  i2cReset(ch);

  ret = i2cBegin(ch, i2c_freq[ch]);

  return ret;
}

bool i2cReadByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout)
{
  return i2cReadBytes(ch, dev_addr, reg_addr, p_data, 1, timeout);
}

bool i2cReadBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);
  i2c_ret = HAL_I2C_Mem_Read(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, length, timeout);

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cRead16Byte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t timeout)
{
  return i2cRead16Bytes(ch, dev_addr, reg_addr, p_data, 1, timeout);
}

bool i2cRead16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);

  i2c_ret = HAL_I2C_Mem_Read(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, timeout);

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);
  i2c_ret = HAL_I2C_Master_Receive(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);

  if( i2c_ret == HAL_OK )
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cWriteByte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout)
{
  return i2cWriteBytes(ch, dev_addr, reg_addr, &data, 1, timeout);
}

bool i2cWriteBytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);
  i2c_ret = HAL_I2C_Mem_Write(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, length, timeout);

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cWrite16Byte (uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t data, uint32_t timeout)
{
  return i2cWrite16Bytes(ch, dev_addr, reg_addr, &data, 1, timeout);
}

bool i2cWrite16Bytes(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);
  i2c_ret = HAL_I2C_Mem_Write(p_handle, (uint16_t)(dev_addr << 1), reg_addr, I2C_MEMADD_SIZE_16BIT, p_data, length, timeout);

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint32_t length, uint32_t timeout)
{
  bool ret;
  HAL_StatusTypeDef i2c_ret;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

  if (ch >= I2C_MAX_CH)
  {
    return false;
  }

  LOCK_BEGIN(ch);
  i2c_ret = HAL_I2C_Master_Transmit(p_handle, (uint16_t)(dev_addr << 1), p_data, length, timeout);

  if(i2c_ret == HAL_OK)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }
  LOCK_END(ch);

  return ret;
}

void i2cClearErrCount(uint8_t ch)
{
  i2c_errcount[ch] = 0;
}

uint32_t i2cGetErrCount(uint8_t ch)
{
  return i2c_errcount[ch];
}


//void delayUs(uint32_t us)
//{
//  volatile uint32_t i;
//
//  for (i=0; i<us*1000; i++)
//  {
//
//  }
//}





void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  UNUSED(hi2c);

  if (hi2c->Instance == i2c_tbl[_DEF_I2C1].p_hi2c->Instance)
  {
    if (hi2c->ErrorCode > 0)
    {
      i2c_errcount[_DEF_I2C1]++;
    }
  }
  else if (hi2c->Instance == i2c_tbl[_DEF_I2C2].p_hi2c->Instance)
  {
    if (hi2c->ErrorCode > 0)
    {
      i2c_errcount[_DEF_I2C2]++;
    }
  }

}

void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
    LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    LL_RCC_SetI2CClockSource(LL_RCC_I2C2_CLKSOURCE_PCLK1);

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB3     ------> I2C2_SCL
    PB4     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(I2C2_3_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

	 if(i2cHandle->Instance==I2C1)
	  {
	  /* USER CODE BEGIN I2C1_MspDeInit 0 */

	  /* USER CODE END I2C1_MspDeInit 0 */
	    /* Peripheral clock disable */
	    __HAL_RCC_I2C1_CLK_DISABLE();

	    /**I2C1 GPIO Configuration
	    PB6     ------> I2C1_SCL
	    PB7     ------> I2C1_SDA
	    */
	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

	  /* USER CODE BEGIN I2C1_MspDeInit 1 */

	  /* USER CODE END I2C1_MspDeInit 1 */
	  }
	  else if(i2cHandle->Instance==I2C2)
	  {
	  /* USER CODE BEGIN I2C2_MspDeInit 0 */

	  /* USER CODE END I2C2_MspDeInit 0 */
	    /* Peripheral clock disable */
	    __HAL_RCC_I2C2_CLK_DISABLE();

	    /**I2C2 GPIO Configuration
	    PB3     ------> I2C2_SCL
	    PB4     ------> I2C2_SDA
	    */
	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);

	    /* I2C2 interrupt DeInit */
	    HAL_NVIC_DisableIRQ(I2C2_3_IRQn);
	  /* USER CODE BEGIN I2C2_MspDeInit 1 */

	  /* USER CODE END I2C2_MspDeInit 1 */
	  }
}


#ifdef _USE_HW_CLI
void cliI2C(cli_args_t *args)
{
  bool ret = true;
  bool i2c_ret;

  uint8_t print_ch;
  uint8_t ch;
  uint16_t dev_addr;
  uint16_t reg_addr;
  uint16_t length;

  uint32_t i;
  uint8_t i2c_data[128];
  uint32_t pre_time;


  if (args->argc == 2)
  {
    print_ch = (uint16_t) args->getData(1);

    print_ch = constrain(print_ch, 1, I2C_MAX_CH);
    print_ch -= 1;

    if(args->isStr(0, "scan") == true)
    {
      for (i=0x00; i<= 0x7F; i++)
      {
        if (i2cIsDeviceReady(print_ch, i) == true)
        {
          cliPrintf("I2C CH%d Addr 0x%X : OK\n", print_ch+1, i);
        }
      }
    }
    else if(args->isStr(0, "open") == true)
    {
      i2c_ret = i2cBegin(print_ch, I2C_FREQ_400KHz);
      if (i2c_ret == true)
      {
        cliPrintf("I2C CH%d Open OK\n", print_ch + 1);
      }
      else
      {
        cliPrintf("I2C CH%d Open Fail\n", print_ch + 1);
      }
    }
  }
  else if (args->argc == 4)
  {
  	print_ch = (uint16_t) args->getData(1);
  	print_ch = constrain(print_ch, 1, I2C_MAX_CH);

  	dev_addr = (uint16_t) args->getData(2);
  	length   = (uint16_t) args->getData(3);
  	ch       = print_ch - 1;

  	if(args->isStr(0, "readData") == true)
  	{
  		i2c_ret = i2cReadData(ch, dev_addr, i2c_data, length, 100);

  		if (i2c_ret == true)
  		{
  			for (i=0; i<length; i++)
  			{
  				cliPrintf("%d I2C : 0x%02X\n", print_ch, i2c_data[i]);
  			}
  		}
  		else
  		{
  			cliPrintf("%d I2C - Fail \n", print_ch);
  		}

  	}

  }
  else if (args->argc == 5)
  {
    print_ch = (uint16_t) args->getData(1);
    print_ch = constrain(print_ch, 1, I2C_MAX_CH);

    dev_addr = (uint16_t) args->getData(2);
    reg_addr = (uint16_t) args->getData(3);
    length   = (uint16_t) args->getData(4);
    ch       = print_ch - 1;

    if(args->isStr(0, "read") == true)
    {
      for (i=0; i<length; i++)
      {
        i2c_ret = i2cReadByte(ch, dev_addr, reg_addr+i, i2c_data, 100);

        if (i2c_ret == true)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\n", print_ch, reg_addr+i, i2c_data[0]);
        }
        else
        {
          cliPrintf("%d I2C - Fail \n", print_ch);
          break;
        }
      }
    }
    else if(args->isStr(0, "write") == true)
    {
      pre_time = millis();
      i2c_ret = i2cWriteByte(ch, dev_addr, reg_addr, (uint8_t)length, 100);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d ms\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \n", print_ch);
      }
    }
    else if(args->isStr(0, "read16") == true)
    {
      for (i=0; i<length; i++)
      {
        i2c_ret = i2cRead16Byte(ch, dev_addr, reg_addr+i, i2c_data, 100);

        if (i2c_ret == true)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\n", print_ch, reg_addr+i, i2c_data[0]);
        }
        else
        {
          cliPrintf("%d I2C - Fail \n", print_ch);
          break;
        }
      }
    }
    else if(args->isStr(0, "write16") == true)
    {
      pre_time = millis();
      i2c_ret = i2cWrite16Byte(ch, dev_addr, reg_addr, (uint8_t)length, 100);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d us\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \n", print_ch);
      }
    }
    else if(args->isStr(0, "writeData") == true)
    {
      pre_time = millis();
      uint8_t tx_buf[5];


      tx_buf[0] = reg_addr >> 8;
      tx_buf[1] = reg_addr >> 0;
      tx_buf[2] = (uint8_t)length;
      tx_buf[3] = 0 - ((uint8_t)length);

      i2c_ret = i2cWriteData(ch, dev_addr, tx_buf, 3, 100);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d us\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \n", print_ch);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf( "i2c scan ch[1~%d]\n", I2C_MAX_CH);
    cliPrintf( "i2c open ch[1~%d]\n", I2C_MAX_CH);
    cliPrintf( "i2c readData ch dev_addr length\n");

    cliPrintf( "i2c read ch dev_addr reg_addr length\n");
    cliPrintf( "i2c write ch dev_addr reg_addr data\n");
    cliPrintf( "i2c read16 ch dev_addr reg_addr length\n");
    cliPrintf( "i2c write16 ch dev_addr reg_addr data\n");
    cliPrintf( "i2c writeData ch dev_addr reg_addr data\n");
  }
}

#endif

#endif
