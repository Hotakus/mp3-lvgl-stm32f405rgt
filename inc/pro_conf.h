#ifndef PRO_CONF_H
#define PRO_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_PRINT rt_kprintf

/* USER USE */
#define USER_USE_RTTHREAD   1
#define USER_USE_LVGL       0
#define USER_USE_UCOS       0
#define USER_USE_FATFS      1

#define USER_USE_APP        0


/* FatFs thread-safe conf */
#define THREAD_SAFE_SW      0

/* USER USE screen */
// USER_USE_SCR == 0 : ips133
// USER_USE_SCR == 1 : lcd_2_inch
// USER_USE_SCR == 2 : tft_1.44_inch
#define USE_SCR_IPS_133_INCH    0
#define USE_SCR_LCD_2_INCH      1
#define USE_SCR_TFT_144_INCH    2
#define USER_USE_SCR      USE_SCR_TFT_144_INCH

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
#endif

#if USER_USE_APP == 1
/* app */
#include "app_main.h"
#endif

#if USER_USE_FATFS == 1
/* file system */
#include "ff.h"
#endif

#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#define APP_THREAD_NUM  5       // 定义APP最大线程
#endif

/* SD configuration */
#define USER_ON_SD          1
#define USER_USE_SD_NUM     1
#define USER_USE_SD_SPI     0
#define USER_USE_SD_SDIO    1
#if USER_ON_SD == 1
#if USER_USE_SD_NUM == 1
#define USER_USE_SD1         USER_USE_SD_SDIO
#elif USER_USE_SD_NUM == 2
#define USER_USE_SD1         USER_USE_SD_SPI
#define USER_USE_SD2         USER_USE_SD_SDIO
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
