/************************************************
 * @file rtc.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 2.0
 * @date 2020-11-23
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "rtc.h"
#include "uart.h"

static uint8_t rtc_conf_flag = 0;

static RTC_HandleTypeDef h_rtc;
static RTC_DateTypeDef rtc_date;
static RTC_TimeTypeDef rtc_time;

//定义用户默认使用的时间格式
static uint8_t time_format = RTC_TIME_FORMAT_NOMAL;

static RTC_STAT rtc_config(void);
static RTC_STAT rtc_check(void);

/************************************************
 * @brief Low level init
*************************************************/
void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

    if (rtcHandle->Instance == RTC) {
        /* RTC clock enable */
        __HAL_RCC_RTC_ENABLE();
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWR_EnableBkUpAccess();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

    if (rtcHandle->Instance == RTC) {

        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();

    }
}

void rtc_init(void)
{
    if ( rtc_conf_flag )
        return;
    if (rtc_check() != RTC_CHECK_OK)
        DEBUG_PRINT("RTC Check Error.\n");
    else
        rtc_conf_flag = 1;
}

RTC_STAT rtc_check(void)
{
    h_rtc.Instance = RTC;
    h_rtc.Init.HourFormat = RTC_HOURFORMAT_24;
    h_rtc.Init.AsynchPrediv = 127;
    h_rtc.Init.SynchPrediv = 255;
    h_rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    h_rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    h_rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&h_rtc) != HAL_OK) {
        return RTC_CONFIG_ERR;
    }

    if (HAL_RTCEx_BKUPRead(&h_rtc, RTC_BKP_DR0) != RTC_CONFIG_FLAG) {
        DEBUG_PRINT("RTC Configuring...\n");
        /* RTC configuration  */
        if (rtc_config() != RTC_CONFIG_OK)
            return RTC_CHECK_ERR;
        DEBUG_PRINT("RTC Configured Ok\n");
    }

    return RTC_CHECK_OK;
}

static RTC_STAT rtc_config(void)
{
    RTC_STAT stat = RTC_CONFIG_ERR;

    /* 设置默认日期 */
    // rtc_date.Year       = 20;                       // 20 年
    // rtc_date.Month      = RTC_MONTH_DECEMBER;       // 12 月
    // rtc_date.Date       = 9;                        // 9  日
    // rtc_date.WeekDay    = RTC_WEEKDAY_WEDNESDAY;    // 星期三
    stat = rtc_config_date(20, RTC_MONTH_DECEMBER, 9, RTC_WEEKDAY_WEDNESDAY);
    if (stat != RTC_CONFIG_OK)
        return stat;

    /* 设置默认时间 */
    stat = rtc_config_time(12, 59, 55, 0);
    if (stat != RTC_CONFIG_OK)
        return stat;


    HAL_RTCEx_BKUPWrite(&h_rtc, RTC_BKP_DR0, RTC_CONFIG_FLAG);

    HAL_PWR_DisableBkUpAccess;

    return stat;
}

/************************************************
 * @brief 配置日期
 *
 * @param hrtc
 * @param year
 * @param mon
 * @param date
 * @param week
 * @return RTC_STAT
*************************************************/
RTC_STAT rtc_config_date(uint8_t year, uint8_t mon, uint8_t date, uint8_t week)
{
    rtc_date.Year = year;
    rtc_date.Month = mon;
    rtc_date.Date = date;
    rtc_date.WeekDay = week;
    if (HAL_RTC_SetDate(&h_rtc, &rtc_date, RTC_FORMAT_BIN) != HAL_OK)
        return RTC_CONFIG_ERR;

    return RTC_CONFIG_OK;
}

/************************************************
 * @brief 配置时间
 *
 * @param hrtc
 * @param hour
 * @param min
 * @param sec
 * @param tf hour format
 * @return RTC_STAT
*************************************************/
RTC_STAT rtc_config_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t tf)
{
    rtc_time.Hours = hour;
    rtc_time.Minutes = min;
    rtc_time.Seconds = sec;
    rtc_time.SubSeconds = 0;

    rtc_time.SecondFraction = 1000;     // 1ms

    /* 判断用户使用的Hour Fornat */
    if (time_format == RTC_TIME_FORMAT_AM_PM && IS_RTC_HOUR_FORMAT(tf))
        rtc_time.TimeFormat = tf;

    if (HAL_RTC_SetTime(&h_rtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK)
        return RTC_CONFIG_ERR;

    return RTC_CONFIG_OK;
}

void rtc_obtain_info(RTC_INFO* rtc_info)
{
    rtc_obtain_time(&rtc_info->time);
    rtc_obtain_date(&rtc_info->date);
}


void rtc_obtain_date(RTC_DateTypeDef* data)
{
    HAL_RTC_GetDate(&h_rtc, data, RTC_FORMAT_BIN);
}

void rtc_obtain_time(RTC_TimeTypeDef* time)
{
    HAL_RTC_GetTime(&h_rtc, time, RTC_FORMAT_BIN);
}

void rtc_obtain_alarm(RTC_AlarmTypeDef* alarm, uint32_t  w_alarm)
{
    HAL_RTC_GetAlarm(&h_rtc, alarm, w_alarm, RTC_FORMAT_BIN);
}

const char* weekday_get(RTC_DateTypeDef* date)
{
    switch (date->WeekDay) {
    case RTC_WEEKDAY_MONDAY:
        return "Mon";
    case RTC_WEEKDAY_TUESDAY:
        return "Tue";
    case RTC_WEEKDAY_WEDNESDAY:
        return "Wed";
    case RTC_WEEKDAY_THURSDAY:
        return "Thur";
    case RTC_WEEKDAY_FRIDAY:
        return "Fri";
    case RTC_WEEKDAY_SATURDAY:
        return "Sat";
    case RTC_WEEKDAY_SUNDAY:
        return "Sun";
    default:
        return "Err";
    }
}

#if USER_USE_FATFS == 1
/* FatFs get_fattime(); */
/*
*   Return Value
*   Currnet local time shall be returned as bit-fields packed into a DWORD value. The bit fields are as follows:
*
*   bit31:25
*       Year origin from the 1980   (0..127, e.g. 37 for 2017) 1980 + 37 = 2017
*   bit24:21
*       Month                       (1..12)
*   bit20:16
*       Day of the month            (1..31)
*   bit15:11
*       Hour                        (0..23)
*   bit10:5
*       Minute                      (0..59)
*   bit4:0
*       Second / 2                  (0..29, e.g. 25 for 50)
*   Description:
*       The get_fattime function shall return any valid time even if the system does not support a real time clock.
*       If a zero is returned, the file will not have a valid timestamp.
*/
DWORD get_fattime(void)
{
    DWORD fatfs_time;
    RTC_INFO rtc_info;

    rtc_init();

    rtc_obtain_info(&rtc_info);

    /* 根据格式将获取的时间数据进行转换 */
    fatfs_time = (rtc_info.date.Year + 20) << 25;     //对齐到2000年 1980 + 20 = 2000
    fatfs_time |= rtc_info.date.Month << 21;
    fatfs_time |= rtc_info.date.Date << 16;

    fatfs_time |= rtc_info.time.Hours << 11;
    fatfs_time |= rtc_info.time.Minutes << 5;
    fatfs_time |= rtc_info.time.Seconds >> 1;

    /* 根据格式将获取的时间数据进行转换 */
    // fatfs_time = (rtc_data.rtcDateStruct.RTC_Year + 20) << 25;     //对齐到2000年 1980 + 20 = 2000
    // fatfs_time |= rtc_data.rtcDateStruct.RTC_Month << 21;
    // fatfs_time |= rtc_data.rtcDateStruct.RTC_Date << 16;

    // fatfs_time |= rtc_data.rtcTimeStruct.RTC_Hours << 11;
    // fatfs_time |= rtc_data.rtcTimeStruct.RTC_Minutes << 5;
    // fatfs_time |= rtc_data.rtcTimeStruct.RTC_Seconds >> 1;

    return fatfs_time;
}
#endif



void rtc_test(void)
{
    RTC_INFO rtc_info;


    rtc_obtain_info(&rtc_info);

    DEBUG_PRINT(
        "20%d.%d.%d %d:%d:%d\n",
        rtc_info.date.Year,
        rtc_info.date.Month,
        rtc_info.date.Date,
        rtc_info.time.Hours,
        rtc_info.time.Minutes,
        rtc_info.time.Seconds
    );

}
#if USER_USE_RTTHREAD == 1
MSH_CMD_EXPORT(rtc_test, rtc test);
MSH_CMD_EXPORT(rtc_init, rtc init);
#endif
