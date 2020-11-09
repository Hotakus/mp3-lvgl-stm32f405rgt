#ifndef SD_TO_SPIF_H
#define SD_TO_SPIF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"

/* definations */
typedef enum SD2SPIF_CHECK{
    SD2SPIF_CHECK_NONE_MEM_DEV = 0x00,
    SD2SPIF_CHECK_EXIST_SPIF   = 0x01,
    SD2SPIF_CHECK_EXIST_SD     = 0x02,
    SD2SPIF_CHECK_EXIST_ALL    = 0x03,
}SD2SPIF_CHECK;

typedef enum TRANS_STAT{
    
    SD2SPIF_TRANS_CHECK_ERR = 0,
    
    SD2SPIF_TRANS_STAT_OK,
    SD2SPIF_TRANS_STAT_ERR,
    
    SD2SPIF_SD_PATH_ERR,
    SD2SPIF_SPIF_PATH_ERR,
    
}TRANS_STAT;

/* function */

TRANS_STAT sd2spif( const char *sd_path, const char *spif_path );

#ifdef __cplusplus
}
#endif
#endif
