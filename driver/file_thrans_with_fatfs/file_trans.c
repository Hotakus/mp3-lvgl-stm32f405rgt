#include "file_trans.h"
#include "ff.h"
#include "pro_conf.h"
#include "ff_user.h"
#include "tim.h"

#if USER_USE_LVGL == 1
#include "lv_port_fs.h"
#include "lv_fs.h"
#endif


#define DATA_BUF_SIZE      1024
#define TRANS_RETRY_TIME   5
#if DATA_BUF_SIZE > 1024
#error "DATA_BUF_SIZE too big"
#endif

#if USER_USE_LVGL == 1
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
#else
FATFS   fs_lv[2] = {NULL, NULL};
FRESULT fr_lv[2] = {FR_NOT_READY, FR_NOT_READY};
#endif

TRANS_STAT file_trans( const char *sd_path, const char *spif_path ) 
{

    FIL sd_fil, spif_fil;
    FRESULT sd_fres, spif_fres;
    u32 dest_size = 0;
    u32 src_size  = 0;
    
    TRANS_STAT err = TRANS_STAT_OK;
    FILINFO t_fno[2];
    const char *file_name = get_file_name(sd_path);
    
    sd_fres = f_stat( sd_path, &t_fno[SD_SDIO_INDEX] );
    if ( sd_fres != FR_OK ) {
        DEBUG_PRINT( "src open file \"%s\" error. (%d)\n", file_name, sd_fres );
        return TRANS_SD_PATH_ERR;
    }
    src_size  = t_fno[SD_SDIO_INDEX].fsize;
    
    char spif_buf[50] = {0};
    sprintf( spif_buf, "%s/%s", spif_path, file_name );
    spif_fres = f_stat( spif_buf, &t_fno[SPIF_INDEX] );
    if ( spif_fres != FR_OK ) {
        dest_size = 0;
        spif_fres = f_open( &spif_fil, spif_buf, FA_CREATE_NEW );
        if ( spif_fres != FR_OK ) {
            rt_kprintf( "dest create %s error.(%d)\n", file_name, spif_fres );
            f_close( &spif_fil );
            return TRANS_STAT_OK;
        }
        f_close( &spif_fil );
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

    double ttime;
    double sec  ;
    double speed;
    
    u8 retry = TRANS_RETRY_TIME;
#if USER_USE_RTTHREAD == 1
    u8 *dat_buf = dat_buf = (u8*)rt_malloc( sizeof(u8)*DATA_BUF_SIZE );
#else
    u8 buf[DATA_BUF_SIZE] = {0};
    u8 * dat_buf = buf;
#endif
    
    DEBUG_PRINT( "%d %d\n", ts, sdatn );
    
    if ( sdatn )
        ts += 1;
    else if ( !sdatn && !ts )
        return TRANS_STAT_ERR;
 
    /* 开启文件 */
    retry = TRANS_RETRY_TIME;
    do {
        sd_fres   = f_open( &sd_fil, sd_path, FA_READ | FA_OPEN_EXISTING );
        spif_fres = f_open( &spif_fil, spif_buf, FA_OPEN_EXISTING | FA_WRITE );
    } while ( (spif_fres != FR_OK || sd_fres != FR_OK) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "0 : open error! ( %d %d )\n", spif_fres, sd_fres  );
        err = TRANS_STAT_ERR;
        goto trans_end;
    }
    /* 寻址 */
    retry = TRANS_RETRY_TIME;
    do {
        sd_fres   = f_lseek( &sd_fil, 0 );
        spif_fres = f_lseek( &spif_fil, 0 );
    } while ( (spif_fres || sd_fres) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "1 : seek error! ( %d %d )\n", spif_fres, sd_fres  );
        err = TRANS_STAT_ERR;
        goto trans_end;
    }
    

    Clock_Start();
    do {
        /* 传输 */
        retry = TRANS_RETRY_TIME;
        do {
            sd_fres   = f_read( &sd_fil, dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
            spif_fres = f_write( &spif_fil, dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
        } while ( (spif_fres || sd_fres) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 : trans error! ( %d %d )\n", spif_fres, sd_fres  );
            err = TRANS_STAT_ERR;
            goto trans_end;
        }
    } while ( --ts );

trans_end:
    f_close( &sd_fil );
    f_close( &spif_fil );
#if USER_USE_RTTHREAD == 1
    rt_free( dat_buf );  
#endif
    
    if ( err == TRANS_STAT_OK ) {
        ttime = (double)Clock_End();
        sec   = ttime / 1000000;
        speed = (double)src_size / sec;
        printf( "\ntrans speed : %0.4f MiB/s\n", speed/(1<<20) );
    }
    
    return err;
}






