#include "file_thrans.h"
#include "ff.h"
#include "pro_conf.h"
#include "ff_user.h"

#if USER_USE_LVGL == 1
#include "lv_port_fs.h"
#include "lv_fs.h"
#endif

FIL fil[2];
FRESULT fres[2];

TRANS_STAT file_thrans( const char *sd_path, const char *spif_path ) 
{
    
    fres[SD_SDIO_INDEX] = f_open( &fil[SD_SDIO_INDEX], "SD_SDIO:/file_thrans.c", FA_OPEN_EXISTING|FA_READ );
    
    printf( " %d\n", f_size( &fil[SD_SDIO_INDEX] ) );
    
    f_close( &fil[SD_SDIO_INDEX] );
    
    

    fres[SD_SDIO_INDEX] = f_open( &fil[SD_SDIO_INDEX], sd_path, FA_OPEN_EXISTING|FA_READ );
    if ( fres[SD_SDIO_INDEX] != FR_OK ) {
        
    }
    
    
    
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

