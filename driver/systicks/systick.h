#ifndef SYSTICK_H
#define SYSTICK_H

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>

/* ucos inc file */
#if USER_USE_UCOS == 1
#include "ucos_inc.h"
#endif

/* definations */


/* functions */
void delay_init( void );
void delay_ms( u32 ums );
void delay_us( u32 uus );

void ticks_decrease( void );

#endif
