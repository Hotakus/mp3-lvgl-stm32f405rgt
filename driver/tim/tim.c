#include "tim.h"

static u8 flag = 0;

void time_counter_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = 1000000;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 83;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
    flag = 1;
}

void Clock_Start(void)
{
    if ( !flag )
        time_counter_init();
    TIM5->CNT = 0x00;
    TIM_Cmd(TIM5, ENABLE);
}

u32 Clock_End(void)
{
    u32 result;
    result = TIM5->CNT;
    TIM_Cmd(TIM5, DISABLE);
    return result;
}

