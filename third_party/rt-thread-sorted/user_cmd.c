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
#include "file_thrans.h"

/************************************************
 * @brief cmd for msh
 ************************************************/
#if USER_USE_FATFS == 1
/* declared in "lv_port_fs.c" */
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];

char *sd_buf   = "SD_SDIO:/file_thrans.c";
char *spif_buf = "SPIF:";

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

static void list_file(int argc, char **args)
{
    
    scan_catalog( args[1], SCAN_OPT_CUR_DIR );
    
}
MSH_CMD_EXPORT(list_file, list files in sd);

static char dev_buf[512]  = {0};
static void rtt_cat( int argc, char **args )
{
    if ( argc != 2 )
        return;

    u8 index = 0;
    u8 dev_name_len = 0;
    char *path_buf = args[1];
    static FIL fil;
    static FRESULT fres = FR_NOT_READY;
    
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
    while ( *p != '\0' )
        my_putc( *p++ );
}
MSH_CMD_EXPORT( rtt_cat, show file content );

void rtt_cp( int argc, char **args )
{
//    FATFS   fss[2];
//    FRESULT res[2];
    
    rt_kprintf( get_file_name( args[1] ) );
    
    file_thrans(0,0);
    
}
MSH_CMD_EXPORT( rtt_cp, copy file );
#endif

#if USER_USE_LVGL == 1
static void lvgl_fs_remount( int argc, char **args )
{
    lv_port_fs_init();          // 文件系统设备初始化
}
MSH_CMD_EXPORT( lvgl_fs_remount, remount sd and spi flash );
#endif

static void reboot(int argc, char **args)
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
