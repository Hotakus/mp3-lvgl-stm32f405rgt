#ifndef VS10XX_H
#define VS10XX_H

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>
#include "systick.h"
#include <stdlib.h>
#include "spi_conf.h"

/* definition */
#define VS10xx_USE_SPI      2
#if VS10xx_USE_SPI == 1
#define VS10xx_SPI        SPI1
#define VS10xx_CS         GPIO_Pin_4    // PA
#define VS10xx_SCLK       GPIO_Pin_5    // PA
#define VS10xx_MISO       GPIO_Pin_6    // PA
#define VS10xx_MOSI       GPIO_Pin_7    // PA
#elif VS10xx_USE_SPI == 2
#define VS10xx_SPI        SPI2
#define VS10xx_CS         GPIO_Pin_12       // PB
#define VS10xx_SCLK       GPIO_Pin_13       // PB
#define VS10xx_MISO       GPIO_Pin_14       // PB
#define VS10xx_MOSI       GPIO_Pin_15       // PB
#endif

#define VS10xx_DREQ       GPIO_Pin_10       // PB
#define VS10xx_XDCS       GPIO_Pin_8        // PA

/* return DREQ's status */
#define DREQ_STAT            ( GPIOB->IDR & VS10xx_DREQ )


#define VS10xx_XDCS_HIGH       GPIOA->BSRRL |= VS10xx_XDCS
#define VS10xx_XDCS_LOW        GPIOA->BSRRH |= VS10xx_XDCS
#if VS10xx_USE_SPI == 1
#define VS10xx_CS_HIGH       GPIOA->BSRRL |= VS10xx_CS
#define VS10xx_CS_LOW        GPIOA->BSRRH |= VS10xx_CS
#elif VS10xx_USE_SPI == 2
#define VS10xx_CS_HIGH       GPIOB->BSRRL |= VS10xx_CS
#define VS10xx_CS_LOW        GPIOB->BSRRH |= VS10xx_CS
#endif

/* SCI optional commands */
#define CMD_VS10xx_WRIT      0x02
#define CMD_VS10xx_READ      0x03

#define DUMMY                0xff

/* Chip's version */
#define VS1001               0
#define VS1011               1
#define VS1002               2
#define VS1003               3

/* CLOCK's definitions */
#define SC_MULT0     0x0000         // XTALI * 0
#define SC_MULT1     0x2000         // XTALI * 1.5
#define SC_MULT2     0x4000         // XTALI * 2.0
#define SC_MULT3     0x6000         // XTALI * 2.5
#define SC_MULT4     0x8000         // XTALI * 3.0
#define SC_MULT5     0xa000         // XTALI * 3.5
#define SC_MULT6     0xc000         // XTALI * 4
#define SC_MULT7     0xe000         // XTALI * 4.5

#define SC_ADD0      0x0000
#define SC_ADD1      0x0800
#define SC_ADD2      0x1000
#define SC_ADD3      0x1800

/* vol definitions */
#define VOL_LEFT_MUL( volLevel )            ( volLevel<<8 )
#define VOL_SETTING( left, right )          vs10xx_writ_reg( REG_VS10xx_SCI_VOL, ( VOL_LEFT_MUL((254-left)) + (254-right) ))

/* REG definition */
enum VS10xx_REG {
    REG_VS10xx_SCI_MODE        = 0x00,
    REG_VS10xx_SCI_STATUS            ,
    REG_VS10xx_SCI_BASS              ,
    REG_VS10xx_SCI_CLOCKF            ,
    REG_VS10xx_SCI_DECODETIME        ,
    REG_VS10xx_SCI_AUDATA            ,
    REG_VS10xx_SCI_WRAME             ,
    REG_VS10xx_SCI_WRAMADDR          ,
    REG_VS10xx_SCI_HDAT0             ,
    REG_VS10xx_SCI_HDAT1             ,
    REG_VS10xx_SCI_AIADDR            ,
    REG_VS10xx_SCI_VOL               ,
    REG_VS10xx_SCI_AICTRL0           ,
    REG_VS10xx_SCI_AICTRL1           ,
    REG_VS10xx_SCI_AICTRL2           ,
    REG_VS10xx_SCI_AICTRL3           ,
};

/* functions */
void vs10xx_init( void );
void vs10xx_config( void );
void vs10xx_reset( void );

u8 vs10xx_trans_byte( u8 byte );
u16 vs10xx_read_reg( u8 reg );
void vs10xx_writ_reg( u8 reg, u16 dat );
void vs10xx_sin_test( u8 test_time );
void vs10xx_send_audio_dat( u8* dat, u16 len );

#endif
