/************************************************
 * @file sdio.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-08
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDIO_H
#define __SDIO_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "pro_conf.h"
#include "uart.h"

#include "ff.h"
typedef struct  {
  HAL_StatusTypeDef (*init) (void);
  HAL_StatusTypeDef (*read) ( uint8_t *buff, LBA_t sector, UINT count );
  HAL_StatusTypeDef (*write) ( uint8_t *buff, LBA_t sector, UINT count );
} fatfs_dev_opr_t;

extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern SD_HandleTypeDef hsd;

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                       ((uint8_t)0x01)
#define SD_NOT_PRESENT                   ((uint8_t)0x00)

#define SD_DETECT_PIN                    GPIO_PIN_4                 /* PH.13 */
#define SD_DETECT_GPIO_PORT              GPIOA                       /* GPIOH */

HAL_StatusTypeDef sd_sdio_init(void);
uint8_t sd_detect( void );
void sd_sdio_deinit(void);
void sd_show_card_info(void);

#ifdef __cplusplus
}
#endif
#endif /*__ sdio_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
