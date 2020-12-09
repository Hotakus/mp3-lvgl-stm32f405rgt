/************************************************
 * @file rtc.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef STMF4_RTC_H
#define STMF4_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "pro_conf.h"
#include "stm32f4xx_hal.h"

#if USER_USE_FATFS == 1
/* file system */
#include "ff.h"
#endif

/* definations */
typedef enum RTC_STAT{
    RTC_CHECK_OK        = 0,
    RTC_CHECK_ERR       = 1,
    
    RTC_CONFIG_OK          ,
    RTC_CONFIG_ERR         ,
} RTC_STAT;

typedef struct RTC_INFO {
    RTC_TimeTypeDef  time;
    RTC_DateTypeDef  date;
} RTC_INFO;



#define RTC_CONFIG_FLAG 0x0807

#define RTC_TIME_FORMAT_AM_PM 0
#define RTC_TIME_FORMAT_NOMAL 1


/* function */
void        rtc_init( void );

RTC_STAT rtc_config_date(uint8_t year, uint8_t mon, uint8_t date, uint8_t week);
RTC_STAT rtc_config_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t tf);

void rtc_obtain_info( RTC_INFO *rtc_info );
void rtc_obtain_date( RTC_DateTypeDef *data );
void rtc_obtain_time( RTC_TimeTypeDef *time );
void rtc_obtain_alarm(RTC_AlarmTypeDef* alarm, uint32_t  w_alarm);

const char* weekday_get(RTC_DateTypeDef* date);

#if USER_USE_FATFS == 1
#if FF_FS_NORTC == 0
DWORD       get_fattime (void);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
