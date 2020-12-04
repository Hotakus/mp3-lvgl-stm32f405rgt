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

#if defined(RT_USING_FINSH)

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
static void lvgl_mem(  )
{
    lv_mem_monitor_t mem;
    lv_mem_monitor( &mem );

    DEBUG_PRINT( "lv free_size  : %d KiB\n", mem.free_size );
    DEBUG_PRINT( "lv total_size : %d KiB\n", mem.total_size );
    DEBUG_PRINT( "lv max_used   : %d KiB\n", mem.max_used );
}
MSH_CMD_EXPORT( lvgl_mem, lvgl_mem );
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

void dma_test(void)
{
    uint8_t buf_src[512];
    uint8_t buf_dst[512];

    RCC_AHB2PeriphClockCmd( RCC_AHB2Periph_RNG, ENABLE );
    RNG_Cmd( ENABLE );

    for ( uint16_t i = 0; i < 512; i++ ) 
        buf_src[i] = RNG_GetRandomNumber()%0xFF;

    MEMSET( buf_dst, 0, sizeof(buf_dst) );

    dma_mem2mem(
        DMA1, DMA1_Stream0, DMA_Channel_1, 
        DMA_Priority_VeryHigh,
        buf_src, buf_dst, 512
    );

    while( DMA_GetCurrDataCounter( DMA1_Stream1 ) );
    DMA_Cmd( DMA1_Stream1, DISABLE );

    if ( MEMCMP( buf_src, buf_src, 512 ) == 0 ) {
        DEBUG_PRINT( "DMA transmited ok\n" );
    } else
        DEBUG_PRINT( "DMA transmited error\n" );

}
MSH_CMD_EXPORT(dma_test, dma_test);

#endif





