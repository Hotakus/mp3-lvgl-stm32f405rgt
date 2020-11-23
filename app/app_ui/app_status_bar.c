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

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME   "status_bar"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void status_bar_create(void);
static void status_bar_remove(void);

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
    time_bar = lv_label_create( lv_scr_act(), NULL );
    lv_obj_set_style_local_text_font( 
        time_bar, 
        LV_LABEL_PART_MAIN, 
        LV_STATE_DEFAULT, 
        &lv_font_montserrat_14 
    );
    status_bar_update();
    /* 创建 status bar 附属信息栏 */
    attached_info_bar = lv_label_create( lv_scr_act(), NULL );
    lv_label_set_long_mode( attached_info_bar, LV_LABEL_LONG_SROLL_CIRC );
    lv_obj_set_width( attached_info_bar, 80 );
    lv_obj_set_height( attached_info_bar, 32 );
    lv_obj_set_pos( attached_info_bar, 0, 20 );
    lv_obj_set_style_local_text_font( 
        attached_info_bar, 
        LV_LABEL_PART_MAIN, 
        LV_STATE_DEFAULT, 
        &MSGOTHIC_B_12_CN
    );
    status_bar_attach_info_update( "我" );

    
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
void status_bar_update( void )
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
    lv_obj_align( time_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0 );
}

/************************************************
 * @brief 更新attach info
 * 
 * @param attach_info 
 ************************************************/
void status_bar_attach_info_update( const char *attach_info )
{
    char tbuf[20] = {0};
    sprintf( 
        tbuf, "Cur Music : %s", 
        attach_info
    );
    lv_label_set_text( attached_info_bar, tbuf );
    lv_obj_align( attached_info_bar, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0 );
}
