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

/* USER USE */
#define USER_USE_RTTHREAD   1
#define USER_USE_LVGL       1
#define USER_USE_UCOS       0
#define USER_USE_FATFS      1

#define USER_USE_APP        1

/* FatFs thread-safe conf */
#define THREAD_SAFE_SW      0

/* USER USE screen */
// USER_USE_SCR == 0 : ips133
// USER_USE_SCR == 1 : lcd_2_inch
// USER_USE_SCR == 2 : LCD_144_ST7735_1.44_inch
#define USE_SCR_IPS_133_INCH                0
#define USE_SCR_LCD_2_INCH                  1
#define USE_SCR_LCD_144_ST7735              2
#define USER_USE_SCR      USE_SCR_LCD_144_ST7735

/* 延时函数时钟源 */
#define DELAY_SRC_SYSTICK  0
#define DELAY_SRC_TIM      1
#define DELAY_SRC   DELAY_SRC_TIM

/* VS10XX_SINTEST SWITCH */
#define VS10XX_SINTEST  1


/* incldues */
#if USER_USE_UCOS == 1
/* ucos */
#include "ucos_inc.h"
#endif

#if USER_USE_LVGL == 1
/* lvgl */
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "lv_port_indev.h"
#endif

#if USER_USE_APP == 1
/* app */
#include "app_main.h"
#if USER_USE_LVGL == 1
#define USE_LV_EX   1
#endif
#endif

#if USER_USE_FATFS == 1
/* file system */
#include "ff.h"
#endif

#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#define APP_THREAD_NUM  5       // 定义APP最大线程
#define APP_INIT_STK_SIZE   (1<<10)         // 应用栈大小

#define RT_PRINT        rt_kprintf
#define RT_DELAY        rt_thread_mdelay
#define RT_STRCMP       rt_strcmp
#define RT_STRCAT       rt_strcat
#define RT_MALLOC       rt_malloc
#define RT_FREE         rt_free

#elif USER_USE_RTTHREAD == 0
#define SECTION(x) __attribute__((section(x)))
#endif

#define DEBUG_PRINT     RT_PRINT
#define DELAY           RT_DELAY
#define STRCMP          RT_STRCMP
#define STRCAT          RT_STRCAT
#define MALLOC          RT_MALLOC
#define FREE            RT_FREE

/* SD configuration */
#define USER_ON_SD          1
#define USER_USE_SD_NUM     1
#define USER_USE_SD_SPI     0
#define USER_USE_SD_SDIO    1
#if USER_ON_SD == 1
#if USER_USE_SD_NUM == 1
#define USER_USE_SD1         USER_USE_SD_SDIO
#elif USER_USE_SD_NUM == 2
#define USER_USE_SD1         USER_USE_SD_SDIO
#define USER_USE_SD2         USER_USE_SD_SPI
#endif

#if USER_USE_SD_NUM == 1
#include "sd_sdio_conf.h"
#elif USER_USE_SD_NUM == 2
#include "sd_spi.h"
#include "sd_sdio_conf.h"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
