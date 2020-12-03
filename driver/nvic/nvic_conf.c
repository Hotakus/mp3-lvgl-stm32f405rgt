/************************************************
 * @file nvic_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "nvic_conf.h"
#include "pro_conf.h"

void nvic_conf( uint8_t IRQx, uint8_t pre_prio, uint8_t sub_prio, FunctionalState stat ) {
    
    static NVIC_InitTypeDef nvic_s;
    
    nvic_s.NVIC_IRQChannel                   = IRQx;
    nvic_s.NVIC_IRQChannelPreemptionPriority = pre_prio;
    nvic_s.NVIC_IRQChannelSubPriority        = sub_prio;
    nvic_s.NVIC_IRQChannelCmd                = stat;
    NVIC_Init( &nvic_s );
}

void exti_conf( uint32_t EXTI_Linex, EXTITrigger_TypeDef trigger_method, FunctionalState stat  )
{
    static EXTI_InitTypeDef exti;
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_EXTIT, ENABLE );
    
    exti.EXTI_Line      = EXTI_Linex;
    exti.EXTI_Mode      = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger   = trigger_method;
    exti.EXTI_LineCmd   = stat;
    EXTI_Init( &exti );

    
    EXTI_ClearITPendingBit( EXTI_Line1 );
    
}


void EXTI1_IRQHandler (void) 
{        
#if USER_USE_OS == 1
    OS_INT_ENTER();
#endif
    
    EXTI_ClearITPendingBit( EXTI_Line1 );
#if USER_USE_OS == 1
    OS_INT_EXIT();
#endif
}

