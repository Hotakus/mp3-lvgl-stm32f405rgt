#include "ff_user.h"

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量     （单位KB）
//free:剩余容量     （单位KB）
//返回值:0,正常.其他,错误代码
FRESULT exf_getfree( TCHAR *path, uint32_t *total, uint32_t *free ) {
    FATFS *fs1;
    FRESULT res;
    DWORD fre_clust = 0;
    DWORD tot_sect  = 0;
    DWORD fre_sect  = 0;
    
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)path, &fre_clust, &fs1);
    if( res == FR_OK) {
        tot_sect = (fs1->n_fatent-2)*fs1->csize;    //得到总扇区数
        fre_sect = fre_clust*fs1->csize;            //得到空闲扇区数
#if FF_MAX_SS != 512                                //扇区大小不是512字节,则转换为512字节
        tot_sect *= fs1->ssize>>9;
        fre_sect *= fs1->ssize>>9;
#endif
        *total = tot_sect>>1;    //单位为KB
        *free  = fre_sect>>1;    //单位为KB
    } else {
        *total = 0;
        *free  = 0;
    }
    return res;
}

/* 遍历指定目录 */
char pathBuf[512] = {0};
FRESULT scan_catalog( TCHAR *path, SCAN_OPT opt ) {
    
    static FILINFO fno;
    static uint8_t root_flag = 0;
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
                tail = STRLEN(path);
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

/************************************************
 * @brief 显示指定文件的信息
 * 
 * @param path 
 * @return FRESULT 
 ************************************************/
FRESULT show_file_info( TCHAR *path ) 
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



/************************************************
 * @brief 根据指定的文件格式，列出指定目录下所有的
 * 此类文件
 * 
 * @param dir_path 要扫描的目录
 * @param file_type 要提取的文件类型
 * @param create_flag 是否要在目录下创建文件列表文件
 * 1 : create
 * 2 : no create
 * @return uint16_t 返回符和条件的文件数量
 ************************************************/
uint16_t list_indic_file( const char *dir_path, const char *file_type, uint8_t create_flag )
{

    DIR *dir = NULL;
    FILINFO *fno = NULL;
    FIL *list_file = NULL;
    FRESULT fres;
    
    char *buf = NULL;
    char *p_buf = NULL;
    uint16_t p_len = 0;
    uint16_t file_cnt = 0;  // 检测到的指定文件的数量

    /* 判断是不是目录 */
    fno = (FILINFO*)MALLOC(sizeof(FILINFO));
    if ( f_stat(dir_path, fno ) == FR_OK ) {
        DEBUG_PRINT( "Your open is not a directory.\n" );
        FREE(fno);
        return !file_cnt;
    }

    dir = (DIR*)MALLOC(sizeof(DIR));
    list_file = (FIL*)MALLOC(sizeof(FIL));

    /* 打开文件夹 */
    fres = f_opendir( dir, dir_path );
    if ( fres != FR_OK ) {
        DEBUG_PRINT( "Open dir occured an error. (%d)\n", fres );
        goto to_free;
    }

    /* 如果 create_flag == 1 则创建list文件 */
    if ( create_flag ) {
        buf = (char*)MALLOC( sizeof(char)*(6 + STRLEN(file_type) + STRLEN(dir_path)) );
        sprintf( buf, "%s/%s.list", dir_path, file_type );

        DEBUG_PRINT( "%s\n", buf );
        /* 清除上个list文件 */
        if ( f_stat( buf, fno ) == FR_OK )
            f_unlink( buf );
        /* 创建list文件 */
        fres = f_open( list_file, buf, FA_CREATE_ALWAYS );
        if ( fres != FR_OK ) {
            DEBUG_PRINT( "Create '%s' file list occured an error. (%d)\n", buf, fres );
            goto to_free;
        }
        f_close(list_file);
    }

    /* 遍历目录 */
    if ( create_flag ) {
        fres = f_open( list_file, buf, FA_OPEN_EXISTING | FA_WRITE );
        if ( fres != FR_OK ) {
            DEBUG_PRINT( "open list file '%s' occured an error. (%d)\n", buf, fres );
            goto to_free;
        }
    }

    while ( 1 ) {
        fres = f_readdir( dir, fno );
        if ( fres != FR_OK || !fno->fname[0] )
            break;
        if ( fno->fattrib & AM_DIR )
            continue;

        if ( (MEMCMP(get_file_type(fno->fname), file_type, STRLEN(file_type) )==0) ) {
            p_len = (STRLEN(dir_path) + STRLEN(fno->fname) + 3);
            p_buf = (char*)MALLOC(sizeof(char) * p_len);
            sprintf( p_buf, "%s/%s\n\r", dir_path, fno->fname );
            f_write( list_file, p_buf, p_len, NULL );
            FREE(p_buf);
            file_cnt++;
        }

    }

    if ( create_flag ) {
        f_close(list_file);
    }

    DEBUG_PRINT( "file_cnt ： %d\n", file_cnt );

    /* 释放资源 */
to_free:
    f_closedir( dir );
    FREE(dir);
    FREE(fno);
    
    FREE(list_file);
    FREE( buf );

    return file_cnt;
}



/************************************************
 * @brief 得到path的某个元素
 * 
 * @param path 
 * @param pt 
 * @return const char* 元素的首地址
 ************************************************/
const char *path_get( const char *path, PATH_TYPE pt )
{
    switch (pt) {
    case PATH_DEV_NAME:
        return get_dev_name(path);
    case PATH_MAIN:
        return get_real_path(path);
    case PATH_FILE_NAME:
        return get_file_name(path);
    case PATH_FILE_TYPE:
        return get_file_type(path);
    default:
        return NULL;
    }
}

void ptest( int argc, const char **argv )
{
    // DEBUG_PRINT( "%s\n", get_real_path( "SD_SDIO:/test.jpg" ) );
    // DEBUG_PRINT( "%s\n", get_file_name( "SD_SDIO:/test.jpg" ) );
    // DEBUG_PRINT( "%s\n", get_dev_name( "SD_SDIO:/test.jpg" ) );
    // DEBUG_PRINT( "%s\n", get_file_type( "SD_SDIO:/test.jpg" ) );
    // DEBUG_PRINT( "%s\n", get_real_path( argv[1] ) );
    // DEBUG_PRINT( "%s\n", get_file_name( argv[1] ) );
    // DEBUG_PRINT( "%s\n", get_dev_name( argv[1] ) );
    // DEBUG_PRINT( "%s\n", get_file_type( argv[1] ) );
    // DEBUG_PRINT( "%s\n", path_get( argv[1], PATH_DEV_NAME ) );
    // DEBUG_PRINT( "%s\n", path_get( argv[1], PATH_FILE_NAME ) );
    // DEBUG_PRINT( "%s\n", path_get( argv[1], PATH_FILE_TYPE ) );
    // DEBUG_PRINT( "%s\n", path_get( argv[1], PATH_MAIN ) );

    list_indic_file( "SD_SDIO:/", "mp3", 1 );

}
MSH_CMD_EXPORT( ptest, ptest );

const char * get_dev_name(const char * path)
{
    if ( path == NULL ) 
        return NULL;

    const char *psp = path;
    const char *name_sp = path;
    uint8_t name_len = 0;
    static char *buf = NULL;
    
    while (1) {
        if ( *psp != ':' ){
            psp++;
            name_len++;
        } else if ( *psp == ':' )
            break;
    }

    if ( buf != NULL )
        FREE(buf);
    buf = (char*)MALLOC(sizeof(char) * name_len);
    for ( uint8_t i = 0; i < name_len; i++ )
        buf[i] = *name_sp++;
    return buf;
}

const char * get_file_type(const char * path)
{
    if ( path == NULL ) 
        return NULL;
    
    uint16_t len = STRLEN(path);
    const char* pep = path+len-1;

    if ( *pep == '.' || *pep == '/' ) 
        return NULL;

    while ( 1 ) {
        if ( *pep == '.' ) {
            return ++pep;
        } else
            pep--;
        if ( *pep == '/' || *pep == ':' || *pep == *path )  // 如果没有type
            return NULL;
    }
}

const char * get_real_path(const char * path)
{
    /* Example path: "S:/folder/file.txt"
     * Leave the letter and the : / \ characters*/
    if ( path == NULL ) 
        return NULL;
    /*Ignore the driver letter*/
    while ( *path != ':' && *path != '\0' )
        path++;

    while(*path != '\0') {
        if(*path == ':' || *path == '\\' || *path == '/')
            path++;
        else 
            break;
    }
    
    return path;
}


const char * get_file_name(const char * path)
{
    if ( path == NULL ) 
        return NULL;
    
    uint16_t len = STRLEN(path);
    const char *pep = path+len-1;   // path end pointer

    if ( *pep == '/' || *pep == ':' )
        return NULL;
    
    while ( 1 ) {
        if ( *pep == '/' || *pep == ':' ) {
            return ++pep;
        } else {
            pep--;
        } 
    }
 
}




