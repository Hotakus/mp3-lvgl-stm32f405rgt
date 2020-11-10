#include "file_trans.h"
#include "ff.h"
#include "pro_conf.h"
#include "ff_user.h"
#include "tim.h"

#define DATA_BUF_SIZE      1024
#define TRANS_RETRY_TIME   5
#if DATA_BUF_SIZE > 1024
#error "DATA_BUF_SIZE too big"
#endif
static u8 *dat_buf = NULL;

#if USER_USE_LVGL == 1
#include "lv_port_fs.h"
#include "lv_fs.h"
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
#else
FATFS   fs_lv[2] = {NULL, NULL};
FRESULT fr_lv[2] = {FR_NOT_READY, FR_NOT_READY};
#endif

/************************************************
 * @brief copy src_path to dest_path
 * 
 * @param src_path 
 * @param dest_path 
 * @return TRANS_STAT 
 ************************************************/
TRANS_STAT file_trans( const char *src_path, const char *dest_path ) 
{
    
    const char *file_name = get_file_name(src_path);
    TRANS_STAT err = TRANS_STAT_OK;
    u32 dest_size = 0;
    u32 src_size  = 0;

    FIL src_fil, dest_fil;
    FRESULT src_fres, dest_fres;
    FILINFO t_fno[2];
    
    /* 获取源文件大小 */
    src_fres = f_stat( src_path, &t_fno[SD_SDIO_INDEX] );
    if ( src_fres != FR_OK ) {
        DEBUG_PRINT( "src open file \"%s\" error. (%d)\n", file_name, src_fres );
        return TRANS_SD_PATH_ERR;
    }
    src_size  = t_fno[SD_SDIO_INDEX].fsize;
    
    char spif_buf[50] = {0};
    sprintf( spif_buf, "%s/%s", dest_path, file_name );
    dest_fres = f_stat( spif_buf, &t_fno[SPIF_INDEX] );
    if ( dest_fres != FR_OK ) {
        dest_size = 0;
        dest_fres = f_open( &dest_fil, spif_buf, FA_CREATE_NEW );
        if ( dest_fres != FR_OK ) {
            rt_kprintf( "dest create %s error.(%d)\n", file_name, dest_fres );
            f_close( &dest_fil );
            return TRANS_STAT_OK;
        }
        f_close( &dest_fil );
    } else {
        dest_size  = t_fno[SPIF_INDEX].fsize;
    }
    
    DEBUG_PRINT( "src_size  %d\n", src_size );
    DEBUG_PRINT( "dest_size %d\n", dest_size );
    
    if ( src_size == dest_size )
        return TRANS_STAT_OK;
    else if ( src_size < dest_size ) {
        f_unlink( spif_buf );
    }

    /* 开始传输 */
    /* 根据DATA BUF SIZE 计算要传输几次 和 余数 */
    u32 ts    = src_size / DATA_BUF_SIZE;
    u32 sdatn = src_size % DATA_BUF_SIZE;
    u8 retry = TRANS_RETRY_TIME;
#if USER_USE_RTTHREAD == 1
    dat_buf = (u8*)rt_malloc( sizeof(u8)*DATA_BUF_SIZE );
#else
    dat_buf = (u8*)malloc( sizeof(u8)*DATA_BUF_SIZE );
#endif
    
    
    double ttime = 0;
    double sec   = 0;
    double speed = 0;

    DEBUG_PRINT( "%d %d\n", ts, sdatn );
    
    if ( sdatn )
        ts += 1;
    else if ( !sdatn && !ts )
        return TRANS_STAT_ERR;
 
    /* 开启文件 */
    retry = TRANS_RETRY_TIME;
    do {
        src_fres   = f_open( &src_fil, src_path, FA_READ | FA_OPEN_EXISTING );
        dest_fres = f_open( &dest_fil, spif_buf, FA_OPEN_EXISTING | FA_WRITE );
    } while ( (dest_fres != FR_OK || src_fres != FR_OK) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "0 : open error! ( %d %d )\n", dest_fres, src_fres  );
        err = TRANS_STAT_ERR;
        goto trans_end;
    }
    /* 寻址 */
    retry = TRANS_RETRY_TIME;
    do {
        src_fres   = f_lseek( &src_fil, 0 );
        dest_fres = f_lseek( &dest_fil, 0 );
    } while ( (dest_fres || src_fres) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "1 : seek error! ( %d %d )\n", dest_fres, src_fres  );
        err = TRANS_STAT_ERR;
        goto trans_end;
    }
    

    Clock_Start();
    do {
        /* 传输 */
        retry = TRANS_RETRY_TIME;
        do {
            src_fres   = f_read( &src_fil, dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
            dest_fres  = f_write( &dest_fil, dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
        } while ( (dest_fres || src_fres) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 : trans error! ( %d %d )\n", dest_fres, src_fres  );
            err = TRANS_STAT_ERR;
            goto trans_end;
        }
    } while ( --ts );

trans_end:
    f_close( &src_fil );
    f_close( &dest_fil );
#if USER_USE_RTTHREAD == 1
    rt_free( dat_buf );  
#else
    free( dat_buf );
#endif
    
    /* 计算传输速度 */
    if ( err == TRANS_STAT_OK ) {
        ttime = (double)Clock_End();
        sec   = ttime / 1000000;
        speed = (double)src_size / sec;
        printf( "\ntrans speed : %0.4f MiB/s\n", speed/(1<<20) );
    }
    
    return err;
}






