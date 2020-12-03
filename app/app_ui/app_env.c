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

static lv_obj_t * info_tabv = NULL;
lv_obj_t * sys_info_tab = NULL;
lv_obj_t * tmp_rh_tab   = NULL;


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

    info_tabv = lv_tabview_create( obj_cont, NULL );
    lv_obj_set_size( info_tabv, 320, 220 );
    lv_obj_set_pos( info_tabv, 0, 20 );
    sys_info_tab = lv_tabview_add_tab( info_tabv, "S\ny\ns" );
    tmp_rh_tab   = lv_tabview_add_tab( info_tabv, "E\nv\nn" );
    lv_tabview_set_btns_pos( info_tabv, LV_TABVIEW_TAB_POS_LEFT );

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

/************************************************
 * @brief 显示system info
 ************************************************/
static void show_system_info( void )
{
    static icon_t cpu = {
        .rel_path = "",
        .pr_path  = "",
    };
    static icon_t mem = {
        .rel_path = "",
        .pr_path  = "",
    };

    cpu.btn = lv_imgbtn_create( sys_info_tab, NULL );
    lv_imgbtn_set_src( cpu.btn, LV_BTN_STATE_RELEASED, cpu.rel_path );
    lv_imgbtn_set_src( cpu.btn, LV_BTN_STATE_PRESSED, cpu.pr_path );
    lv_obj_align( mem.btn, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10 );

    mem.btn = lv_imgbtn_create( tmp_rh_tab, NULL );
    lv_imgbtn_set_src( mem.btn, LV_BTN_STATE_RELEASED, mem.rel_path );
    lv_imgbtn_set_src( mem.btn, LV_BTN_STATE_PRESSED, mem.pr_path );
    lv_obj_align( mem.btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -34 );
}

/************************************************
 * @brief 显示温湿度信息
 ************************************************/
static void show_temp_rh_info( void )
{
    
}
