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
#include "file_trans.h"

/************************************************
 * @brief cmd for msh
 ************************************************/
#if USER_USE_FATFS == 1
/* declared in "lv_port_fs.c" */
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];

char *sd_buf   = "SD_SDIO:/file_trans.c";
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

static void scan_file(int argc, char **args)
{
    
    scan_catalog( args[1], SCAN_OPT_CUR_DIR );
    
}
MSH_CMD_EXPORT(scan_file, list files in sd);

static void file_info(int argc, char **args)
{
    show_element_info( args[1] );
}
MSH_CMD_EXPORT(file_info, get info which file);


static void rtt_cat( int argc, char **args )
{
    
    u32 btr = 0;
    u32 br = 0;
    u8 *ch_buf = NULL;
    
    if ( argc < 2 )
        rt_kprintf( "usage rtt_cat {file}\n" );
    else if ( argc == 2 ) {
        btr = sizeof(ch_buf);
    } else if ( argc == 3 ) {
        btr = atoi(args[2]);
    }
    ch_buf = (u8*)rt_malloc( sizeof(u8)*btr );
    
    FRESULT cat_fres;
    FIL     cat_fil;
    
    cat_fres = f_open( &cat_fil, args[1], FA_READ | FA_OPEN_EXISTING );
    if ( cat_fres != FR_OK ) {
        rt_kprintf( "open '%s' error. (%d)\n", get_file_name(args[1]), cat_fres );
        f_close( &cat_fil );
        return;
    }
    
    cat_fres = f_read( &cat_fil, ch_buf, btr, &br );
    if ( cat_fres != FR_OK ) {
        rt_kprintf( "read '%s' error. (%d)\n", get_file_name(args[1]), cat_fres );
        f_close( &cat_fil );
        return;
    }
    
    rt_kprintf( "get %d characters\n", br );
    
    if ( !br ) {
        u8 *pc = ch_buf;
        while( *pc != '\0' )
            my_putc( *pc++ );
    }
    
    rt_free( ch_buf );
    f_close( &cat_fil );
    
}
MSH_CMD_EXPORT( rtt_cat, show file content );


extern u8 ws[FF_MAX_SS];
void test( int argc, char **args )
{
    FIL fil_test;
    FRESULT fres = FR_NOT_READY;
    
    if ( args[1][0] == 'K' ) {
        fres = f_mkfs( "SPIF:", 0, ws, sizeof(ws) );
        if ( fres != FR_OK ) {
            rt_kprintf( "f_mkfs fres: %d\n", fres );
            return;
        }
    }
    
    if ( args[1][0] == 'F' )
        fres = f_open( &fil_test, "SPIF:/vimrc", FA_OPEN_EXISTING | FA_READ );
    if ( args[1][0] == 'S' )
        fres = f_open( &fil_test, "SD_SDIO:/h.c", FA_OPEN_EXISTING | FA_READ );
    
    if ( fres != FR_OK ) {
        rt_kprintf( "f_open fres: %d\n", fres );
        return;
    }
    
    
    rt_kprintf( "seek: %d\n", f_lseek( &fil_test, 0 ) );
    f_gets( ws, 50, &fil_test );
    rt_kprintf( "text: %s\n", ws );
    f_close( &fil_test );
}
MSH_CMD_EXPORT( test, copy file );

void rtt_cp( int argc, char **args )
{
    if ( argc != 3 )
        return;
    file_trans( args[1] , args[2] );
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
