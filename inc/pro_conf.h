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

/* USER USE OS */
#define USER_USE_OS         1u
#if USER_USE_OS == (1u)
#define USER_USE_RTTHREAD   1u
#define USER_USE_UCOS       0u
#if USER_USE_RTTHREAD == (1u)
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

#define USER_USE_LVGL       1u
#define USER_USE_FATFS      1u
#define USER_USE_APP        1u

/* FatFs thread-safe conf */
#define THREAD_SAFE_SW      0u

/* USER USE screen */
// USER_USE_SCR == 0 : ips133
// USER_USE_SCR == 1 : lcd_2_inch
// USER_USE_SCR == 2 : LCD_144_ST7735_1.44_inch
#define USE_SCR_LCD_ST7789_130              0u
#define USE_SCR_LCD_ST7789_200              1u
#define USE_SCR_LCD_144_ST7735              2u
#define USER_USE_SCR                        USE_SCR_LCD_144_ST7735


/* VS10XX_SINTEST SWITCH */
#define VS10XX_SINTEST  1u


/* incldues */
#if USER_USE_UCOS == 1u
/* ucos */
#include "ucos_inc.h"
#endif

#if USER_USE_LVGL == 1u
/* lvgl */
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "lv_port_indev.h"

#define LV_CPU_USAGE_GET    (100 - lv_task_get_idle())
#endif

#if USER_USE_APP == 1
/* app */
#include "app_main.h"
#if USER_USE_LVGL ==    1u
#define USE_LV_EX       1u
#endif
#endif

#if USER_USE_FATFS == 1u
/* file system */
#include "ff.h"
#endif

#if USER_USE_RTTHREAD == 1u
#include <rtthread.h>
#define APP_THREAD_NUM      10u         // 定义APP最大线程
#define APP_INIT_STK_SIZE   (4<<10)         // 应用栈大小

#define RT_PRINT        rt_kprintf
#define RT_DELAY        rt_thread_mdelay
#define RT_STRCMP       rt_strcmp
#define RT_STRCAT       rt_strcat
#define RT_STRLEN       rt_strlen
#define RT_MALLOC       rt_malloc
#define RT_FREE         rt_free
#define RT_MEMSET       rt_memset
#define RT_MEMCMP       rt_memcmp

#define DEBUG_PRINT     RT_PRINT
#define DELAY           RT_DELAY
#define STRCMP          RT_STRCMP
#define STRCAT          RT_STRCAT
#define STRLEN          RT_STRLEN
#define MALLOC          RT_MALLOC
#define FREE            RT_FREE
#define MEMSET          RT_MEMSET
#define MEMCMP          RT_MEMCMP

#elif USER_USE_RTTHREAD == 0
#define SECTION(x) __attribute__((section(x)))

#define DEBUG_PRINT     printf
#define DELAY           delay_ms
#define STRCMP          strcmp
#define STRCAT          strcat
#define MALLOC          malloc
#define FREE            free

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

#if USER_USE_SD_NUM == 1u
#include "sd_sdio_conf.h"
#elif USER_USE_SD_NUM == 2u
#include "sd_spi.h"
#include "sd_sdio_conf.h"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
