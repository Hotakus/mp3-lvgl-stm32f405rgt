/************************************************
 * @file app_env.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_env.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "env"

#define SIDE_BTN_WIDTH  40

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void env_create(void);
static void env_remove(void);

static void show_system_info( void );
static void show_temp_rh_info( void );

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = env_create,
    .remove = env_remove,
};

static app_ui_t app_env = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_cont = NULL;           // icons顶层容器

static lv_obj_t * info_cont = NULL;

static icon_t system_info;
static icon_t temp_rh_info;

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

    switch (event) {
    case LV_EVENT_CLICKED:
        lv_btn_set_state( obj, LV_BTN_STATE_CHECKED_RELEASED );
        lv_btn_set_state( obj, LV_BTN_STATE_CHECKED_PRESSED );
        if ( obj == system_info.btn )
            show_system_info();
        else if ( obj == temp_rh_info.btn )
            show_temp_rh_info();
        break;
    default:
        break;
    }

}

/************************************************
 * @brief 创建env
 ************************************************/
static void env_create(void)
{

    obj_cont = lv_cont_create( lv_scr_act(), NULL );
    lv_obj_set_size( obj_cont, 320, 220 );
    lv_obj_set_pos( obj_cont, SIDE_BTN_WIDTH, 20 );
    lv_obj_set_style_local_bg_opa( obj_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    lv_obj_set_style_local_border_opa( obj_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );

    system_info.btn = lv_btn_create( obj_cont, NULL );
    lv_obj_set_size( system_info.btn, SIDE_BTN_WIDTH, LV_VER_RES_MAX/2 );
    lv_btn_set_checkable( system_info.btn, true );
    lv_obj_set_event_cb( system_info.btn, event_handler );
    system_info.label = lv_label_create( obj_cont, NULL );

    temp_rh_info.btn = lv_btn_create( obj_cont, system_info.btn );
    lv_obj_set_size( temp_rh_info.btn, SIDE_BTN_WIDTH, LV_VER_RES_MAX/2 );
    lv_btn_set_checkable( temp_rh_info.btn, true );
    lv_obj_set_event_cb( temp_rh_info.btn, event_handler );
    temp_rh_info.label = lv_label_create( obj_cont, NULL );

    info_cont = lv_cont_create( obj_cont, NULL );
    lv_obj_set_size( info_cont, LV_HOR_RES_MAX-SIDE_BTN_WIDTH, 220 );
    lv_obj_set_pos( info_cont, SIDE_BTN_WIDTH, 20 );


    show_system_info();

}

/************************************************
 * @brief 移除env
 ************************************************/
static void env_remove(void)
{
    lv_obj_del( obj_cont );
}

app_ui_t *env_ui_get( void )
{
    return &app_env;
}

static void show_system_info( void )
{

}

static void show_temp_rh_info( void )
{
    
}
