/************************************************
 * @file user_cmd.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "stm32f4xx_conf.h"
#include "sd_sdio_conf.h"
#include "pro_conf.h"
#include "screen_conf.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ff_user.h"

/************************************************
 * @brief cmd for msh
 ************************************************/
/************************************************
 * @brief test cmd for msh
 * 
 * @param argc 
 * @param args 
 ************************************************/
void test( int argc, const char **args )
{
    if ( argc == 1 )
        DEBUG_PRINT( "%s\n",  __func__ );
    else
        DEBUG_PRINT( "%s\n",  args[1] );
}
MSH_CMD_EXPORT( test, test );

void scr_test( int argc, char **args )
{
    scr_opr_handler *scr;
    scr = scr_get_opr_handler();

    if ( argc > 2 )
        return;
    if ( argc < 2 )
        scr->clear( 0xffff );
    else {
        switch( *args[1] ) {
        case 'r':
            scr->clear( 0xf800 );
            break;
        case 'g':
            scr->clear( 0x07f0 );
            break;
        case 'b':
            scr->clear( 0x001f );
            break;
        }
    }
   
}
MSH_CMD_EXPORT( scr_test, RT_NULL );

#if USER_USE_FATFS == 1
/* declared in "lv_port_fs.c" */
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
void fatfs( int argc, char **args )
{
    if ( argc < 1 ) {
        if ( fr_lv[SD_SDIO_INDEX] != FR_OK )
            lv_port_fs_init(); 
        fatfs_test( "SD_SDIO" );
        return;
    } else if ( argc > 2 )
        return;
    DEBUG_PRINT( "into fatfs test.\n" );
    fatfs_test( args[1] );
}
MSH_CMD_EXPORT( fatfs, RT_NULL );

void mount( int argc, char **args )
{
    if ( argc != 2 ) {
        DEBUG_PRINT( "usage: mount {dev}\n" );
        return;
    }

    if ( !strcmp( args[1], "SD_SDIO" ) ) {
        if ( fr_lv[SD_SDIO_INDEX] != FR_OK ) {
            fr_lv[SD_SDIO_INDEX] = f_mount( &fs_lv[SD_SDIO_INDEX], "SD_SDIO:",  1 );
        }
    } else if ( !strcmp( args[1], "SPIF" ) ) {
        if ( fr_lv[SD_SDIO_INDEX] != FR_OK ) {
            fr_lv[SPIF_INDEX] = f_mount( &fs_lv[SPIF_INDEX], "SPIF:",  1 );
        }
    }
    
}
MSH_CMD_EXPORT( mount, RT_NULL );

void umount( int argc, char **args )
{
    
}
MSH_CMD_EXPORT( umount, RT_NULL );
#endif

#if USER_USE_LVGL == 1
void lvgl_fs_remount( int argc, char **args )
{
    lv_port_fs_init();          // 文件系统设备初始化
}
MSH_CMD_EXPORT( lvgl_fs_remount, RT_NULL );
#endif
