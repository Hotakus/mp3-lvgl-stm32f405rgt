/************************************************
 * @file adc.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-11
 *
 * @copyright Copyright (c) 2020
 *
*************************************************/

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "pro_conf.h"
#include <rtthread.h>

static void MX_ADC1_Init(void);

ADC_HandleTypeDef hadc1;
static uint8_t conf_flag = 0;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = { 0 };

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) {
    DEBUG_PRINT("HAL_ADC_Init error.\n");
    return;
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    DEBUG_PRINT("HAL_ADC_ConfigChannel error.\n");
    return;
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (adcHandle->Instance == ADC1) {

    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC5     ------> ADC1_IN15
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if (adcHandle->Instance == ADC1) {

    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC5     ------> ADC1_IN15
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_5);


  }
}

/************************************************
 * @brief adc_get
 *
 * @param avg 进行多少次采样
 * @return uint16_t 返回平均采样值
*************************************************/
uint16_t adc_get(uint8_t avg)
{
  if (!conf_flag) {
    MX_ADC1_Init();
    conf_flag = 1;
  }

  uint32_t res = 0;

  for (uint8_t i = 0; i < avg; i++) {
    HAL_ADC_Start(&hadc1);
    res += HAL_ADC_GetValue(&hadc1);
  }
  res /= avg;

  return HAL_ADC_GetValue(&hadc1);
}

