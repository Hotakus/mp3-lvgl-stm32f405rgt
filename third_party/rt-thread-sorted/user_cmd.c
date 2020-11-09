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
static void scr_test( int argc, char **args )
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
MSH_CMD_EXPORT( scr_test, screen test for current screen. );

#if USER_USE_FATFS == 1
/* declared in "lv_port_fs.c" */
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
static void fatfs( int argc, char **args )
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
MSH_CMD_EXPORT( fatfs, fatfs test for sd and spi flash );

static void list_sd_file(uint8_t argc, char **argv)
{
    
    scan_catalog( "SD_SDIO:", SCAN_OPT_CUR_DIR );
    
}
MSH_CMD_EXPORT(list_sd_file, list files in sd);

static FIL fil;
static FRESULT fres = FR_NOT_READY;
static char dev_buf[512]  = {0};
static void cat( int argc, char **args )
{
    if ( argc != 2 )
        return;

    u8 index = 0;
    u8 dev_name_len = 0;
    char *path_buf = args[1];
    
    
    while ( *(path_buf+dev_name_len) != ':' ) {
        dev_buf[dev_name_len] = *(path_buf+dev_name_len);
        dev_name_len++;
    }

    rt_kprintf( "open dev : %s\n", dev_buf );

    if ( strcmp( dev_buf, "SD_SDIO" ) == 0 ) {
        index = SD_SDIO_INDEX;
        rt_kprintf( "SD_SDIO_INDEX\n" );
    } else {
        index = SPIF_INDEX;
        rt_kprintf( "SPIF_INDEX\n" );
    }

    if ( fr_lv[index] != FR_OK )
        return;
    
    rt_kprintf( "open file: %s\n", path_buf );
    fres = f_open( &fil, path_buf, FA_OPEN_EXISTING | FA_READ );
    if ( fres != FR_OK ) {
        rt_kprintf( "open file \"%s\" error. (%d)\n", args[2], fres );
        return;
    }

    f_read( &fil, dev_buf, sizeof( dev_buf ), NULL );
    
    f_close( &fil );
    

    char *p = dev_buf;
    while ( *p++ != '\0' )
        my_putc( *p );
    
}
MSH_CMD_EXPORT( cat, show file content );

#endif

#if USER_USE_LVGL == 1
static void lvgl_fs_remount( int argc, char **args )
{
    lv_port_fs_init();          // 文件系统设备初始化
}
MSH_CMD_EXPORT( lvgl_fs_remount, remount sd and spi flash );
#endif

static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, reboot cpu);

static int clear(void)
{
    rt_kprintf("\x1b[2J\x1b[H");
    return 0;
}
MSH_CMD_EXPORT(clear,clear the terminal screen);
