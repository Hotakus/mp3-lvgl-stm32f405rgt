#ifndef __FF_USER_H
#define __FF_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/* file system */
#include "ff.h"

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>
#include "systick.h"
#include "usart.h"

/* definations */
typedef enum {
    SCAN_OPT_ALL        = 0x00,     // 遍历所有
    SCAN_OPT_NO_HIDEN         ,     // 遍历除隐藏文件夹外的所有
    SCAN_OPT_CUR_DIR          ,     // 仅遍历当前文件夹
} SCAN_OPT;

/************************************************
 * @brief Path type
 ************************************************/
typedef enum {
    PATH_DEV_NAME = 0,    // 设备名
    PATH_MAIN        ,    // 主路径
    PATH_FILE_NAME   ,    // 文件名
    PATH_FILE_TYPE   ,    // 文件类型
} PATH_TYPE;

/* functions */
FRESULT exf_getfree ( TCHAR *path, uint32_t *total, uint32_t *free );
FRESULT scan_catalog( TCHAR *path, SCAN_OPT opt );
FRESULT show_file_info( TCHAR *path );

const char *path_get( const char *path, PATH_TYPE pt );
const char * get_dev_name(const char * path);
const char * get_real_path(const char * path);
const char * get_file_name(const char * path);
const char * get_file_type(const char * path);
void fatfs_test( char *dev );

#ifdef __cplusplus
}
#endif
		
#endif

