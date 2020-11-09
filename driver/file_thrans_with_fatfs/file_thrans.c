#include "file_thrans.h"
#include "ff.h"
#include "pro_conf.h"
#include "usart.h"
#include "ff_user.h"

#if USER_USE_LVGL == 1
#include "lv_port_fs.h"
#include "lv_fs.h"

extern FRESULT fr_lv[2];
extern FATFS   fs_lv[FF_VOLUMES];
#elif USER_USE_LVGL == 0
#define SPIF_INDEX  0
#define SD_SDIO_INDEX    1
FRESULT fr_lv[2] = {0};
FATFS   fs_lv[FF_VOLUMES];
#endif

#define BUF_SIZE    1024
static  u8  temp_buf[BUF_SIZE] = {0};               // 暂存数组, 越大速度越快

/*--------------------------------------------------*/
static SD2SPIF_CHECK trans_check( void );
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* 当fr_lv[x]的值不为0时代表设备未挂载 */
static SD2SPIF_CHECK trans_check( void )
{
    if ( fr_lv[SPIF_INDEX] == fr_lv[SD_SDIO_INDEX] ) {
        if ( fr_lv[SPIF_INDEX] )
            return SD2SPIF_CHECK_NONE_MEM_DEV;
        else
            return SD2SPIF_CHECK_EXIST_ALL;
    } else {
        if ( fr_lv[SPIF_INDEX] )
            return SD2SPIF_CHECK_EXIST_SD;
        else
            return SD2SPIF_CHECK_EXIST_SPIF;
    }
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*
*   SD卡传输文件到SPI FLASH
*/
static FIL file[2];
static FRESULT res[2];
static FILINFO fno[2];
TRANS_STAT sd2spif( const char *sd_path, const char *spif_path ) {
    
    const char *sd_path_buf   = get_real_path(sd_path);
    const char *spif_path_buf = get_real_path(spif_path);
    
    /* 设备检测 */
    if ( trans_check() != SD2SPIF_CHECK_EXIST_ALL ) 
        return SD2SPIF_TRANS_CHECK_ERR;

    if ( sd_path_buf == NULL )
        return SD2SPIF_SD_PATH_ERR;
    if ( spif_path_buf == NULL ) 
        return SD2SPIF_SPIF_PATH_ERR;

    /* 开始传输 */
    u8 retry = 5;

    /* 取文件信息 */
    u32 dest_size = 0;
    u32 src_size  = 0;
    
    f_stat( sd_path, &fno[SD_SDIO_INDEX] );
    src_size  = fno[SD_SDIO_INDEX].fsize;    // 传入函数的SD卡的源文件大小

    if ( f_stat( spif_path, &fno[SPIF_INDEX] ) == FR_OK ) {
        dest_size = fno[SPIF_INDEX].fsize;
        /* 若两个文件不同，则删除之前的文件, 否则表示文件已存在，不用再次传输 */
        if ( dest_size > src_size )
            f_unlink( spif_path );
        if ( dest_size == src_size )
            return SD2SPIF_TRANS_STAT_OK;
    } else {
        dest_size = 0;
    }

    u32 br  = 0;                                // 记录读取和写入的位置
    u16 buf_size = BUF_SIZE;
    
    /* 判断BUF_SIZE > dest_size的情况 */
    if ( buf_size > src_size )
        buf_size =  src_size;
    
    u32 src_ssurplus  = src_size%buf_size;      // 取余
    u32 src_num       = src_size/buf_size;      // 取temp_buf大小的倍数

    do {
        res[SD_SDIO_INDEX]   = f_open( &file[SD_SDIO_INDEX]  , sd_path  , FA_OPEN_EXISTING|FA_READ );
        res[SPIF_INDEX] = f_open( &file[SPIF_INDEX], spif_path, FA_OPEN_ALWAYS|FA_WRITE ); 
    } while ( (res[SPIF_INDEX] || res[SD_SDIO_INDEX]) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "1 sd to spi flash occured a error! ( %d %d )\n", res[SPIF_INDEX], res[SD_SDIO_INDEX]  );
        return SD2SPIF_TRANS_STAT_ERR;
    }
    
    /* 传输主体 */
    do {
        retry = 5;
        do {
            res[SD_SDIO_INDEX]   = f_lseek( &file[SD_SDIO_INDEX], br );
            res[SPIF_INDEX] = f_lseek( &file[SPIF_INDEX], br );
        } while ( (res[SPIF_INDEX] || res[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 sd to spi flash occured a error! ( %d %d )\n", res[SPIF_INDEX], res[SD_SDIO_INDEX]  );
            return SD2SPIF_TRANS_STAT_ERR;
        }
        retry = 5;
        do {
            res[SD_SDIO_INDEX]   = f_read( &file[SD_SDIO_INDEX], temp_buf, buf_size, NULL );
            res[SPIF_INDEX] = f_write( &file[SPIF_INDEX], temp_buf, buf_size, NULL );
        } while ( (res[SPIF_INDEX] || res[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "3 sd to spi flash occured a error! ( %d %d )\n", res[SPIF_INDEX], res[SD_SDIO_INDEX]  );
            return SD2SPIF_TRANS_STAT_ERR;
        }
        br += buf_size;
    } while ( --src_num );
    
    br -= buf_size;
    
    /* 判断有余数的情况 */
    if ( !src_num && src_ssurplus ) {
        retry = 5;
        br += src_ssurplus;
        do {
            res[SD_SDIO_INDEX]   = f_lseek( &file[SD_SDIO_INDEX], br );
            res[SPIF_INDEX] = f_lseek( &file[SPIF_INDEX], br );
        } while ( (res[SPIF_INDEX] || res[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "4 sd to spi flash occured a error! ( %d %d )\n", res[SPIF_INDEX], res[SD_SDIO_INDEX]  );
            return SD2SPIF_TRANS_STAT_ERR;
        }
        retry = 5;
        do {
            res[SD_SDIO_INDEX]   = f_read( &file[SD_SDIO_INDEX], temp_buf, buf_size, NULL );
            res[SPIF_INDEX] = f_write( &file[SPIF_INDEX], temp_buf, buf_size, NULL );
        } while ( (res[SPIF_INDEX] || res[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "5 sd to spi flash occured a error! ( %d %d )\n", res[SPIF_INDEX], res[SD_SDIO_INDEX]  );
            return SD2SPIF_TRANS_STAT_ERR;
        }
    }
    
    f_close( &file[SPIF_INDEX] );
    f_close( &file[SD_SDIO_INDEX] );
    
    return SD2SPIF_TRANS_STAT_OK;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

