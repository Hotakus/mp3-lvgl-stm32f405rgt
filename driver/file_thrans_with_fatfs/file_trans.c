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

#define DATA_BUF_SIZE      128
#define TRANS_RETRY_TIME   5


TRANS_STAT file_trans( const char *sd_path, const char *spif_path ) 
{

    const char *file_name = get_file_name(sd_path);
    FILINFO t_fno[2];
    u8 dat_buf[DATA_BUF_SIZE] = {0};
    
    u32 dest_size = 0;
    u32 src_size  = 0;

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
    
    DEBUG_PRINT( "size check done.\n" );
    
    DEBUG_PRINT( "trans..." );
    /* 开始传输 */
    /* 根据DATA BUF SIZE 计算要传输几次 和 余数 */
    u32 ts    = src_size / DATA_BUF_SIZE;
    u32 sdatn = src_size % DATA_BUF_SIZE;
    u32 cur_pos = 0;
    u8 retry = TRANS_RETRY_TIME;
    
    DEBUG_PRINT( "%d %d\n", ts, sdatn );
    
    if ( sdatn )
        ts += 1;
    else if ( !sdatn && !ts )
        return TRANS_STAT_ERR;
        
    retry = TRANS_RETRY_TIME;
    do {
        t_fres[SD_SDIO_INDEX]   = f_open( &t_fil[SD_SDIO_INDEX], spif_buf, FA_READ | FA_OPEN_EXISTING );
        t_fres[SPIF_INDEX]      = f_open( &t_fil[SPIF_INDEX], spif_buf, FA_WRITE | FA_OPEN_ALWAYS );
    } while ( (t_fres[SPIF_INDEX] || t_fres[SD_SDIO_INDEX]) && --retry );
    if ( !retry ) {
        DEBUG_PRINT( "0 : open error! ( %d %d )\n", t_fres[SPIF_INDEX], t_fres[SD_SDIO_INDEX]  );
        return TRANS_STAT_ERR;
    }
    
    do {
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
        cur_pos += (ts==1)?(sdatn):(DATA_BUF_SIZE);
    } while ( --ts );
    
    DEBUG_PRINT( "out.\n" );
    
    f_close( &t_fil[SD_SDIO_INDEX] );
    f_close( &t_fil[SPIF_INDEX] );
    
    DEBUG_PRINT( "done.\n" );
    
    return TRANS_STAT_OK;
    
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

