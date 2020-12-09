/************************************************
 * @file sdio.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-10
 *
 * @copyright Copyright (c) 2020
 *
*************************************************/

  /* Includes ------------------------------------------------------------------*/
#include "sdio.h"


static HAL_StatusTypeDef sd_sdio_read(uint8_t* buff, LBA_t sector, UINT count);
static HAL_StatusTypeDef sd_sdio_write(uint8_t* buff, LBA_t sector, UINT count);

fatfs_dev_opr_t dev_sd = {
  .init = sd_sdio_init,
  .read = sd_sdio_read,
  .write = sd_sdio_write,
};

SD_HandleTypeDef  hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

HAL_StatusTypeDef MX_SDIO_SD_Init(void)
{

  GPIO_InitTypeDef pin;

  pin.Pin = SD_DETECT_PIN;
  pin.Mode = GPIO_MODE_INPUT;
  pin.Pull = GPIO_PULLUP;
  pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &pin);

  if (sd_detect() != SD_PRESENT) {
    DEBUG_PRINT("No SD Card.\n");
    return HAL_ERROR;
  }

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd) != HAL_OK) {
    DEBUG_PRINT("SD Card init error.\n");
    return HAL_ERROR;
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK) {
    DEBUG_PRINT("SD Card into HighSpeed failed.\n");
    return HAL_ERROR;
  }
  return HAL_OK;
}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (sdHandle->Instance == SDIO) {
      /* USER CODE BEGIN SDIO_MspInit 0 */

      /* USER CODE END SDIO_MspInit 0 */
        /* SDIO clock enable */
    __HAL_RCC_SDIO_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    /* DMA2_Stream6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
      | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* SDIO DMA Init */
    /* SDIO_RX Init */
    hdma_sdio_rx.Instance = DMA2_Stream3;
    hdma_sdio_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_rx) != HAL_OK) {
      DEBUG_PRINT("hdma_sdio_rx error.\n");
      return;
    }

    __HAL_LINKDMA(sdHandle, hdmarx, hdma_sdio_rx);

    /* SDIO_TX Init */
    hdma_sdio_tx.Instance = DMA2_Stream6;
    hdma_sdio_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_tx) != HAL_OK) {
      DEBUG_PRINT("hdma_sdio_tx error.\n");
      return;
    }

    __HAL_LINKDMA(sdHandle, hdmatx, hdma_sdio_tx);

    /* SDIO interrupt Init */
    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
    /* USER CODE BEGIN SDIO_MspInit 1 */

    /* USER CODE END SDIO_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if (sdHandle->Instance == SDIO) {
      /* USER CODE BEGIN SDIO_MspDeInit 0 */

      /* USER CODE END SDIO_MspDeInit 0 */
        /* Peripheral clock disable */
    __HAL_RCC_SDIO_CLK_DISABLE();

    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
      | GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(sdHandle->hdmarx);
    HAL_DMA_DeInit(sdHandle->hdmatx);

    /* SDIO interrupt Deinit */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
    /* USER CODE BEGIN SDIO_MspDeInit 1 */

    /* USER CODE END SDIO_MspDeInit 1 */
  }
}

/************************************************
 * @brief User functions
*************************************************/
static uint8_t conf_flag = 0;
HAL_StatusTypeDef sd_sdio_init(void)
{
  HAL_StatusTypeDef status = HAL_ERROR;

  if (MX_SDIO_SD_Init() != HAL_OK)
    return HAL_ERROR;

  status = HAL_SD_InitCard(&hsd);
  if (status != HAL_OK) {
    DEBUG_PRINT("SD Card init occured an error.\n");
    return HAL_ERROR;
  }

  DEBUG_PRINT("SD Card init successfully.\n");

  return status;

}

void sd_sdio_deinit(void)
{
  if (conf_flag) {
    HAL_SD_MspDeInit(&hsd);
    conf_flag = 0;
  }
}

void sd_show_card_info(void)
{
  HAL_SD_CardInfoTypeDef info;
  HAL_SD_CardCIDTypeDef cid;

  HAL_SD_GetCardInfo(&hsd, &info);
  HAL_SD_GetCardCID(&hsd, &cid);


  DEBUG_PRINT("\nSD Card Info:\n");

  switch (info.CardType) {
  case CARD_SDSC:
    DEBUG_PRINT("Type \t\t: Standard Capacity\n");
    break;
  case CARD_SDHC_SDXC:
    DEBUG_PRINT("Type \t\t: High Capacity or Extended Capacity\n");
    break;
  case CARD_SECURED:
    DEBUG_PRINT("Type \t\t: Secured\n");
    break;
  default:
    break;
  }

  double cap = ((double)info.BlockNbr / (1 << 30)) * info.BlockSize;
  printf("Capacity \t: %0.2f GiB\n", cap);


  printf("OEM_AppliID \t: %c%c\n",
    cid.OEM_AppliID >> 8,
    cid.OEM_AppliID
  );
  printf("ProdName  \t: %c%c%c%c%c\n",
    cid.ProdName1 >> 24,
    cid.ProdName1 >> 16,
    cid.ProdName1 >> 8,
    cid.ProdName1,
    cid.ProdName2
  );
  printf("ManufactDate \t: %d.%d\n",
    2000 + (cid.ManufactDate >> 4),
    (cid.ManufactDate >> 4) & 0x0f
  );

}


uint8_t sd_detect(void)
{
  if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET)
    return SD_NOT_PRESENT;
  else
    return SD_PRESENT;
}

static HAL_StatusTypeDef sd_sdio_read(uint8_t* buff, LBA_t sector, UINT count)
{
  uint16_t retry = 0xFFFF;
  HAL_StatusTypeDef sd_state;

  do {
    sd_state = HAL_SD_ReadBlocks_IT(&hsd, buff, sector, count);
  } while (--retry && sd_state != HAL_OK);
  if (sd_state != HAL_OK) {
    DEBUG_PRINT("SD read error (%d)\n", sd_state);
    return sd_state;
  } else {
    while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
  }

  return sd_state;
}

static HAL_StatusTypeDef sd_sdio_write(uint8_t* buff, LBA_t sector, UINT count)
{
  uint16_t retry = 0xFFFF;
  HAL_StatusTypeDef sd_state = HAL_ERROR;

  do {
    sd_state = HAL_SD_WriteBlocks_IT(&hsd, (uint8_t*)buff, sector, count);
  } while (--retry && sd_state != HAL_OK);
  if (sd_state != HAL_OK) {
    DEBUG_PRINT("SD write error (%d)\n", sd_state);
    return sd_state;
  } else {
    while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
  }

  return sd_state;
}
