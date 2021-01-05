/************************************************
 * @file w25qxx.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-19
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "w25qxx.h"
#include "pro_conf.h"
#include "spi_conf.h"
#include "ff_devices.h"

 /************************************************
  * @brief FUNCTION PROPOTYPE
  ************************************************/
static HAL_StatusTypeDef w25qxx_wait_busy(uint32_t timeout);
static void w25qxx_gpio(void);
static HAL_StatusTypeDef w25qxx_writ_enable(uint32_t timeout);
static HAL_StatusTypeDef w25qxx_writ_disable(uint32_t timeout);

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
static uint8_t conf_flag = 0;


/************************************************
 * @brief GLOBAL VARIABLE
 ************************************************/

w25qxx_feature_s w25qxx = {
    .page_cnt = 0,
    .sect_cnt = 0,
    .capacity = 0,
    .JEDECID = 0,
};

fatfs_dev_opr_t dev_spif = {
    .init = w25qxx_init,
    .read = w25qxx_read_sector,
    .write = w25qxx_writ_sector,
};

/************************************************
 * @brief FUNCTION REALIZED
 ************************************************/

static void w25qxx_gpio(void)
{
    GPIO_InitTypeDef w25qxx_g = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    w25qxx_g.Pin = W25QXX_CS;
    w25qxx_g.Mode = GPIO_MODE_OUTPUT_PP;
    w25qxx_g.Pull = GPIO_PULLUP;
    w25qxx_g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(W25QXX_CS_PORT, &w25qxx_g);

    h_spi1.Instance = W25QXX_SPI;
}

HAL_StatusTypeDef w25qxx_init(void)
{
    if (!conf_flag) {
        w25qxx_gpio();
        conf_flag = 1;
    }
    w25qxx_powerUp();
    return HAL_OK;
}

/************************************************
 * @brief 传输一个字节
 *
 * @param byte
 * @return uint8_t
 ************************************************/
static uint8_t w25qxx_trans_byte(uint8_t byte)
{
    return spi_full_duplex_trans_byte(&h_spi1, byte);
}

/************************************************
 * @brief 写使能
 *
 * @return HAL_StatusTypeDef
 ************************************************/
static HAL_StatusTypeDef w25qxx_writ_enable(uint32_t timeout)
{
    HAL_StatusTypeDef stat;

    stat = w25qxx_wait_busy(timeout);
    if (stat != HAL_OK)
        return stat;

    W25QXX_CS_CLR;
    w25qxx_trans_byte(W25QXX_CMD_WRIT_ENABLE);
    W25QXX_CS_SET;

    return HAL_OK;
}

/************************************************
 * @brief 写失能
 *
 * @return HAL_StatusTypeDef
 ************************************************/
static HAL_StatusTypeDef w25qxx_writ_disable(uint32_t timeout)
{
    HAL_StatusTypeDef stat;

    stat = w25qxx_wait_busy(timeout);
    if (stat != HAL_OK)
        return stat;

    W25QXX_CS_CLR;
    w25qxx_trans_byte(W25QXX_CMD_WRIT_DISABLE);
    W25QXX_CS_SET;

    return HAL_OK;
}

/************************************************
 * @brief 等忙
 *
 * @param timeout
 * @return HAL_StatusTypeDef
 ************************************************/
static HAL_StatusTypeDef w25qxx_wait_busy(uint32_t timeout)
{
    uint8_t status = 0;

    do {
        W25QXX_CS_CLR;
        w25qxx_trans_byte(W25QXX_CMD_READ_STATUS1);
        status = w25qxx_trans_byte(SPI_DUMMY_BYTE);
        W25QXX_CS_SET;
    } while (--timeout && (status & 0x01));
    if (!timeout)
        return HAL_TIMEOUT;
    return HAL_OK;
}

/************************************************
 * @brief 上电
 ************************************************/
void w25qxx_powerUp(void)
{
    uint8_t status = 0;
    uint16_t retry = 0xFFFF;

    do {
        W25QXX_CS_CLR;
        w25qxx_trans_byte(W25QXX_CMD_POWERUP);
        for (uint8_t i = 0; i < 3; i++) {
            w25qxx_trans_byte(0xFF);
        }
        status = w25qxx_trans_byte(SPI_DUMMY_BYTE);
        W25QXX_CS_SET;
    } while (--retry && (status == SPI_DUMMY_BYTE || status == 0));
    if (!retry) {
        DEBUG_PRINT("W25QXX init failed.\n");
        return;
    }
    DEBUG_PRINT("W25QXX init Ok.\n");
    w25qxx_get_JEDECID();
}

/************************************************
 * @brief 掉电
 ************************************************/
void w25qxx_powerDown(void)
{
}

/************************************************
 * @brief 读JEDECID
 ************************************************/
void w25qxx_get_JEDECID(void)
{
    if (w25qxx.JEDECID)
        return;

    W25QXX_CS_CLR;
    w25qxx_trans_byte(W25QXX_CMD_JEDECID);
    for (uint8_t i = 0; i < 3; i++) {
        w25qxx.JEDECID <<= 8;
        w25qxx.JEDECID |= w25qxx_trans_byte(SPI_DUMMY_BYTE);
    }
    W25QXX_CS_SET;

    if (w25qxx.JEDECID >> 16 != 0xEF) {
        w25qxx.JEDECID = 0;
        return;
    }

    switch (w25qxx.JEDECID) {
    case W25Q256:
        w25qxx.capacity = 32 << 20;  // 32MiB
        break;
    case W25Q128:
        w25qxx.capacity = 16 << 20;  // 16MiB
        break;
    case W25Q64:
        w25qxx.capacity = 8 << 20;  // 8MiB
        break;
    case W25Q32:
        w25qxx.capacity = 4 << 20;  // 4MiB
        break;
    case W25Q16:
        w25qxx.capacity = 2 << 20;  // 2MiB
        break;
    default:
        DEBUG_PRINT("W25QXX unknown.\n");
        break;
    }
    w25qxx.page_cnt = w25qxx.capacity >> 8;
    w25qxx.sect_cnt = w25qxx.capacity >> 12;
}

/************************************************
 * @brief 擦除指定范围扇区
 *
 * @param ssect
 * @param sect_num
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef w25qxx_erase_sector(uint32_t ssect, uint32_t sect_num)
{
    HAL_StatusTypeDef stat;

    uint32_t addr;

    if (!sect_num)
        return HAL_OK;

    while (sect_num--) {
        stat = w25qxx_writ_enable(0xFFFFFFFF);
        if (stat != HAL_OK)
            return stat;

        W25QXX_CS_CLR;
        w25qxx_trans_byte(W25QXX_CMD_ERASE_SECT);
        addr = ssect << 12;
        w25qxx_trans_byte((addr >> 16) & 0xFF);
        w25qxx_trans_byte((addr >> 8) & 0xFF);
        w25qxx_trans_byte((addr >> 0) & 0xFF);
        W25QXX_CS_SET;

        stat = w25qxx_writ_disable(0xFFFFFFFF);
        if (stat != HAL_OK)
            return stat;
        ssect++;
        DEBUG_PRINT(".");
    }

    return HAL_OK;
}

/************************************************
 * @brief 擦除全片
 ************************************************/
HAL_StatusTypeDef w25qxx_erase_chip(void)
{
    HAL_StatusTypeDef stat;

    DEBUG_PRINT("please wait a moment.\n");
    stat = w25qxx_writ_enable(0xFFFF);  // 写使能
    if (stat != HAL_OK)
        return stat;

    W25QXX_CS_CLR;
    w25qxx_trans_byte(W25QXX_CMD_ERASE_CHIP);
    W25QXX_CS_SET;

    stat = w25qxx_writ_disable(0xFFFFFFFF);  // 写失能
    if (stat != HAL_OK)
        return stat;
    DEBUG_PRINT("W25QXX chip erased.\n");
    return HAL_OK;
}

/************************************************
 * @brief 页读
 *
 * @param rec_buf
 * @param page
 * @param cnt
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef w25qxx_read_page(uint8_t* rec_buf, uint32_t page, uint32_t cnt)
{
    uint32_t addr = 0;

    if (!cnt)
        return HAL_OK;

    while (cnt--) {
        W25QXX_CS_CLR;
        w25qxx_trans_byte(W25QXX_CMD_FAST_READ);
        addr = page << 8;
        w25qxx_trans_byte((addr >> 16) & 0xFF);
        w25qxx_trans_byte((addr >> 8) & 0xFF);
        w25qxx_trans_byte((addr >> 0) & 0xFF);
        w25qxx_trans_byte(0xFF);  // dummy byte
        for (uint16_t i = 0; i < 256; i++)
            *rec_buf++ = w25qxx_trans_byte(SPI_DUMMY_BYTE);
        W25QXX_CS_SET;
        page += 1;
    }
    return HAL_OK;
}

/************************************************
 * @brief 页写
 *
 * @param send_buf
 * @param page
 * @param cnt
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef w25qxx_writ_page(uint8_t* send_buf, uint32_t page, uint32_t cnt)
{
    HAL_StatusTypeDef stat;
    uint32_t addr = 0;

    if (!cnt)
        return HAL_OK;

    while (cnt--) {
        stat = w25qxx_writ_enable(0xFFFF);  // 写使能
        if (stat != HAL_OK)
            return stat;

        W25QXX_CS_CLR;
        // 发送页写命令
        w25qxx_trans_byte(W25QXX_CMD_PAGE_PROG);
        addr = page << 8;
        /* 发送24位地址 */
        w25qxx_trans_byte((addr >> 16) & 0xFF);
        w25qxx_trans_byte((addr >> 8) & 0xFF);
        w25qxx_trans_byte((addr >> 0) & 0xFF);
        for (uint16_t i = 0; i < 256; i++)
            w25qxx_trans_byte(*send_buf++);
        W25QXX_CS_SET;

        stat = w25qxx_writ_disable(0xFFFF);
        if (stat != HAL_OK)
            return stat;
        page += 1;
    }
    return HAL_OK;
}

/************************************************
 * @brief 读扇区
 *
 * @param rec_buf
 * @param sector
 * @param cnt
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef w25qxx_read_sector(
    uint8_t* rec_buf,
    uint32_t sector,
    uint32_t cnt
)
{
    return w25qxx_read_page(rec_buf, (sector << 4), (cnt << 4));
}

/************************************************
 * @brief 写扇区
 *
 * @param send_buf
 * @param sector
 * @param cnt
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef w25qxx_writ_sector(
    uint8_t* send_buf,
    uint32_t sector,
    uint32_t cnt
)
{
    if (w25qxx_erase_sector(sector, cnt) != HAL_OK)
        return HAL_OK;
    return w25qxx_writ_page(send_buf, (sector << 4), (cnt << 4));
}
