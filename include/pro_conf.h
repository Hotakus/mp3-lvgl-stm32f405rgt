/************************************************
 * @file pro_conf.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef PRO_CONF_H
#define PRO_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#define USER_USE_LVGL       1u
#define USER_USE_FATFS      1u
#define USER_USE_APP        1u
#define USER_USE_LIBJPEG    0u
#define USER_USE_OS         1u

/* USER USE screen */
// USER_USE_SCR == 0 : ips133
// USER_USE_SCR == 1 : lcd_2_inch
// USER_USE_SCR == 2 : LCD_144_ST7735_1.44_inch
#include "screen_conf.h"
#define USE_SCR_LCD_ST7789_130              0u
#define USE_SCR_LCD_ST7789_200              1u
#define USE_SCR_LCD_144_ST7735              2u
#define USER_USE_SCR                        2


/* USER USE OS */
#if USER_USE_OS == (1u)
#define USER_USE_RTTHREAD   1u
#define USER_USE_UCOS       0u
#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#define RT_OS_INT_ENTER     rt_interrupt_enter
#define RT_OS_INT_EXIT      rt_interrupt_leave
#define OS_INT_ENTER        RT_OS_INT_ENTER
#define OS_INT_EXIT         RT_OS_INT_EXIT
#elif USER_USE_UCOS == (1u)
#define RT_OS_INT_ENTER     
#define RT_OS_INT_EXIT      
#define OS_INT_ENTER        
#define OS_INT_ENTER        
#endif
#endif


#if USER_USE_APP == 1
/* app */
#if USER_USE_LVGL ==    1u
#define USE_LV_EX       1u
#endif
#endif


#if USER_USE_RTTHREAD == 1u
#define APP_THREAD_NUM      10u         // 定义APP最大线程
#define APP_INIT_STK_SIZE   (4<<10)         // 应用栈大小

#define RT_PRINT        rt_kprintf
#define RT_DELAY        rt_thread_mdelay
#define RT_STRCMP       rt_strcmp
#define RT_STRCAT       rt_strcat
#define RT_STRLEN       rt_strlen
#define RT_SPRINTF      rt_sprintf
#define RT_MALLOC       rt_malloc
#define RT_FREE         rt_free
#define RT_MEMSET       rt_memset
#define RT_MEMCMP       rt_memcmp
#define RT_MEMCPY       rt_memcpy


#define DEBUG_PRINT     RT_PRINT
#define DELAY           RT_DELAY
#define SPRINTF         RT_SPRINTF
#define STRCMP          RT_STRCMP
#define STRCAT          RT_STRCAT
#define STRLEN          RT_STRLEN
#define MALLOC          RT_MALLOC
#define FREE            RT_FREE
#define MEMSET          RT_MEMSET
#define MEMCMP          RT_MEMCMP
#define MEMCPY          RT_MEMCPY

#elif USER_USE_RTTHREAD == 0
#define SECTION(x) __attribute__((section(x)))

#define DEBUG_PRINT     printf
#define DELAY           HAL_Delay
#define STRCMP          strcmp
#define STRCAT          strcat
#define STRLEN          strlen
#define MALLOC          malloc
#define FREE            free
#define MEMSET          memset
#define MEMCMP          memcmp

#endif


/* SD configuration */
#define USER_ON_SD          1u
#define USER_USE_SD_NUM     1u
#define USER_USE_SD_SPI     0u
#define USER_USE_SD_SDIO    1u
#if USER_ON_SD == 1
#if USER_USE_SD_NUM == 1
#define USER_USE_SD1         USER_USE_SD_SDIO
#elif USER_USE_SD_NUM == 2
#define USER_USE_SD1         USER_USE_SD_SDIO
#define USER_USE_SD2         USER_USE_SD_SPI
#endif

#ifdef __cplusplus
}
#endif

#endif


#endif
