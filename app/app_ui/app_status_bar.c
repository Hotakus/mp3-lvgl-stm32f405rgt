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

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 创建 status bar
 ************************************************/
static void status_bar_create(void)
{
    
}

/************************************************
 * @brief 移除 status bar
 ************************************************/
static void status_bar_remove(void)
{
    
}

app_ui_t *status_bar_ui_get( void )
{
    return &app_status_bar;
}

