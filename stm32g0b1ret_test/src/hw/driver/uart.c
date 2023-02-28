/*
 * uart.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */



#include "uart.h"

#ifdef _USE_HW_UART

#include "qbuffer.h"
#include "cdc.h"

#define UART_BUF_LENGTH      128


#ifdef _USE_HW_RTOS
#define LOCK_BEGIN(x) osMutexWait(uart_mutex_lock[x], osWaitForever)
#define LOCK_END(x)   osMutexRelease(uart_mutex_lock[x])
#define PRIORITY_ORDER   3
#else
#define LOCK_BEGIN(x)
#define LOCK_END(x)
#define PRIORITY_ORDER   0
#endif

#ifdef _USE_HW_RTOS
static osMutexId uart_mutex_lock[UART_MAX_CH];
static const osMutexDef_t uart_mutex_lock_def[UART_MAX_CH];
#endif

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;


#ifdef _USE_HW_UART_1_DMA
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
static uint8_t  uart1_tx_data[UART_BUF_LENGTH];  //
#endif

#ifdef _USE_HW_UART_3_DMA
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
static uint8_t  uart3_tx_data[UART_BUF_LENGTH];  //
#endif

typedef struct
{
  bool     is_open;
  uint32_t baud;
  uint8_t  rx_buf[UART_BUF_LENGTH];
  uint8_t  rx_data;
} uart_tbl_t;

static qbuffer_t qbuffer[UART_MAX_CH];
static uart_tbl_t uart_tbl[UART_MAX_CH];

bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;
    uart_tbl[i].rx_data = 0;
    memset(&uart_tbl[i].rx_buf[0],0,UART_BUF_LENGTH);
#ifdef _USE_HW_RTOS
    uart_mutex_lock[i] = osMutexCreate(&uart_mutex_lock_def[i]);
#endif
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  switch(ch)
  {
    case _DEF_UART1:
    {
      uart_tbl[ch].is_open = true;
      ret = true;
    }
    break;

    case _DEF_UART2:
    {
      huart1.Instance                    = USART1;
      huart1.Init.BaudRate               = baud;
      huart1.Init.WordLength             = UART_WORDLENGTH_8B;
      huart1.Init.StopBits               = UART_STOPBITS_1;
      huart1.Init.Parity                 = UART_PARITY_NONE;
      huart1.Init.Mode                   = UART_MODE_TX_RX;
      huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
      huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
      huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
      huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
      huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#ifdef _USE_HW_UART_1_DMA
huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
#endif

      HAL_UART_DeInit(&huart1);
      qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);

#ifdef _USE_HW_UART_1_DMA
__HAL_RCC_DMA1_CLK_ENABLE();
/* DMA interrupt init */
/* DMA1_Channel1_IRQn interrupt configuration */
HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, PRIORITY_ORDER, 0);
HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
/* DMA1_Channel2_3_IRQn interrupt configuration */
HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, PRIORITY_ORDER, 0);
HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
#endif

     if (HAL_UART_Init(&huart1) != HAL_OK)
     {
       ret = false;
     }
     else
     {
       ret = true;
       uart_tbl[ch].is_open = true;

       HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8);
       HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8);
       HAL_UARTEx_DisableFifoMode(&huart1);
#ifdef _USE_HW_UART_1_DMA
  if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
  {
    ret = false;
  }

  qbuffer[ch].in  = qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR;
  qbuffer[ch].out = qbuffer[ch].in;
#else
      if (HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
      {
        ret = false;
      }
#endif

     }
    }
    break;

    case _DEF_UART3:
    {

      huart2.Instance                    = USART2;
      huart2.Init.BaudRate               = baud;
      huart2.Init.WordLength             = UART_WORDLENGTH_8B;
      huart2.Init.StopBits               = UART_STOPBITS_1;
      huart2.Init.Parity                 = UART_PARITY_NONE;
      huart2.Init.Mode                   = UART_MODE_TX_RX;
      huart2.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
      huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
      huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
      huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
      huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

      HAL_UART_DeInit(&huart2);

      qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        uart_tbl[ch].is_open = true;

        HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_DisableFifoMode(&huart2);
        if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
        {
          ret = false;
        }
        //HAL_UARTEx_EnableFifoMode(&huart2);
      }
    }
    break;
    case _DEF_UART4:
    {
      huart4.Instance                    = USART4;
      huart4.Init.BaudRate               = baud;
      huart4.Init.WordLength             = UART_WORDLENGTH_8B;
      huart4.Init.StopBits               = UART_STOPBITS_1;
      huart4.Init.Parity                 = UART_PARITY_NONE;
      huart4.Init.Mode                   = UART_MODE_TX_RX;
      huart4.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
      huart4.Init.OverSampling           = UART_OVERSAMPLING_16;
      huart4.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
      huart4.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
      huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

      HAL_UART_DeInit(&huart4);

      if (HAL_UART_Init(&huart4) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        uart_tbl[ch].is_open = true;
        qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);

        if (HAL_UART_Receive_IT(&huart4, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
        {
          ret = false;
        }
      }
    }
    break;

  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  uart_tbl[ch].is_open = false;
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;
  LOCK_BEGIN(ch);
  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcAvailable();
      break;
    case _DEF_UART2:
#ifdef _USE_HW_UART_1_DMA
      qbuffer[ch].in = (qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR);
      ret = qbufferAvailable(&qbuffer[ch]);
      break;
#endif
    case _DEF_UART3:
#ifdef _USE_HW_UART_3_DMA
      qbuffer[ch].in = (qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR);
      ret = qbufferAvailable(&qbuffer[ch]);
      break;
#endif
    case _DEF_UART4:
      ret = qbufferAvailable(&qbuffer[ch]);
      break;
    default:
      break;
  }
  LOCK_END(ch);
  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;
  LOCK_BEGIN(ch);
  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }
  LOCK_END(ch);
  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;
  LOCK_BEGIN(ch);
  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcRead();
      break;

    case _DEF_UART2:
    case _DEF_UART3:
    case _DEF_UART4:
      qbufferRead(&qbuffer[ch], &ret, 1);
      break;

    default:
      break;
  }
  LOCK_END(ch);
  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status = HAL_OK;
  LOCK_BEGIN(ch);
  switch(ch)
  {
    case _DEF_UART1:
    {
      ret = cdcWrite(p_data, length);
    }
    break;
    case _DEF_UART2:
    {
#ifdef _USE_HW_UART_1_DMA
      uint32_t pre_time;
      pre_time = millis();
      while(huart1.gState != HAL_UART_STATE_READY)
      {
        if (millis()-pre_time >= 100)
        {
          break;
        }
      }
      if (huart1.gState == HAL_UART_STATE_READY)
      {
        memset(&uart1_tx_data[0],0,UART_BUF_LENGTH);
        memcpy(&uart1_tx_data[0],p_data,length);
        status = HAL_UART_Transmit_DMA(&huart1, &uart1_tx_data[0], length);
      }
#else
      status = HAL_UART_Transmit(&huart1, p_data, length, 100);
#endif
      if (status == HAL_OK)
        ret = length;
    }
    break;
    case _DEF_UART3:
    {
      status = HAL_UART_Transmit(&huart2, p_data, length, 100);
      if (status == HAL_OK)
        ret = length;
    }
    break;
    case _DEF_UART4:
      status = HAL_UART_Transmit(&huart4, p_data, length, 100);
      if (status == HAL_OK)
        ret = length;
      break;
    default:
      break;
  }
  LOCK_END(ch);
  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);
  ret = uartWrite(ch, (uint8_t *)buf, len);
  va_end(args);

  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcGetBaud();
      break;
    case _DEF_UART2:
      ret = huart1.Init.BaudRate;
      break;
    case _DEF_UART3:
      ret = huart2.Init.BaudRate;
      break;
    case _DEF_UART4:
      ret = huart4.Init.BaudRate;
      break;
    default:
      break;
  }

  return ret;
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

void USART2_LPUART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
}

void USART3_4_5_6_LPUART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart4);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
  }
  else if (huart->Instance == USART2)
  {

  }
  else if (huart->Instance == USART3)
  {

  }
  else if (huart->Instance == USART4)
  {

  }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {
#ifdef _USE_HW_UART_1_DMA
#else
    qbufferWrite(&qbuffer[_DEF_UART2], &uart_tbl[_DEF_UART2].rx_data, 1);
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[_DEF_UART2].rx_data, 1);
#endif
  }
  else if (huart->Instance == USART2)
  {
    qbufferWrite(&qbuffer[_DEF_UART3], &uart_tbl[_DEF_UART3].rx_data, 1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[_DEF_UART3].rx_data, 1);
  }
  else if (huart->Instance == USART3)
  {
#if 0
#ifdef _USE_HW_UART_3_DMA
#else
    qbufferWrite(&qbuffer[_DEF_UART3], &uart_tbl[_DEF_UART3].rx_data, 1);
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&uart_tbl[_DEF_UART3].rx_data, 1);
#endif
#endif
  }
  else if (huart->Instance == USART4)
  {
    qbufferWrite(&qbuffer[_DEF_UART4], &uart_tbl[_DEF_UART4].rx_data, 1);
    HAL_UART_Receive_IT(&huart4, (uint8_t *)&uart_tbl[_DEF_UART4].rx_data, 1);
  }
}



void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
     */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


#ifdef _USE_HW_UART_1_DMA
    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance                 = DMA1_Channel1;
    hdma_usart1_rx.Init.Request             = DMA_REQUEST_USART1_RX;
    hdma_usart1_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance                 = DMA1_Channel2;
    hdma_usart1_tx.Init.Request             = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode                = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);
#endif

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, PRIORITY_ORDER, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
  else if(uartHandle->Instance==USART2)
  {
    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
     */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_LPUART2_IRQn, PRIORITY_ORDER, 0);
    HAL_NVIC_EnableIRQ(USART2_LPUART2_IRQn);


  }
  else if(uartHandle->Instance==USART3)
  {

#if 0
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
     PB10     ------> USART3_TX
     PB11     ------> USART3_RX
     */
    GPIO_InitStruct.Pin       = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#ifdef _USE_HW_UART_3_DMA
    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance                 = DMA1_Channel3;
    hdma_usart3_rx.Init.Request             = DMA_REQUEST_USART3_RX;
    hdma_usart3_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_usart3_rx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance                 = DMA1_Channel4;
    hdma_usart3_tx.Init.Request             = DMA_REQUEST_USART3_TX;
    hdma_usart3_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode                = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

#endif

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_4_IRQn, PRIORITY_ORDER, 0);
    HAL_NVIC_EnableIRQ(USART3_4_IRQn);

#endif
  }
  else if(uartHandle->Instance==USART4)
  {
    /* USART4 clock enable */
    __HAL_RCC_USART4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART4 GPIO Configuration
    PC11     ------> USART4_RX
    PC10     ------> USART4_TX
     */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART4 interrupt Init */
    HAL_NVIC_SetPriority(USART3_4_5_6_LPUART1_IRQn, PRIORITY_ORDER, 0);
    HAL_NVIC_EnableIRQ(USART3_4_5_6_LPUART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
     PA9     ------> USART1_TX
     PA10     ------> USART1_RX
     */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

#ifdef _USE_HW_UART_1_DMA
    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
#endif

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
  else if(uartHandle->Instance==USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
      PA2     ------> USART2_TX
      PA3     ------> USART2_RX
     */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_LPUART2_IRQn);

  }
  else if(uartHandle->Instance==USART3)
  {
#if 0
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
      PB10     ------> USART3_TX
      PB11     ------> USART3_RX
     */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
#ifdef _USE_HW_UART_3_DMA
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
#endif
    /* USART3 interrupt Deinit */
    /* USER CODE BEGIN USART3:USART3_4_IRQn disable */
    /**
     * Uncomment the line below to disable the "USART3_4_IRQn" interrupt
     * Be aware, disabling shared interrupt may affect other IPs
     */
    /* HAL_NVIC_DisableIRQ(USART3_4_IRQn); */
    /* USER CODE END USART3:USART3_4_IRQn disable */
#endif
  }
  else if(uartHandle->Instance==USART4)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART4_CLK_DISABLE();

    /**USART4 GPIO Configuration
        PC11     ------> USART4_RX
        PC10     ------> USART4_TX
     */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11|GPIO_PIN_10);

    /* USART4 interrupt Deinit */

    HAL_NVIC_DisableIRQ(USART3_4_5_6_LPUART1_IRQn);

  }
}





#endif
