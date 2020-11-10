#include "file_trans.h"
#include "ff.h"
#include "pro_conf.h"
#include "ff_user.h"

#if USER_USE_LVGL == 1
#include "lv_port_fs.h"
#include "lv_fs.h"
#endif

FIL t_fil[2];
FRESULT t_fres[2];

#define DATA_BUF_SIZE      1024
#define TRANS_RETRY_TIME   5

#if USER_USE_LVGL == 1
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
#else
FATFS   fs_lv[2] = {NULL, NULL};
FRESULT fr_lv[2] = {FR_NOT_READY, FR_NOT_READY};
#endif

TRANS_STAT file_trans( const char *sd_path, const char *spif_path ) 
{

    u32 dest_size = 0;
    u32 src_size  = 0;
    
//        FIL fil;
//        FRESULT fres = FR_NOT_READY;
//        u8 test_buf[20] = {0};
//        u32 br = 0;

//        f_unlink( "SD_SDIO:/sd.txt" );
//        fres = f_open( &fil, "SD_SDIO:/sd.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ );
//        if ( fres != FR_OK ) {
//            rt_kprintf( "f_open fres: %d\n", fres );
//            f_close( &fil );
//            return TRANS_STAT_OK;
//        }
//        f_lseek( &fil, 0 );
//        f_printf( &fil, "Hello sd.ad aa s\n" );
//        f_close( &fil );
//        
//        fres = f_open( &fil, "SD_SDIO:/h.txt", FA_OPEN_EXISTING|FA_READ );
//        if ( fres != FR_OK ) {
//            rt_kprintf( "f_open fres: %d\n", fres );
//            f_close( &fil );
//            return TRANS_STAT_OK;
//        }
//        f_read( &fil, test_buf, 20, &br );
//        rt_kprintf( "sd br: %d\n", br );
//        f_close( &fil );
//    
//        f_unlink( "SPIF:/spif2.txt" );
//        fres = f_open( &fil, "SPIF:/spif2.txt",  FA_WRITE |FA_OPEN_APPEND );
//        if ( fres != FR_OK ) {
//            rt_kprintf( "f_open fres: %d\n", fres );
//            f_close( &fil );
//            return TRANS_STAT_OK;
//        }
//        f_lseek( &fil, 0 );
//        f_printf( &fil, "Hello spi flash. %s\n", test_buf );
//        f_write( &fil, test_buf, br, 0 );
//        f_close( &fil );
//        
//        return TRANS_STAT_OK;
//    
//        
//        
    FILINFO t_fno[2];
    const char *file_name = get_file_name(sd_path);
    
    
    t_fres[SD_SDIO_INDEX] = f_stat( sd_path, &t_fno[SD_SDIO_INDEX] );
    if ( t_fres[SD_SDIO_INDEX] != FR_OK ) {
        DEBUG_PRINT( "sd open file \"%s\" error. (%d)\n", file_name, t_fres[SD_SDIO_INDEX] );
        return TRANS_SD_PATH_ERR;
    }
    src_size  = t_fno[SD_SDIO_INDEX].fsize;
    
    char spif_buf[50] = {0};
    sprintf( spif_buf, "%s/%s", spif_path, file_name );
    t_fres[SPIF_INDEX] = f_stat( spif_buf, &t_fno[SPIF_INDEX] );
    if ( t_fres[SPIF_INDEX] != FR_OK ) {
        dest_size = 0;
        t_fres[SPIF_INDEX] = f_open( &t_fil[SPIF_INDEX], spif_buf, FA_CREATE_NEW );
        if ( t_fres[SPIF_INDEX] != FR_OK ) {
            rt_kprintf( "create %s error.(%d)\n", file_name, t_fres[SPIF_INDEX] );
            f_close( &t_fil[SPIF_INDEX] );
            return TRANS_STAT_ERR;
        }
        f_close( &t_fil[SPIF_INDEX] );
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
    u32 cur_pos = 0;
    u8 retry = TRANS_RETRY_TIME;
    u8 *dat_buf = (u8*)rt_malloc( sizeof(u8)*DATA_BUF_SIZE );
    
    DEBUG_PRINT( "%d %d\n", ts, sdatn );
    
    if ( sdatn )
        ts += 1;
    else if ( !sdatn && !ts )
        return TRANS_STAT_ERR;
        
    
    
    do {
        /* 开启文件 */
        retry = TRANS_RETRY_TIME;
        do {
            t_fres[SD_SDIO_INDEX]   = f_open( &t_fil[SD_SDIO_INDEX], spif_buf, FA_READ | FA_OPEN_EXISTING );
            t_fres[SPIF_INDEX]      = f_open( &t_fil[SPIF_INDEX], spif_buf, FA_OPEN_EXISTING | FA_WRITE );
        } while ( (t_fres[SPIF_INDEX] != FR_OK || t_fres[SD_SDIO_INDEX] != FR_OK) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "0 : open error! ( %d %d )\n", t_fres[SPIF_INDEX], t_fres[SD_SDIO_INDEX]  );
            return TRANS_STAT_ERR;
        }
        /* 寻址 */
        retry = TRANS_RETRY_TIME;
        do {
            t_fres[SD_SDIO_INDEX]   = f_lseek( &t_fil[SD_SDIO_INDEX], cur_pos );
            t_fres[SPIF_INDEX]      = f_lseek( &t_fil[SPIF_INDEX], cur_pos );
        } while ( (t_fres[SPIF_INDEX] || t_fres[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "1 : seek error! ( %d %d )\n", t_fres[SPIF_INDEX], t_fres[SD_SDIO_INDEX]  );
            return TRANS_STAT_ERR;
        }
        /* 传输 */
        retry = TRANS_RETRY_TIME;
        do {
            t_fres[SD_SDIO_INDEX]   = f_read( &t_fil[SD_SDIO_INDEX], dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
            t_fres[SPIF_INDEX]      = f_write( &t_fil[SPIF_INDEX], dat_buf, (ts==1)?(sdatn):(DATA_BUF_SIZE), NULL );
        } while ( (t_fres[SPIF_INDEX] || t_fres[SD_SDIO_INDEX]) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 : trans error! ( %d %d )\n", t_fres[SPIF_INDEX], t_fres[SD_SDIO_INDEX]  );
            return TRANS_STAT_ERR;
        }
        /* 关闭文件 */
        f_close( &t_fil[SPIF_INDEX] );
        f_close( &t_fil[SD_SDIO_INDEX] );
        cur_pos += (ts==1)?(sdatn):(DATA_BUF_SIZE);
    } while ( --ts );
    
    rt_free( dat_buf );
    
    return TRANS_STAT_OK;
    
}






