#ifndef TIMER_H
#define TIMER_H

/* includes */
#include "stm32f4xx_hal.h"


extern TIM_HandleTypeDef h_tim1;

/* definations */
/* 每周期 1000 us */
#define SEC_PER_PREIOD     100
#define T_COUNT_PREIOD     1000000 / SEC_PER_PREIOD

/* functions */
void Clock_Start(void);
uint32_t Clock_End(void);

void LED_PWM_Init(void);

#endif
