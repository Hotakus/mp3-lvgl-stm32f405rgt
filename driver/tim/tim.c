#include "tim.h"
#include "stm32f4xx_hal.h"
#include "uart.h"
#include "pro_conf.h"

static uint8_t flag = 0;
uint8_t sflag = 0;

TIM_HandleTypeDef h_tim;

void time_counter_init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    h_tim.Instance = TIM2;
    h_tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    h_tim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    h_tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    h_tim.Init.Period = (SystemCoreClock / 2) / 10;
    h_tim.Init.Prescaler = 999;

    HAL_TIM_Base_Init(&h_tim);

    flag = 1;

}

void Clock_Start(void)
{
    if (!flag)
        time_counter_init();
    TIM2->CNT = 0;
    HAL_TIM_Base_Start(&h_tim);
    sflag = 1;
}

uint32_t Clock_End(void)
{
    uint32_t result;
    HAL_TIM_Base_Stop(&h_tim);
    result = TIM2->CNT;
    sflag = 0;

    return result;
}

