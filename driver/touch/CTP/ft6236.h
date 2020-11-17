#ifndef FT6236_H
#define FT6236_H

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>


/* definations */
#define FT6236_RST      GPIO_Pin_0  // PA
#define FT6236_INT      GPIO_Pin_1  // PA

/* functions */
void ctp_ft6236_init( void );

#endif
