/************************************************
 * @file spi_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "spi_conf.h"
#include "pro_conf.h"

GPIO_InitTypeDef spix_gpio;

/************************************************
 * @brief 根据用户输入信息配置SPI
 *
 * @param h_spix->Instance 哪个spi
 * @param speed spi速度
 * SPI_MODE_1: CPOL=0, CPHA=0
 * SPI_MODE_2: CPOL=0, CPHA=1
 * SPI_MODE_3: CPOL=1, CPHA=0
 * SPI_MODE_4: CPOL=1, CPHA=1
 * @param mode spi的四种模式
 * @param spi_dir spi方向
 ************************************************/
void spi_conf(
    SPI_HandleTypeDef* h_spix,
    uint32_t speed,
    SPI_MODE mode,
    uint32_t spi_dir
)
{
    uint32_t pin = 0;

    if (h_spix->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        pin = SPI1_SCLK;
        if (spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI1_MISO;
        if (spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI1_MOSI;
    } else if (h_spix->Instance == SPI2) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_SPI2_CLK_ENABLE();
        pin = SPI2_SCLK;
        if (spi_dir == SPI_DIRECTION_2LINES_RXONLY ||
            spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI2_MISO;
        if (spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI2_MOSI;
    } else if (h_spix->Instance == SPI3) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_SPI3_CLK_ENABLE();
        pin = SPI3_SCLK;
        if (spi_dir == SPI_DIRECTION_2LINES_RXONLY ||
            spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI3_MISO;
        if (spi_dir == SPI_DIRECTION_1LINE || spi_dir == SPI_DIRECTION_2LINES)
            pin |= SPI3_MOSI;
    }

    /* SPI conf */
    switch (mode) {
    case SPI_MODE_1:
        h_spix->Init.CLKPhase = SPI_PHASE_1EDGE;
        h_spix->Init.CLKPolarity = SPI_POLARITY_LOW;
        break;
    case SPI_MODE_2:
        h_spix->Init.CLKPhase = SPI_PHASE_2EDGE;
        h_spix->Init.CLKPolarity = SPI_POLARITY_LOW;
        break;
    case SPI_MODE_3:
        h_spix->Init.CLKPhase = SPI_PHASE_1EDGE;
        h_spix->Init.CLKPolarity = SPI_POLARITY_HIGH;
        break;
    case SPI_MODE_4:
        h_spix->Init.CLKPhase = SPI_PHASE_2EDGE;
        h_spix->Init.CLKPolarity = SPI_POLARITY_HIGH;
        break;
    }

    spix_gpio.Pin = pin;
    spix_gpio.Mode = GPIO_MODE_AF_PP;
    spix_gpio.Pull = GPIO_PULLUP;
    spix_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    if (h_spix->Instance == SPI1) {
        spix_gpio.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &spix_gpio);
        h_spix->Instance = SPI1;
    } else if (h_spix->Instance == SPI2) {
        spix_gpio.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &spix_gpio);
        h_spix->Instance = SPI2;
    } else if (h_spix->Instance == SPI3) {
        spix_gpio.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOB, &spix_gpio);
        h_spix->Instance = SPI3;
    }

    h_spix->Init.BaudRatePrescaler = speed;
    h_spix->Init.Mode = SPI_MODE_MASTER;
    h_spix->Init.NSS = SPI_NSS_SOFT;
    h_spix->Init.FirstBit = SPI_FIRSTBIT_MSB;
    h_spix->Init.DataSize = SPI_DATASIZE_8BIT;
    h_spix->Init.Direction = spi_dir;
    h_spix->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_spix->Init.TIMode = SPI_TIMODE_DISABLE;
    if (HAL_SPI_Init(h_spix) != HAL_OK) {
        DEBUG_PRINT( "SPI Config error.\n" );
    }
    __HAL_SPI_ENABLE(h_spix);
}

uint8_t spi_full_duplex_trans_byte(SPI_HandleTypeDef* h_spix, uint8_t byte)
{
    while ((h_spix->Instance->SR & SPI_FLAG_TXE)!=SPI_FLAG_TXE);
    *((__IO uint8_t*) & h_spix->Instance->DR) = (byte);
    while ((h_spix->Instance->SR & SPI_FLAG_RXNE)!=SPI_FLAG_RXNE);
    return h_spix->Instance->DR;
}

void spi_one_line_tx_trans_byte(SPI_HandleTypeDef* h_spix, uint8_t byte)
{
    while ((h_spix->Instance->SR & SPI_FLAG_TXE)!=SPI_FLAG_TXE);
    *((__IO uint8_t*) & h_spix->Instance->DR) = (byte);
}
