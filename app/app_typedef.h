#ifndef APP_TYPEDEF_H
#define APP_TYPEDEF_H

/* includes */
#include "stm32f4xx_conf.h"

#define APP_UI_NUM      10      // app 的最大UI数量
#define APP_UI_LAYER    5       // app 的最大UI层级

/* typedef */
/* ui控制句柄 */
typedef struct {
    void (*create) (void);
    void (*remove) (void);
} app_ui_ctl_handler;

/* ui 属性结构体 */
typedef struct {
    char *ui_name;
    app_ui_ctl_handler *ctl_h;
} app_ui_t;

/* UI管理块 */
typedef struct {
    u8 reg_ui_num;                              // 已注册ui数量
    u8 cur_ui_layer;                            // 当前显示的ui层级
    u8 next_ui_layer;                           // 下一个显示的ui层级
    app_ui_t *app_ui_s[APP_UI_NUM];             // 已注册ui块
    app_ui_t *app_ui_layer[APP_UI_LAYER];       // ui层级管理块
} app_ui_managed_t;

/* functions */


#endif
