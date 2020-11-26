/************************************************
 * @file app_setting.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_setting.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "setting"


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void setting_create(void);
static void setting_remove(void);

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = setting_create,
    .remove = setting_remove,
};

static app_ui_t app_setting = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_container = NULL;           // icons顶层容器

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
 * @brief 创建setting
 ************************************************/
static void setting_create(void)
{


}

/************************************************
 * @brief 移除setting
 ************************************************/
static void setting_remove(void)
{
    lv_obj_del( obj_container );
}

app_ui_t *setting_ui_get( void )
{
    return &app_setting;
}

