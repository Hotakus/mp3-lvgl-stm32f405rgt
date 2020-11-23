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
#include "stm32f4xx_conf.h"
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
    RTC_TimeTypeDef  rtcTimeStruct;
    RTC_DateTypeDef  rtcDateStruct;
    RTC_AlarmTypeDef rtcAlarmStruct[2];
} RTC_INFO;

#define RTC_CONFIG_FLAG 0x0807

/* function */
void        rtc_init( void );
RTC_STAT    rtc_check( void );
RTC_STAT    rtc_config( void );
void        rtc_obtain_time( RTC_INFO *rtc_data );
void        rtc_obtain_alarm( RTC_INFO *rtc_data ) ;
const char *weekday_get( RTC_INFO *info );

#if USER_USE_FATFS == 1
#if FF_FS_NORTC == 0
DWORD       get_fattime (void);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
