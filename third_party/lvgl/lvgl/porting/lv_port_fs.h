/**
 * @file lv_port_fs_templ.h
 *
 */

 /*Copy this file as "lv_port_fs.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_FS_TEMPL_H
#define LV_PORT_FS_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/
#define SPIF_INDEX  0
#define SD_INDEX    1
/**********************
 *      TYPEDEFS
 **********************/
 typedef enum {
    DEVICE_SPI_FLASH  = 0,
    DEVICE_SD_CARD       ,
 } FS_DEVICE;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

void lv_port_fs_init(void);
void fs_deinit( FS_DEVICE device );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_PORT_FS_TEMPL_H*/

#endif /*Disable/Enable content*/