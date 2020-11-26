#include "tim.h"
#include "stm32f4xx_conf.h"
#include "usart.h"
#include "pro_conf.h"

static uint8_t flag = 0;
uint8_t sflag = 0;

void time_counter_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* 100us */
    TIM_TimeBaseInitStructure.TIM_Period = (SystemCoreClock/2)/10;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 999;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x0000;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    flag = 1;
}

void Clock_Start(void)
{
    if ( !flag )
        time_counter_init();
    TIM2->CNT = 0;
    TIM_Cmd(TIM2, ENABLE);
    sflag = 1;
}

u32 Clock_End(void)
{
    u32 result;
    TIM_Cmd(TIM2, DISABLE);
    result = TIM2->CNT;
    sflag = 0;

    return result;
}

