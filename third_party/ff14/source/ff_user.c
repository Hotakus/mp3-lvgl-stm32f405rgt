#include "ff_user.h"

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量     （单位KB）
//free:剩余容量     （单位KB）
//返回值:0,正常.其他,错误代码
FRESULT exf_getfree( TCHAR *path, u32 *total, u32 *free ) {
    FATFS *fs1;
    FRESULT res;
    DWORD fre_clust = 0;
    DWORD tot_sect  = 0;
    DWORD fre_sect  = 0;
    
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)path, &fre_clust, &fs1);
    if(res==0) {
        tot_sect = (fs1->n_fatent-2)*fs1->csize;    //得到总扇区数
        fre_sect = fre_clust*fs1->csize;            //得到空闲扇区数
#if FF_MAX_SS!=512                                  //扇区大小不是512字节,则转换为512字节
        tot_sect *= fs1->ssize/512;
        fre_sect *= fs1->ssize/512;
#endif
        *total = tot_sect>>1;    //单位为KB
        *free  = fre_sect>>1;    //单位为KB
     }
    return res;
}

/* 遍历指定目录 */
char pathBuf[512] = {0};
FRESULT scan_catalog( TCHAR *path, u8 opt ) {
    
    static FILINFO fno;
    static u8 root_flag = 0;
    FRESULT fres = FR_OK;
    DIR dir;
    u16 tail = 0;
    
    /* 判断根目录 */
    if ( !root_flag ) {
        strcpy( pathBuf, path );
        root_flag = 1;
    }
    
    fres = f_opendir( &dir, path );
    if ( fres != FR_OK ) {
        DEBUG_PRINT( "open dir: '%s' error.\n", path );
        return fres;
    }
    while ( 1 ) {
        fres = f_readdir( &dir, &fno );
        if ( fres != FR_OK || fno.fname[0] == NULL ) break;
        if ( fno.fattrib & AM_DIR ) {
            if ( opt&SCAN_OPT_NO_HIDEN && fno.fattrib&AM_HID )
                continue;
            if ( opt&SCAN_OPT_CUR_DIR || SCAN_OPT_ALL )
                DEBUG_PRINT("%s/%s\n", path, fno.fname);
            else {
                tail = strlen(path);
                sprintf(&pathBuf[tail], "/%s", fno.fname);
                fres = scan_catalog( pathBuf , SCAN_OPT_NO_HIDEN );                    /* Enter the directory */
                if ( fres != FR_OK ) break;
                pathBuf[tail] = 0;
            }
        } else {
            if ( opt&SCAN_OPT_NO_HIDEN ) 
                if ( fno.fattrib&AM_HID ) 
                    continue;
            DEBUG_PRINT("%s/%s (size: %d)\n", path, fno.fname, fno.fsize );
        }
    }
    
    f_closedir( &dir );
    return fres;
}

FRESULT show_element_info( TCHAR *path ) 
{
    FRESULT fr;
    FILINFO fno;

    fr = f_stat( path, &fno );
    
    switch ( fr ) {
    case FR_OK:
        DEBUG_PRINT( "Size: %d\n", fno.fsize );
        DEBUG_PRINT("Timestamp: %u/%02u/%02u, %02u:%02u\n",
               (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
               fno.ftime >> 11, fno.ftime >> 5 & 63);
        DEBUG_PRINT("Attributes: %c%c%c%c%c\n",
               (fno.fattrib & AM_DIR) ? 'D' : '-',
               (fno.fattrib & AM_RDO) ? 'R' : '-',
               (fno.fattrib & AM_HID) ? 'H' : '-',
               (fno.fattrib & AM_SYS) ? 'S' : '-',
               (fno.fattrib & AM_ARC) ? 'A' : '-');
        break;
    case FR_NO_FILE:
        DEBUG_PRINT("It is not exist.\n");

    default:
        DEBUG_PRINT("An error occured. (%d)\n", fr);
    }
    return fr;
}

#define TEST_DEV_NUM    5
FATFS test_fs[TEST_DEV_NUM];
FRESULT test_fr[TEST_DEV_NUM];
u8 ws[FF_MAX_SS] = {0};
void fatfs_test( char *dev )
{
    char buf[10] = {0};
    strcat( buf, dev );
    strcat( buf, ":" );
    
    test_fr[0] = f_mount( &test_fs[0], buf, 1 );
    if ( test_fr[0] != FR_OK ) {
        printf( "test dev mounted error. (%d)\n", test_fr[0] );
        printf( "you want to do mkfs ? ( y/n )\n" );
        u8 input = 0;
        fflush(stdout);
        fflush(stdin);
        input = getchar();
        printf( "input: %c.\n", input );
        if ( input == 'y' ) {
            printf( "mkfs start.\n" );
            test_fr[0] = f_mkfs( buf, 0, ws, FF_MAX_SS );
            if ( test_fr[0] != FR_OK ) {
                printf( "test dev mkfs error. (%d)\n", test_fr[0] );
                goto test_done;
            } else {
                printf( "test dev mkfs successfully.\n" );
            }
        } else if ( input == 'n' ) {
            goto test_done;
        }
    }
    
    u32 total_size;
    u32 free_size;
    exf_getfree( buf, &total_size, &free_size );
    printf( "total size: %d kb\n", total_size );
    printf( "free size : %d kb\n", free_size );
    
test_done:
    f_mount( NULL, buf, 0 );
    printf( "test done.\n" );
}






