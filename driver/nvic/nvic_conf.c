#include "nvic_conf.h"

void nvic_conf( uint8_t IRQx, u8 pre_prio, u8 sub_prio, FunctionalState stat ) {
    
    static NVIC_InitTypeDef nvic_s;
    
    nvic_s.NVIC_IRQChannel                   = IRQx;
    nvic_s.NVIC_IRQChannelPreemptionPriority = pre_prio;
    nvic_s.NVIC_IRQChannelSubPriority        = sub_prio;
    nvic_s.NVIC_IRQChannelCmd                = stat;
    NVIC_Init( &nvic_s );
}
