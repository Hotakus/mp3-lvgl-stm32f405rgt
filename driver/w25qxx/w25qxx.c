#include "w25qxx.h"
#include "spi_conf.h"
#include "dma_conf.h"
#include "pro_conf.h"

#define W25QXX_USE_DMA     0

static u8 w25qxx_trans_byte( u8 byte );
static u8 w25qxx_writ_enable( void );
static u8 w25qxx_writ_disable( void );

w25qxx_feature w25qxx = {
    .pageSize   = 256,
    .pageCnt    = 0x00,      // pages
    .sect_size  = 4096,      // bytes
    .sect_cnt   = 0x00,      // sectors
    .capacity   = 0x00,      // bytes
    .JEDECID    = 0x00, 
};

void w25qxx_gpio( void ) {
    
    SPI_InitTypeDef  spi_s;
    GPIO_InitTypeDef spix_gpio;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
    
    GPIO_InitTypeDef w25Io;
    W25QXX_CS_SET;
    w25Io.GPIO_Pin               = W25QXX_SPI_CS;
    w25Io.GPIO_Mode              = GPIO_Mode_OUT;
    w25Io.GPIO_OType             = GPIO_OType_PP;
    w25Io.GPIO_PuPd              = GPIO_PuPd_UP;
    w25Io.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &w25Io );
    
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource3, GPIO_AF_SPI1 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_SPI1 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_SPI1 );
    
    /* SPI conf */
    spi_s.SPI_CPOL      = SPI_CPOL_High;
    spi_s.SPI_CPHA      = SPI_CPHA_2Edge;

    spi_s.SPI_Mode              = SPI_Mode_Master;
    spi_s.SPI_FirstBit          = SPI_FirstBit_MSB;
    spi_s.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    spi_s.SPI_DataSize          = SPI_DataSize_8b;
    spi_s.SPI_NSS               = SPI_NSS_Soft;
    spi_s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
    SPI_Init( W25QXX_SPI, &spi_s );
    SPI_Cmd( W25QXX_SPI, ENABLE );
    
    spix_gpio.GPIO_Pin          = W25QXX_SPI_SCLK | W25QXX_SPI_MOSI | W25QXX_SPI_MISO;
    spix_gpio.GPIO_Mode         = GPIO_Mode_AF;
    spix_gpio.GPIO_OType        = GPIO_OType_PP;
    spix_gpio.GPIO_PuPd         = GPIO_PuPd_UP;
    spix_gpio.GPIO_Speed        = GPIO_Speed_100MHz;
    GPIO_Init( GPIOB, &spix_gpio );
 
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
static u8 w25qxx_trans_byte( u8 byte ) {
    while ( !( W25QXX_SPI->SR & SPI_I2S_FLAG_TXE ) );
    W25QXX_SPI->DR = byte;
    while ( !( W25QXX_SPI->SR & SPI_I2S_FLAG_RXNE ) );
    return (u8)W25QXX_SPI->DR;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
static u8 w25qxx_writ_enable( void ) {
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    W25QXX_CS_CLR;
    w25qxx_trans_byte( W25QXX_CMD_WRIT_ENABLE );
    W25QXX_CS_SET;
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    return W25QXX_OPT_OK;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
static u8 w25qxx_writ_disable( void ) {
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    W25QXX_CS_CLR;
    w25qxx_trans_byte( W25QXX_CMD_WRIT_DISABLE );
    W25QXX_CS_SET;
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    return W25QXX_OPT_OK;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

void w25qxx_init( void ) {
    w25qxx_gpio();
    w25qxx_powerUp();
    w25qxx_JEDECID();
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
void w25qxx_powerUp( void ) {
    u8 i;
    u8 status = 0;
    u16 retry = 0xFFFF;
    
    do {
        W25QXX_CS_CLR;
        w25qxx_trans_byte( W25QXX_CMD_POWERUP );
        for ( i = 0; i < 3; i++ ) {
            w25qxx_trans_byte( W25QXX_DUMMY_BYTE );
        }
        status = w25qxx_trans_byte( W25QXX_DUMMY_BYTE );
        W25QXX_CS_SET;
        //DEBUG_PRINT( "status (%x)\n", status );
    } while ( --retry && (status == W25QXX_DUMMY_BYTE) );
    if ( !retry ) {
        DEBUG_PRINT( "W25QXX init failed.\n" );
    } else {
        DEBUG_PRINT( "W25QXX init ok. (%x)\n", status );
    }
    
}

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
void w25qxx_JEDECID( void ) {
    u8 i;
    
    if ( w25qxx.JEDECID != 0 )
        return;

    W25QXX_CS_CLR;
    w25qxx_trans_byte( W25QXX_CMD_JEDECID );
    for ( i = 0; i < 3; i++ ) {
        w25qxx.JEDECID <<= 8;
        w25qxx.JEDECID |= w25qxx_trans_byte( W25QXX_DUMMY_BYTE );
    }
    if ( w25qxx.JEDECID>>16 == 0xEF ) {
        DEBUG_PRINT( "W25QXX JEDECID: %x\n", w25qxx.JEDECID );
        w25qxx.mType = w25qxx.JEDECID&0xFFFF;
        switch ( w25qxx.mType ) {
            case W25Q256:
                w25qxx.pageCnt   = 131072;
                break;
            case W25Q128:
                w25qxx.pageCnt   = 131072>>1;
                break;
            case W25Q64:
                w25qxx.pageCnt   = 131072>>2;
                break;
            case W25Q32:
                w25qxx.pageCnt   = 131072>>3;
                break;
            case W25Q16:
                w25qxx.pageCnt   = 131072>>4;
                break;
            default:
                DEBUG_PRINT( "W25QXX unknown.\n" );
                break;
        }
        w25qxx.sect_cnt  = w25qxx.pageCnt>>4;
        w25qxx.capacity  = w25qxx.pageCnt*w25qxx.pageSize;
        DEBUG_PRINT( "W25QXX capacity: %d MiB\n", w25qxx.capacity>>20 );
    } else {
        DEBUG_PRINT( "W25QXX JEDECID error. %x\n", w25qxx.JEDECID );
    }
    
    
    W25QXX_CS_SET;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_wait_busy( void ) {
    u32 timeWait = 0xFFFF;
    u8 status = 0;
    W25QXX_CS_CLR;
    do {
        w25qxx_trans_byte( W25QXX_CMD_READ_STATUS1 );
        status = w25qxx_trans_byte( W25QXX_DUMMY_BYTE );
    } while ( --timeWait && (status&0x01) );
    W25QXX_CS_SET;
    if ( !timeWait ) return W25QXX_TIMEOUT;
    return W25QXX_OPT_OK;;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_erase_sector ( u32 ssect, u32 sectLen ) {
    u32 addr;

    while ( sectLen-- ) {
        addr = ssect<<12;
        w25qxx_writ_enable();        // 写使能
        W25QXX_CS_CLR;
        w25qxx_trans_byte( W25QXX_CMD_ERASE_SECT );
        w25qxx_trans_byte( (addr>>16)&0xFF );
        w25qxx_trans_byte( (addr>>8 )&0xFF );
        w25qxx_trans_byte( (addr>>0 )&0xFF );
        W25QXX_CS_SET;
        w25qxx_writ_disable();        // 写失能
        DEBUG_PRINT( "erase: %d\n", ssect );
        ssect++;
    }
    return W25QXX_OPT_OK;
}

u8 w25qxx_erase_chip( void ) {
    DEBUG_PRINT( "please wait a moment.\n" );
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    w25qxx_writ_enable();        // 写使能
    W25QXX_CS_CLR;
    w25qxx_trans_byte( W25QXX_CMD_ERASE_CHIP );
    W25QXX_CS_SET;
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ) return W25QXX_TIMEOUT;
    w25qxx_writ_disable();        // 写失能
    DEBUG_PRINT( "W25QXX chip erased.\n" );
    return W25QXX_OPT_OK;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_read_page( u8* pRecBuf , u32 page, u32 cnt ) {
    u32 addr = page<<8;
    u32 i;
    
    if ( cnt == 0 ) return W25QXX_OPT_FAILED;
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ){
        DEBUG_PRINT( "read page timeout.\n" );
        return W25QXX_TIMEOUT;
    }

    W25QXX_CS_CLR;
    
    w25qxx_trans_byte( W25QXX_CMD_FAST_READ );
    w25qxx_trans_byte( addr>>16 );
    w25qxx_trans_byte( addr>>8 );
    w25qxx_trans_byte( addr&0xFF );
    w25qxx_trans_byte( 0xFF );

    while ( cnt-- ) {
#if W25QXX_USE_DMA == 0
        for ( i = 0; i < 256; i++ ) {
            *pRecBuf++ = w25qxx_trans_byte( 0xFF );
        }
#elif W25QXX_USE_DMA == 1
        spi1_dma_trans( t_buf, pRecBuf, w25qxx.pageSize );
        pRecBuf += w25qxx.pageSize;
#endif       
    }
    
    W25QXX_CS_SET;
    return W25QXX_OPT_OK;
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_writ_page( u8* pSendBuf, u32 page, u32 cnt ) {
    u32 addr;
    u16 i;
    
    if ( cnt == 0 ) return W25QXX_OPT_FAILED;
    if ( w25qxx_wait_busy() == W25QXX_TIMEOUT ){
        DEBUG_PRINT( "write page timeout.\n" );
        return W25QXX_TIMEOUT;
    }
    /* 循环写入 */
    while ( cnt-- ) {
        w25qxx_writ_enable();        // 写使能
        W25QXX_CS_CLR;
        addr = page<<8;
        w25qxx_trans_byte( W25QXX_CMD_PAGE_PROG );
        w25qxx_trans_byte( addr>>16 );
        w25qxx_trans_byte( addr>>8 );
        w25qxx_trans_byte( addr&0xFF );
#if W25QXX_USE_DMA == 0
        for ( i = 0; i < 256; i++ ) {
            w25qxx_trans_byte( *pSendBuf++ );
        }
#elif W25QXX_USE_DMA == 1
        spi1_dma_trans( pSendBuf, t_buf, w25qxx.pageSize );
        pSendBuf += w25qxx.pageSize;
#endif 
        W25QXX_CS_SET;
        w25qxx_writ_disable();        // 写失能
        page++;
    }
    
    
    return W25QXX_OPT_OK;
}

/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_read_sector( u8* pBuf, u32 sector, u32 cnt ) {
    return w25qxx_read_page( pBuf, sector<<4, cnt<<4 );
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
u8 w25qxx_writ_sector( u8* pBuf, u32 sector, u32 cnt ) {
    w25qxx_erase_sector( sector, cnt );
    return w25qxx_writ_page( pBuf, sector<<4, cnt<<4 );
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

