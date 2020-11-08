/************************************************
 * @file board.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/

#include "stm32f4xx_conf.h"
#include "sd_sdio_conf.h"
#include "pro_conf.h"
#include "screen_conf.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }
    
    _SYSTICK_LOAD = ticks - 1; 
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;  
    
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 4096
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{

    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
#include "usart.h"
void rt_hw_board_init()
{
    
    rt_usart_conf( USART1, 115200 );
    
    /* System Clock Update */
    SystemCoreClockUpdate();
    
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}


void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();
    
    /* leave interrupt */
    rt_interrupt_leave();
}

char rt_hw_console_getchar(void)
{
    return my_getc();
}

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            my_putc(a);
        }
        my_putc(*(str + i));
    }
}


#include "ff_user.h"

/************************************************
 * @brief cmd for msh
 ************************************************/
/************************************************
 * @brief test cmd for msh
 * 
 * @param argc 
 * @param args 
 ************************************************/
void test( int argc, const char **args )
{
    if ( argc == 1 )
        DEBUG_PRINT( "%s\n",  __func__ );
    else
        DEBUG_PRINT( "%s\n",  args[1] );
}
MSH_CMD_EXPORT( test, test );

void fatfs( int argc, char **args )
{
    if ( argc != 2 )
        return;
    DEBUG_PRINT( "into fatfs test.\n" );
    fatfs_test( args[1] );
}
MSH_CMD_EXPORT( fatfs, RT_NULL );

void scr_test( int argc, char **args )
{
    scr_opr_handler *scr;
    scr = scr_get_opr_handler();

    if ( argc > 2 )
        return;
    if ( argc < 2 )
        scr->clear( 0xffff );
    else {
        switch( *args[1] ) {
        case 'r':
            scr->clear( 0xf800 );
            break;
        case 'g':
            scr->clear( 0x07f0 );
            break;
        case 'b':
            scr->clear( 0x001f );
            break;
        }
    }
   
}
MSH_CMD_EXPORT( scr_test, RT_NULL );

