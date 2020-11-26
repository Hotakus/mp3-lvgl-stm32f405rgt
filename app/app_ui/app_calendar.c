/************************************************
 * @file app_calendar.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_calendar.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "calendar"


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void calendar_create(void);
static void calendar_remove(void);

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = calendar_create,
    .remove = calendar_remove,
};

static app_ui_t app_calendar = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_container = NULL;             // 顶层容器
static lv_obj_t * calendar = NULL;                  // 日历

static lv_obj_t * return_btn = NULL;                // 返回按键
static lv_obj_t * return_label = NULL;              

static lv_obj_t * festive_info = NULL;              // 节假日信息
static lv_obj_t * festive_label = NULL;

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 事件句柄
 * 
 * @param obj       
 * @param event 
 ************************************************/
static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    app_ui_t * ui = NULL;
    switch (event) {
    case LV_EVENT_CLICKED:

        break;
    default:
        break;
    }

}

/************************************************
 * @brief 创建calendar
 ************************************************/
static void calendar_create(void)
{

    obj_container = lv_cont_create( lv_scr_act(), NULL );
    lv_obj_set_size( obj_container, 320, 220 );
    lv_obj_set_pos( obj_container, 0, 20 );
    lv_obj_set_style_local_bg_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    lv_obj_set_style_local_border_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );

    festive_info = lv_page_create( obj_container, NULL );
    lv_obj_set_size( festive_info, 100, 170 );
    lv_obj_align( festive_info, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0 );
    lv_obj_set_event_cb( festive_info, event_handler );
    festive_label = lv_label_create( festive_info, NULL );
    lv_label_set_text( festive_label, "festive." );

    return_btn = lv_btn_create( obj_container, NULL );
    lv_obj_set_size( return_btn, 100, 50 );
    lv_obj_align( return_btn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0 );
    lv_obj_set_event_cb( return_btn, event_handler );
    return_label = lv_label_create( return_btn, NULL );
    lv_label_set_text( return_label, "return."LV_SYMBOL_NEW_LINE ); 

    calendar = lv_calendar_create( obj_container, NULL );
    lv_obj_set_size( calendar, 220, 220 );
    lv_obj_set_pos( calendar, 0, 0 );


    lv_obj_set_event_cb( calendar, event_handler );
}

/************************************************
 * @brief 移除calendar
 ************************************************/
static void calendar_remove(void)
{
    lv_obj_del( obj_container );
}

app_ui_t *calendar_ui_get( void )
{
    return &app_calendar;
}

