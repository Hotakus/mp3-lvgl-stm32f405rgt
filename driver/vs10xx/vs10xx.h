/************************************************
 * @file vs10xx.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-24
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef VS10XX_H
#define VS10XX_H

/* includes */
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include <stdio.h>
#include <stdlib.h>
#include "spi_conf.h"
#include "mp3_decode.h"

/* definition */
#define VS10xx_SPI              SPI1
#define VS10xx_XRESET           GPIO_PIN_0        // PC
#define VS10xx_DREQ             GPIO_PIN_1        // PC
#define VS10xx_XDCS             GPIO_PIN_2        // PC
#define VS10xx_XCS              GPIO_PIN_3        // PC

/* return DREQ's status */
#define DREQ_STAT               ( ((GPIOC->IDR & VS10xx_DREQ)==VS10xx_DREQ)?(1):(0) )

#define VS10xx_XRESET_HIGH      GPIOC->BSRR = (uint32_t)VS10xx_XRESET
#define VS10xx_XRESET_LOW       GPIOC->BSRR = (uint32_t)VS10xx_XRESET <<16

#define VS10xx_XDCS_HIGH        GPIOC->BSRR = (uint32_t)VS10xx_XDCS
#define VS10xx_XDCS_LOW         GPIOC->BSRR = (uint32_t)VS10xx_XDCS <<16

#define VS10xx_CS_HIGH          GPIOC->BSRR = (uint32_t)VS10xx_XCS
#define VS10xx_CS_LOW           GPIOC->BSRR = (uint32_t)VS10xx_XCS <<16


/* SCI optional commands */
#define CMD_VS10xx_WRIT      0x02
#define CMD_VS10xx_READ      0x03

/* Chip's version */
#define VS1001               0
#define VS1011               1
#define VS1002               2
#define VS1003               3
#define VS1053               4

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

/* 音量设置 */
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
void vs10xx_hw_reset( void );
void vs10xx_sw_reset( void );

void vs10xx_sin_test( uint16_t test_time );

void vs10xx_playback( const char *audio_path );
void vs10xx_play_mp3( const char *path );


#endif
