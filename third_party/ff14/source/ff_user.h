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

/* functions */
FRESULT exf_getfree ( TCHAR *path, u32 *total, u32 *free );
FRESULT scan_catalog( TCHAR *path, SCAN_OPT opt );
FRESULT show_element_info( TCHAR *path );

const char * get_real_path(const char * path);
void fatfs_test( char *dev );

#ifdef __cplusplus
}
#endif
		
#endif

