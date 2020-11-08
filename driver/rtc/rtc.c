#include "rtc.h"
#include "usart.h"

void rtc_init( void ) 
{
    printf( "rtc_check: %x\n", rtc_check() );
}

RTC_STAT rtc_check( void ) 
{
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_CONFIG_FLAG) {  
        /* RTC configuration  */
        if ( rtc_config() == RTC_CONFIG_ERR ) return RTC_CHECK_ERR;
    }
    return RTC_CHECK_OK;
}


RTC_STAT rtc_config( void ) 
{
    ErrorStatus status = ERROR;
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */  
    while( !RCC_GetFlagStatus(RCC_FLAG_LSERDY) );
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC APB registers synchronisation */
    status = RTC_WaitForSynchro();
    if ( status == ERROR ) return RTC_CONFIG_ERR;
    
    /* Configure the RTC data register and RTC prescaler */
    /* ck_spre(1Hz) = (RTCCLK(LSE) /(uwAsynchPrediv + 1)) * (uwSynchPrediv + 1) */
    u16 rtcAsynchPrediv = 0x7F;
    u16 rtcSynchPrediv  = 0xFF;
    RTC_InitTypeDef rtcInitStruct;
    rtcInitStruct.RTC_AsynchPrediv  = rtcAsynchPrediv;
    rtcInitStruct.RTC_SynchPrediv   = rtcSynchPrediv;
    rtcInitStruct.RTC_HourFormat    = RTC_HourFormat_24;        // RTC日期格式
    RTC_Init( &rtcInitStruct );
    
    /* Set the date: Sunday September 6th 2020 */
    RTC_DateTypeDef rtcDateStruct;
    rtcDateStruct.RTC_Year      = 0x20;
    rtcDateStruct.RTC_Month     = RTC_Month_September;
    rtcDateStruct.RTC_Date      = 0x06;
    rtcDateStruct.RTC_WeekDay   = RTC_Weekday_Sunday;
    RTC_SetDate( RTC_Format_BCD, &rtcDateStruct );
    
    /* Set the time to 06h 30mn 00s AM */
    RTC_TimeTypeDef rtcTimeStruct;
    rtcTimeStruct.RTC_H12     = RTC_H12_AM;
    rtcTimeStruct.RTC_Hours   = 0x04;
    rtcTimeStruct.RTC_Minutes = 0x24;
    rtcTimeStruct.RTC_Seconds = 0x30; 
    RTC_SetTime( RTC_Format_BCD, &rtcTimeStruct );  
    
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister( RTC_BKP_DR0, RTC_CONFIG_FLAG );
    
    return RTC_CONFIG_OK;
}

void rtc_obtain_time( RTC_INFO *rtc_data ) 
{
    /* Get the current Time */
    RTC_GetTime (  RTC_Format_BIN, &rtc_data->rtcTimeStruct  );
    RTC_GetDate (  RTC_Format_BIN, &rtc_data->rtcDateStruct  );
    
}

void rtc_obtain_alarm( RTC_INFO *rtc_data ) 
{
    RTC_GetAlarm(  RTC_Format_BIN, RTC_Alarm_A, &rtc_data->rtcAlarmStruct[0] );
    RTC_GetAlarm(  RTC_Format_BIN, RTC_Alarm_B, &rtc_data->rtcAlarmStruct[1] );
}

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

#if USER_USE_FATFS == 1
DWORD get_fattime (void) {
    DWORD fatfs_time;
    RTC_INFO rtc_data;
    
    rtc_init();
    
    rtc_obtain_time( &rtc_data );
    
    /* 根据格式将获取的时间数据进行转换 */
    fatfs_time  = (rtc_data.rtcDateStruct.RTC_Year + 20) << 25;     //对齐到2000年 1980 + 20 = 2000
    fatfs_time |= rtc_data.rtcDateStruct.RTC_Month       << 21;
    fatfs_time |= rtc_data.rtcDateStruct.RTC_Date        << 16;
    
    fatfs_time |= rtc_data.rtcTimeStruct.RTC_Hours       << 11;
    fatfs_time |= rtc_data.rtcTimeStruct.RTC_Minutes     << 5;
    fatfs_time |= rtc_data.rtcTimeStruct.RTC_Seconds     >> 1;
    
    return fatfs_time;
}
#endif




