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

SPI_InitTypeDef  spi_s;
GPIO_InitTypeDef spix_gpio;

// SPI_MODE_1: CPOL=0, CPHA=0 
// SPI_MODE_2: CPOL=0, CPHA=1
// SPI_MODE_3: CPOL=1, CPHA=0
// SPI_MODE_4: CPOL=1, CPHA=1
void spi_conf( SPI_TypeDef* SPIx, u16 speed, SPI_MODE mode, u32 SPI_DIR ) {
    
    u32 pin = 0;
    
    if ( SPIx == SPI1 ) {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
        pin = SPI1_SCLK;
        GPIO_PinAFConfig( GPIOA, GPIO_PinSource5, GPIO_AF_SPI1 );
        if ( SPI_DIR == SPI_Direction_1Line_Rx || SPI_DIR == SPI_Direction_2Lines_FullDuplex ) {
            GPIO_PinAFConfig( GPIOA, GPIO_PinSource6, GPIO_AF_SPI1 );
            pin |= SPI1_MISO;
        }
        if ( SPI_DIR == SPI_Direction_1Line_Tx || SPI_DIR == SPI_Direction_2Lines_FullDuplex ) {
            GPIO_PinAFConfig( GPIOA, GPIO_PinSource7, GPIO_AF_SPI1 );
            pin |= SPI1_MOSI;
        }
    } else if ( SPIx == SPI2 ) {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource13, GPIO_AF_SPI2 );
        pin = SPI2_SCLK;
        if ( SPI_DIR == SPI_Direction_1Line_Rx || SPI_DIR == SPI_Direction_2Lines_FullDuplex ) {
            GPIO_PinAFConfig( GPIOB, GPIO_PinSource14, GPIO_AF_SPI2 );
            pin |= SPI2_MISO;
        }
        if ( SPI_DIR == SPI_Direction_1Line_Tx || SPI_DIR == SPI_Direction_Tx || SPI_DIR == SPI_Direction_2Lines_FullDuplex  ) {
            GPIO_PinAFConfig( GPIOB, GPIO_PinSource15, GPIO_AF_SPI2 );
            pin |= SPI2_MOSI;
        }
    } else if ( SPIx == SPI3 ) {
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );
        pin = SPI3_SCLK;
        GPIO_PinAFConfig( GPIOB, GPIO_PinSource3, GPIO_AF_SPI3 );
        if ( SPI_DIR == SPI_Direction_1Line_Rx || SPI_DIR == SPI_Direction_2Lines_FullDuplex ) {
            GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_SPI3 );
            pin |= SPI3_MISO;
        }
        if ( SPI_DIR == SPI_Direction_1Line_Tx || SPI_DIR == SPI_Direction_2Lines_FullDuplex ) {
            GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_SPI3 );
            pin |= SPI3_MOSI;
        }
    }
    
    /* SPI conf */
    u8 t_mode = mode;
    switch ( t_mode ) {
        case SPI_MODE_1:
            spi_s.SPI_CPOL      = SPI_CPOL_Low;
            spi_s.SPI_CPHA      = SPI_CPHA_1Edge;
            break;
        case SPI_MODE_2:
            spi_s.SPI_CPOL      = SPI_CPOL_Low;
            spi_s.SPI_CPHA      = SPI_CPHA_2Edge;
            break;
        case SPI_MODE_3:
            spi_s.SPI_CPOL      = SPI_CPOL_High;
            spi_s.SPI_CPHA      = SPI_CPHA_1Edge;
            break;
        case SPI_MODE_4:
            spi_s.SPI_CPOL      = SPI_CPOL_High;
            spi_s.SPI_CPHA      = SPI_CPHA_2Edge;
            break;
    }   
    
    spi_s.SPI_Mode              = SPI_Mode_Master;
    spi_s.SPI_FirstBit          = SPI_FirstBit_MSB;
    spi_s.SPI_Direction         = SPI_DIR;
    spi_s.SPI_DataSize          = SPI_DataSize_8b;
    spi_s.SPI_NSS               = SPI_NSS_Soft;
    spi_s.SPI_BaudRatePrescaler = speed; 
    SPI_Init( SPIx, &spi_s );
    SPI_Cmd( SPIx, ENABLE );
    
    spix_gpio.GPIO_Pin          = pin;
    spix_gpio.GPIO_Mode         = GPIO_Mode_AF;
    spix_gpio.GPIO_OType        = GPIO_OType_PP;
    spix_gpio.GPIO_PuPd         = GPIO_PuPd_UP;
    spix_gpio.GPIO_Speed        = GPIO_Speed_100MHz;
    if ( SPIx == SPI1 ) {
        GPIO_Init( GPIOA, &spix_gpio );
    } else if ( SPIx == SPI2 ) {
        GPIO_Init( GPIOB, &spix_gpio );
    } else if ( SPIx == SPI3 ) {
        GPIO_Init( GPIOB, &spix_gpio );
    }
    
}

