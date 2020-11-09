#ifndef APP_TYPEDEF_H
#define APP_TYPEDEF_H

/* includes */
#include "stm32f4xx_conf.h"


/* typedef */
typedef struct {
    void (*create) (void);
    void (*remove) (void);
} app_ui_ctl_handler;


typedef struct {
    u8 *ui_name;
    app_ui_ctl_handler *ctl_h;
} app_ui;

/* functions */


#endif
