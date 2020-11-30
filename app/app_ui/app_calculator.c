/************************************************
 * @file app_calculator.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_calculator.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "calculator"


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void calculator_create(void);
static void calculator_remove(void);

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = calculator_create,
    .remove = calculator_remove,
};

static app_ui_t app_calculator = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_container = NULL;           // 顶层容器

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
 * @brief 创建calculator
 ************************************************/
static void calculator_create(void)
{
    obj_container = lv_cont_create( lv_scr_act(), NULL );
    lv_obj_set_size( obj_container, 320, 220 );
    lv_obj_set_pos( obj_container, 0, 20 );
    lv_obj_set_style_local_radius( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10 );
    lv_obj_set_style_local_bg_color( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x90, 0x42, 0x42 ) );
    lv_obj_set_style_local_border_width( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 2 );
    lv_obj_set_style_local_border_color( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x4d, 0x16, 0x16 ) );
    
}

/************************************************
 * @brief 移除calculator
 ************************************************/
static void calculator_remove(void)
{
    lv_obj_del( obj_container );
}

app_ui_t *calculator_ui_get( void )
{
    return &app_calculator;
}

