/************************************************
 * @file app_status_bar.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_status_bar.h"
#include "rtc.h"
#include "adc.h"
#include "pro_conf.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME   "status_bar"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void status_bar_create(void);
static void status_bar_remove(void);

static void attach_info_create( void );
static void time_info_create( void );
static void battery_info_create( void );

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = status_bar_create,
    .remove = status_bar_remove,
};

static app_ui_t app_status_bar = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};

static lv_obj_t * sbb = NULL;                   // status bar 背景 
static lv_obj_t * time_bar = NULL;              // status bar 时间栏 
static lv_obj_t * attached_info_bar = NULL;     // status bar 附属信息栏 

static lv_obj_t * battery_percentage = NULL;     // battery  
static lv_obj_t * battery_icon = NULL;          // battery  

/* 用户配置变量 */
static uint8_t battery_per_sw = 0;      // 电量百分比显示开关, 默认关闭
static uint8_t attach_info_sw = 1;      // 状态栏附属信息开关, 默认开启
static uint8_t date_info_sw   = 1;      // 日期信息开关, 默认开启

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 创建 status bar
 ************************************************/
static void status_bar_create(void)
{
    
    /* 创建 status bar 背景 */
    LV_IMG_DECLARE(status_bar_bg);
    sbb = lv_img_create( lv_scr_act(), NULL );
    lv_img_set_src( sbb, &status_bar_bg );
    lv_obj_align( sbb, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0 );

    /* 创建 status bar 时间栏 */
    time_info_create();
    
    /* 创建 status bar 附属信息栏 */
    if ( attach_info_sw ) {
        attach_info_create();
    }

    /* 创建 电池信息 */
    battery_info_create();

}

/************************************************
 * @brief 移除 status bar
 ************************************************/
static void status_bar_remove(void)
{
    
}

/************************************************
 * @brief 获得status bar ui 
 * 
 * @return app_ui_t* 
 ************************************************/
app_ui_t *status_bar_ui_get( void )
{
    return &app_status_bar;
}

/************************************************
 * @brief 更新状态栏信息
 ************************************************/
void status_bar_time_update( void )
{
    char tbuf[20] = {0};
    RTC_INFO rtc_info;
    
    rtc_init();
    rtc_obtain_time( &rtc_info );

    sprintf( 
        tbuf, "%02d : %02d %s %d", 
        rtc_info.rtcTimeStruct.RTC_Hours, 
        rtc_info.rtcTimeStruct.RTC_Minutes,
        weekday_get( &rtc_info ),
        rtc_info.rtcDateStruct.RTC_Date
    );

    lv_label_set_text( time_bar, tbuf );
    lv_obj_align( time_bar, NULL, LV_ALIGN_CENTER, 0, 0 );
}

/************************************************
 * @brief 更新attach info
 * 
 * @param attach_info 
 ************************************************/
void status_bar_attach_info_update( const char *attach_info )
{
    char *tbuf = NULL;
    tbuf = ( char*)MALLOC( sizeof( char ) * STRLEN( attach_info ) + 13 );
    sprintf( 
        tbuf, "Cur Music : %s", 
        attach_info
    );
    lv_label_set_text( attached_info_bar, tbuf );
    lv_obj_align( attached_info_bar, NULL, LV_ALIGN_IN_LEFT_MID, 5, 4 );
}

/************************************************
 * @brief 更新bat info
 * 
 * @param now 0:正常模式 1:立即模式
 ************************************************/
void status_bar_bat_info_update( uint8_t now ) 
{
    uint16_t res = 0;
    /* 获取电池电量信息(ADC) */
    if ( !now ) {
        res = battery_info_statistic();
        if ( res == 0 )
            return;
        else if ( res == NO_BATTERY ) {
            lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_EMPTY );
            return;
        }
    } else 
        res = adc_get_value(ADC1, ADC_Channel_1);

    double probe_adc_pixel = 3.3/((double)(1<<12));
    double full_charge_adc_pixel = (FULL_CHARGE/3.3)*(1<<12);
    uint8_t left_eq = (int)(((double)res/full_charge_adc_pixel) * 100);        // 剩余电量
    
    /* 防止超限 */
    left_eq = (left_eq>100)?(100):(left_eq);
    
    if ( left_eq >= 0 && left_eq <= 9 ) {
        lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_EMPTY );
    } else if ( left_eq >= 10 && left_eq <= 24 ) {
        lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_1 );
    } else if ( left_eq >= 25 && left_eq <= 49 ) {
        lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_2 );
    } else if ( left_eq >= 50 && left_eq <= 74 ) {
        lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_3 );
    } else if ( left_eq >= 75 && left_eq <= 100 ) {
        lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_FULL );
    }
    
    printf( "Cur EQ : %d %%\n", left_eq );
    printf( "Cur Voltage : %f V\n", probe_adc_pixel * res * 2 );
    
}

/************************************************
 * @brief 电池电量取样次数
 * 
 * @return uint16_t 返回BATTERY_CALCULATE_CNT内的
 *                  ADC平均值
 ************************************************/
uint16_t battery_info_statistic( void )
{
    static uint16_t bat_adc_val[BATTERY_CALCULATE_CNT] = {0};
    static uint8_t  adc_cnt = 0;
    
    uint8_t  zero_cnt = 0;
    uint16_t sum = 0;
    
    if ( adc_cnt != BATTERY_CALCULATE_CNT ){
        bat_adc_val[adc_cnt] = adc_get_value( ADC1, ADC_Channel_1 );
        adc_cnt += 1;
        return 0;
    } else {
        for ( uint8_t i = 0; i < BATTERY_CALCULATE_CNT; i++ ) {
            if ( bat_adc_val[i] <= 10 )
                zero_cnt += 1;
            sum += bat_adc_val[i];
        }
        adc_cnt = 0;
        MEMSET( bat_adc_val, 0, BATTERY_CALCULATE_CNT );
        /* 如果adc值低于10的数量大于等于2 则认为没有电池 */
        if ( zero_cnt >= 1 ) {
            sum = 0;
            return NO_BATTERY;
        }
        return (sum/BATTERY_CALCULATE_CNT);
    }
}

/************************************************
 * @brief attach info create
 ************************************************/
void attach_info_create( void )
{
    attached_info_bar = lv_label_create( sbb, NULL );
    lv_label_set_long_mode( attached_info_bar, LV_LABEL_LONG_SROLL_CIRC );
    lv_obj_set_width( attached_info_bar, 80 );
    lv_obj_set_height( attached_info_bar, 20 );
    lv_obj_set_style_local_text_font( 
        attached_info_bar, 
        LV_LABEL_PART_MAIN, 
        LV_STATE_DEFAULT, 
        &MSGOTHIC_B_12_CN
    );
    status_bar_attach_info_update( "我Hello asdasda" );
}
void attach_info_delete( void )
{
    lv_obj_del( attached_info_bar );
}

/************************************************
 * @brief time info create
 ************************************************/
void time_info_create( void )
{
    time_bar = lv_label_create( sbb, NULL );
    lv_obj_set_style_local_text_font( 
        time_bar, 
        LV_LABEL_PART_MAIN, 
        LV_STATE_DEFAULT, 
        &lv_font_montserrat_14 
    );
    status_bar_time_update();
}
void time_info_delete( void )
{
    lv_obj_del( time_bar );
}

/************************************************
 * @brief battery info create
 ************************************************/
void battery_info_create( void )
{
    battery_icon = lv_img_create( sbb, NULL );
    lv_img_set_src( battery_icon, &LV_SYMBOL_BATTERY_1 );
    status_bar_bat_info_update( 1 );
}
void battery_info_delete( void )
{
    // lv_obj_del( time_bar );
}

