/************************************************
 * @file app_typedef.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef APP_TYPEDEF_H
#define APP_TYPEDEF_H

/* includes */
#include "sys_conf.h"
#include "lvgl.h"

#define APP_UI_NUM      50      // app 的最大UI数量
#define APP_UI_LAYER    5       // app 的最大UI层级(深度)

typedef struct icon_t{
    const char *en_name;    // icon英文名
    const char *cn_name;    // icon中文名
    const char *rel_path;   // 释放状态的icon
    const char *pr_path;    // 按压状态的icon
    lv_obj_t * btn;         // lv_obj_t 按键对象
    lv_obj_t * label;       // lv_obj_t 标签对象
} icon_t;

/* typedef */
/* ui控制句柄 */
/* 每个不同的ui都有自己的ui控制句柄 */
typedef struct app_ui_ctl_handler{
    void (*create) (void);
    void (*remove) (void);
} app_ui_ctl_handler;

/* ui 属性结构体 */
typedef struct app_ui_t{
    char *ui_name;
    app_ui_ctl_handler *ctl_h;
} app_ui_t;

/* UI管理块 */

typedef struct app_ui_managed_t{
    uint8_t     reg_ui_num;                  // 已注册ui数量
    uint8_t     cur_ui_layer;                // 当前显示的ui层级
    uint8_t     next_ui_layer;               // 下一个显示的ui层级
    uint8_t     reserve;                     // 1Byte对齐保留字节
    app_ui_t    *app_ui_s[APP_UI_NUM];       // 已注册ui块
    app_ui_t    *app_ui_layer[APP_UI_LAYER]; // ui层级管理块
} app_ui_managed_t;

/* functions */


#endif
