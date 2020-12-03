#include "systick.h"
#include "pro_conf.h"
#include "nvic_conf.h"


static u32 timeLeft = 0;
static u32 usDiv    = 1000;
static u32 msDiv    = 1000;
#define     mul     1000

/* 使用定时器可以实现us微秒级延时 */
void delay_init( void ) {

    TIM_TimeBaseInitTypeDef tim_s;
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
    tim_s.TIM_CounterMode       = TIM_CounterMode_Up;
    tim_s.TIM_Period            = (SystemCoreClock>>1) / (msDiv*usDiv) - 1;
    tim_s.TIM_Prescaler         = 0;
    tim_s.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInit( TIM4, &tim_s );
    TIM_ITConfig( TIM4, TIM_IT_Update, DISABLE );
    nvic_conf( TIM4_IRQn, 1, 1, ENABLE );
    timeLeft = 0;
}

void delay_ms( u32 ums ) {
    delay_us( ums*usDiv );
}

void delay_us( u32 uus ) {
    timeLeft = uus;
    
    TIM_ITConfig( TIM4, TIM_IT_Update, ENABLE );
    TIM_Cmd( TIM4, ENABLE );
    while ( timeLeft );
    TIM_Cmd( TIM4, DISABLE );
    TIM_ITConfig( TIM4, TIM_IT_Update, DISABLE );
}

void ticks_decrease( void ) {
    if ( timeLeft ) timeLeft--;
}
