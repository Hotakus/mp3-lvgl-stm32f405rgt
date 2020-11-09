#ifndef APP_TASK_H
#define APP_TASK_H

/* includes */
#include "stm32f4xx_conf.h"

/* definations */
#define LV_CPU_USAGE_GET (100 - lv_task_get_idle())

/* functions */
int app_create_task( void );

#endif
