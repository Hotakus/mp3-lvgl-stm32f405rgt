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

#include "sys_conf.h"
#include "rtc.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usb_device.h"

#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))


void cpu_usage_init(void);
void cpu_usage_get(rt_uint8_t* major, rt_uint8_t* minor);
static void cpu_usage_idle_hook(void);


// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
  if ((ticks - 1) > 0xFFFFFF) {
    return 1;
  }

  _SYSTICK_LOAD = ticks - 1;
  _SYSTICK_PRI = 0xFF;
  _SYSTICK_VAL = 0;
  _SYSTICK_CTRL = 0x07;

  return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE (64<<8)
// SECTION("CCRAM") ALIGN(4) static uint32_t rt_heap[RT_HEAP_SIZE];
SECTION("CCRAM") ALIGN(4) static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void* rt_heap_begin_get(void)
{
  return rt_heap;
}

RT_WEAK void* rt_heap_end_get(void)
{

  return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
#include "uart.h"
void rt_hw_board_init()
{
    /* HAL init */
  HAL_Init();

  /* System Clock Update */
  SystemClock_Config();

  /* System Tick Configuration */
  _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

  uart1_conf(115200);

  cpu_usage_init();

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

void rt_hw_console_output(const char* str)
{
  for (rt_size_t i = 0; i < rt_strlen(str); i++) {
    if (*(str + i) == '\n') {
      my_putc('\r');
    }
    my_putc(*(str + i));
  }

  // for (rt_size_t i = 0; i < STRLEN(str); i++) {
  //   if (*(str + i) == '\n') {
  //     CDC_Transmit_FS((uint8_t*)'\r', 1);
  //   }
  //   // my_putc(*(str + i));
  //   CDC_Transmit_FS((uint8_t*)(str+i), 1);
  // }
}



#define CPU_USAGE_CALC_TICK     1000
#define CPU_USAGE_LOOP          100


static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor = 0;
static rt_uint32_t total_count = 0;


static void cpu_usage_idle_hook(void)
{
  rt_tick_t tick;
  rt_uint32_t count;
  volatile rt_uint32_t loop;


  if (total_count == 0) {
      /* get total count */
    rt_enter_critical();
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK) {
      total_count++;
      loop = 0;
      while (loop < CPU_USAGE_LOOP)
        loop++;
    }
    rt_exit_critical();
  }


  count = 0;
  /* get CPU usage */
  tick = rt_tick_get();
  while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK) {
    count++;
    loop = 0;
    while (loop < CPU_USAGE_LOOP)
      loop++;
  }


  /* calculate major and minor */
  if (count < total_count) {
    count = total_count - count;
    cpu_usage_major = (count * 100) / total_count;
    cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
  } else {
    total_count = count;


    /* no CPU usage */
    cpu_usage_major = 0;
    cpu_usage_minor = 0;
  }
}


void cpu_usage_get(rt_uint8_t* major, rt_uint8_t* minor)
{
  RT_ASSERT(major != RT_NULL);
  RT_ASSERT(minor != RT_NULL);


  *major = cpu_usage_major;
  *minor = cpu_usage_minor;
}

void cpu_usage_init(void)
{
    /* set idle thread hook */
  rt_thread_idle_sethook(cpu_usage_idle_hook);
}

/************************************************
 * @brief reboot cpu
 ************************************************/
static void reboot(void)
{
  rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, reboot cpu);

/************************************************
 * @brief clear terminal
 ************************************************/
static int clear(void)
{
  rt_kprintf("\x1b[2J\x1b[H");
  return 0;
}
MSH_CMD_EXPORT(clear, clear the terminal screen);
