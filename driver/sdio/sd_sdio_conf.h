#ifndef SD_SDIO_CONF_H
#define SD_SDIO_CONF_H

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "sd_sdio.h"
/* definations */


/* functions */
SD_Error sd_sdio_init(void);

void SD_MultiBlockTest(void);
    
#endif

