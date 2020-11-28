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
#include "oled.h"
#include "ft6236.h"
#include "vs10xx.h"

/************************************************
 * @brief cmd for msh
 ************************************************/
 
 static void lcd_test(int argc, char **args)
{
    scr_opr_handler *scr = scr_get_opr_handler();
    
    if ( argc == 2 ) {
        switch ( *args[1] ) {
        case 'r':
            scr->clear( 0xF800 );
            break;
        case 'g':
            scr->clear( 0x07F0 );
            break;
        case 'b':
            scr->clear( 0x001F );
            break;
        default:
            scr->clear( 0xF800 );
            break;
        }
    } else
        scr->clear( 0xF800 );
    
}
MSH_CMD_EXPORT(lcd_test, lcd test);
 
#if USER_USE_FATFS == 1
/* declared in "lv_port_fs.c" */

static void fatfs( int argc, char **args )
{
    FIL fil;
    FRESULT fres = FR_NOT_READY;
    
    if ( args[1][0] == 'F' ) {
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
    } else if ( args[1][0] == 'S' ) {
        fres = f_open( &fil, "SD_SDIO:/sd.txt", FA_OPEN_ALWAYS | FA_WRITE );
        if ( fres != FR_OK ) {
            rt_kprintf( "f_open fres: %d\n", fres );
            f_close( &fil );
            return;
        }
        
        f_lseek( &fil, 0 );
        f_printf( &fil, "Hello spi sd." );
        
        f_close( &fil );
        return;
    }

}
MSH_CMD_EXPORT( fatfs, fatfs test for sd and spi flash );

/************************************************
 * @brief 扫描目录下文件
 * 
 * @param argc 
 * @param args 
 ************************************************/
static void scan_file(int argc, char **args)
{
    
    scan_catalog( args[1], SCAN_OPT_CUR_DIR );
    
}
MSH_CMD_EXPORT(scan_file, list files in sd);

/************************************************
 * @brief 显示文件属性
 * 
 * @param argc 
 * @param args 
 ************************************************/
static void file_info(int argc, char **args)
{
    show_element_info( args[1] );
}
MSH_CMD_EXPORT(file_info, get info which file);

/************************************************
 * @brief 查看文件内容
 * 
 * @param argc 
 * @param args 
 ************************************************/
static void rtt_cat( int argc, char **args )
{
    
    u32 btr = 0;
    u32 br = 0;
    uint8_t *ch_buf = NULL;
    
    if ( argc < 2 ) {
        rt_kprintf( "usage rtt_cat {file}\n" );
        return;
    } else if ( argc == 2 ) {
        btr = sizeof(ch_buf);
    } else if ( argc == 3 ) {
        btr = atoi(args[2]);
    }
    rt_kprintf( "btr: %d\n", btr );
    ch_buf = (uint8_t*)MALLOC( sizeof(uint8_t)*btr );
    
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

/************************************************
 * @brief 格式化存储设备
 * 
 * @param argc 
 * @param args 
 ************************************************/
static void rtt_mkfs( int argc, char **args )
{

    FRESULT fres = FR_NOT_READY;
    
    uint8_t *works = (uint8_t*)MALLOC( sizeof(uint8_t)*FF_MAX_SS );
    
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

/************************************************
 * @brief 拷贝文件
 * 
 * @param argc 
 * @param args 
 ************************************************/
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
/************************************************
 * @brief lvgl_reboot
 * 
 * @param argc 
 * @param args 
 ************************************************/
static void lvgl_reboot( int argc, char **args )
{
    lv_port_disp_init();        // 显示器初始化
    lv_port_indev_init();       // 输入设备初始化
#if USER_USE_FATFS == 1
    lv_port_fs_init();          // 文件系统设备初始化
#endif
}
MSH_CMD_EXPORT( lvgl_reboot, lvgl_reboot );

/************************************************
 * @brief 在当前屏幕显示一张来自存储设备的图片
 ************************************************/
static void show_pic(int argc, char **args)
{
    ALIGN(4) static lv_obj_t *pic = NULL;
    
    if ( argc == 1 )
        return;
    
    if ( STRCMP( args[1], "free" ) == 0 && pic != NULL ) {
        lv_res_t res;
        uint8_t retry = 5;
        do {
            res = lv_obj_del( pic );
        } while ( --retry && res != LV_RES_INV );
        if ( res != LV_RES_INV ) {
            rt_kprintf( "delete obj error.\n" );
        }
        pic = NULL;
        return;
    }
    
    if ( pic == NULL ) {
        pic = lv_img_create( lv_scr_act(), NULL );
    }
    
    lv_img_set_src(pic, args[1] );
    lv_obj_align(pic, NULL, LV_ALIGN_CENTER, 0, 0);

}
MSH_CMD_EXPORT(show_pic, show a picture in screen from memory device.);
#endif

/************************************************
 * @brief oled 测试
 ************************************************/
static void oled_test( int argc, char **args )
{
    static uint8_t flag = 0;
    if ( !flag )
        oled_i2c_init(I2C1,400,0);
    else {
        extern uint8_t OLED_GRAM[8][128];
        oled_clean();
        show_str( (uint8_t*)"Hello!!!", FONT_8x16, 0, 64-16 );
        oled_flush_with( (uint8_t*)OLED_GRAM );
    }
}
MSH_CMD_EXPORT(oled_test , oled test);

/************************************************
 * @brief vs10xx 测试
 ************************************************/
static void vs10xx_test( int argc, char **args )
{
    vs10xx_init();
}
MSH_CMD_EXPORT(vs10xx_test , vs10xx test);

static void vs10xx_play( int argc, char **args )
{
    if ( argc == 1 )
        return;
    vs10xx_play_mp3( "SPIF:/test.mp3" );
}
MSH_CMD_EXPORT(vs10xx_play , vs10xx play);

static void mp3_get_jpeg( int argc, char **args )
{
    if ( argc == 1 )
        return;
    extract_mp3_pic( "SD_SDIO:/test.mp3","SD_SDIO:/test.jpg" );
} 
MSH_CMD_EXPORT(mp3_get_jpeg , get jpeg from mp3 file);


/************************************************
 * @brief reboot cpu
 ************************************************/
static void reboot(int argc, char **args)
{
    rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, reboot cpu);

/************************************************
 * @brief clear terminal
 ************************************************/
static int clear( int argc, char **args )
{
    rt_kprintf("\x1b[2J\x1b[H");
    return 0;
}
MSH_CMD_EXPORT(clear,clear the terminal screen);
