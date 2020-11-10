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
    
    if ( args[1][0] == 'F' ) {
        FIL fil;
        FRESULT fres = FR_NOT_READY;
    
        fres = f_open( &fil, "SPIF:/spif.txt", FA_OPEN_ALWAYS | FA_WRITE );
        if ( fres != FR_OK ) {
            rt_kprintf( "f_open fres: %d\n", fres );
            f_close( &fil );
            return;
        }
        
        f_lseek( &fil, 0 );
        f_printf( &fil, "Hello spi flash." );
        
        f_close( &fil );
        return;
    }
    
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

//u8 ch_buf[50] = {0};
static void rtt_cat( int argc, char **args )
{
    
    u32 btr = 0;
    u32 br = 0;
    u8 *ch_buf = NULL;
    
    if ( argc < 2 ) {
        rt_kprintf( "usage rtt_cat {file}\n" );
        return;
    } else if ( argc == 2 ) {
        btr = sizeof(ch_buf);
    } else if ( argc == 3 ) {
        btr = atoi(args[2]);
    }
    rt_kprintf( "btr: %d\n", btr );
    ch_buf = (u8*)rt_malloc( sizeof(u8)*btr );
    
    FRESULT cat_fres;
    FIL     cat_fil;
    
    cat_fres = f_open( &cat_fil, args[1], FA_READ | FA_OPEN_EXISTING );
    if ( cat_fres != FR_OK ) {
        rt_kprintf( "open '%s' error. (%d)\n", get_file_name(args[1]), cat_fres );
        f_close( &cat_fil );
        return;
    }
    
    cat_fres = f_lseek( &cat_fil, 0 );
    if ( cat_fres != FR_OK ) {
        rt_kprintf( "seek begin error. (%d)\n", cat_fres );
        f_close( &cat_fil );
        return;
    }
    
    cat_fres = f_read( &cat_fil, ch_buf, btr, &br );
    if ( cat_fres != FR_OK ) {
        rt_kprintf( "read '%s' error. (%d)\n", get_file_name(args[1]), cat_fres );
        f_close( &cat_fil );
        return;
    }

    if ( br ) {
        rt_kprintf( "br: %d\n", br );
        for ( u32 i = 0; i < br; i++ )
            my_putc( ch_buf[i] );
    } else {
        rt_kprintf( "Null to read\n" );
    }
    
    rt_free( ch_buf );
    f_close( &cat_fil );
    
}
MSH_CMD_EXPORT( rtt_cat, show file content );


static void rtt_mkfs( int argc, char **args )
{

    FRESULT fres = FR_NOT_READY;
    
    u8 *works = (u8*)rt_malloc( sizeof(u8)*FF_MAX_SS );
    
    if ( args[1][0] == 'F' ) {
        fres = f_mkfs( "SPIF:", 0, works, FF_MAX_SS );
        if ( fres != FR_OK ) {
            rt_kprintf( "mkfs SPIF: %d\n", fres );
            return;
        }
    } else if ( args[1][0] == 'S' ) {
        fres = f_mkfs( "SD_SDIO:", 0, works, FF_MAX_SS );
        if ( fres != FR_OK ) {
            rt_kprintf( "mkfs SD: %d\n", fres );
            return;
        }
    }
    
    rt_kprintf( "mkfs done.\n" );
    
    rt_free( works );

}
MSH_CMD_EXPORT( rtt_mkfs, mkfs dev );

void rtt_cp( int argc, char **args )
{
    if ( argc != 3 )
        return;
    rt_kprintf( "trans begin.\n" );
    if ( file_trans( args[1] , args[2] ) != TRANS_STAT_OK )
        rt_kprintf( "trans error.\n" );
    rt_kprintf( "trans end.\n" );
}
MSH_CMD_EXPORT( rtt_cp, copy file );
#endif

#if USER_USE_LVGL == 1
static void app_boot( int argc, char **args )
{
    lv_init();
    lv_port_disp_init();        // 显示器初始化
    lv_port_indev_init();       // 输入设备初始化
#if USER_USE_FATFS == 1
    lv_port_fs_init();          // 文件系统设备初始化
#endif
    app_init(0);
}
MSH_CMD_EXPORT( app_boot, lvgl_boot );
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
