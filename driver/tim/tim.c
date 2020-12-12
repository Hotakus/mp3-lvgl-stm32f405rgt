#include "tim.h"
#include "stm32f4xx_hal.h"
#include "uart.h"
#include "pro_conf.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);

static uint8_t flag = 0;
uint8_t sflag = 0;

TIM_HandleTypeDef h_tim2;
TIM_HandleTypeDef h_tim1;

/* TIM1 init function */
void LED_PWM_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
  TIM_MasterConfigTypeDef sMasterConfig = { 0 };
  TIM_OC_InitTypeDef sConfigOC = { 0 };
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

  h_tim1.Instance = TIM1;
  h_tim1.Init.Prescaler = 168;
  h_tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  h_tim1.Init.Period = 100;
  h_tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  h_tim1.Init.RepetitionCounter = 0;
  h_tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&h_tim1) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIM_Base_Init failed.\n");
    return ;
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&h_tim1, &sClockSourceConfig) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIM_ConfigClockSource failed.\n");
    return ;
  }
  if (HAL_TIM_PWM_Init(&h_tim1) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIM_PWM_Init failed.\n");
    return ;
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&h_tim1, &sMasterConfig) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIMEx_MasterConfigSynchronization failed.\n");
    return ;
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 100;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&h_tim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIM_PWM_ConfigChannel failed.\n");
    return ;
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&h_tim1, &sBreakDeadTimeConfig) != HAL_OK)   {
    DEBUG_PRINT("HAL_TIMEx_ConfigBreakDeadTime failed.\n");
    return ;
  }
  HAL_TIM_MspPostInit(&h_tim1);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM1)   {
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (timHandle->Instance == TIM1)   {

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM1)   {
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  }
}



/************************************************
 * @brief Time Counter
*************************************************/
void time_counter_init(void)
{
  __HAL_RCC_TIM2_CLK_ENABLE();
  h_tim2.Instance = TIM2;
  h_tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  h_tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  h_tim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  h_tim2.Init.Period = (SystemCoreClock / 2) / 10;
  h_tim2.Init.Prescaler = 999;

  HAL_TIM_Base_Init(&h_tim2);

  flag = 1;

}

void Clock_Start(void)
{
  if (!flag)
    time_counter_init();
  TIM2->CNT = 0;
  HAL_TIM_Base_Start(&h_tim2);
  sflag = 1;
}

uint32_t Clock_End(void)
{
  uint32_t result;
  HAL_TIM_Base_Stop(&h_tim2);
  result = TIM2->CNT;
  sflag = 0;

  return result;
}

