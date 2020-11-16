#include "vs10xx.h"
#include "usart.h"
#include "pro_conf.h"
#if USER_USE_UCOS == 1
#include "ucos_inc.h"
#endif

static u8 version = 0;
/* sin test */
static u8 sinStart[] = { 0x53, 0xEF, 0x6E, 0x6f, 0, 0, 0, 0 };
static u8 sinEnd[]   = { 0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0 };
static u8* audioDataBuffer = NULL;

#if USER_USE_UCOS == 1
OS_ERR  err;    
#endif

/* address of the peripheral */
#define SPI1_DR_ADDR        (uint32_t)&(VS10xx_SPI->DR)
#define AUDIO_BUF_LEN       256

void vs10xx_config( void ) {
    
    GPIO_InitTypeDef vsIo;
    SPI_InitTypeDef  vsSpi;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
    
    /* GPIO */
    vsIo.GPIO_Pin               = VS10xx_DREQ;       // PB
    vsIo.GPIO_Mode              = GPIO_Mode_IN;
    vsIo.GPIO_OType             = GPIO_OType_OD;
    vsIo.GPIO_PuPd              = GPIO_PuPd_NOPULL;
    vsIo.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOB, &vsIo );

    vsIo.GPIO_Pin               = VS10xx_CS;        // PB
    vsIo.GPIO_Mode              = GPIO_Mode_OUT;
    vsIo.GPIO_OType             = GPIO_OType_PP;
    vsIo.GPIO_PuPd              = GPIO_PuPd_UP;
    vsIo.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOB, &vsIo );
    
    vsIo.GPIO_Pin               = VS10xx_XDCS;       // PA
    vsIo.GPIO_Mode              = GPIO_Mode_OUT;
    vsIo.GPIO_OType             = GPIO_OType_PP;
    vsIo.GPIO_PuPd              = GPIO_PuPd_UP;
    vsIo.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &vsIo );

}

void vs10xx_init( void ) {
    
    spi_conf( VS10xx_SPI, SPI_BaudRatePrescaler_256, SPI_MODE_1 );
    vs10xx_config();
    vs10xx_reset();
    
    version = ((u8)vs10xx_read_reg( REG_VS10xx_SCI_STATUS ))>>4;
    if( version == 0x0f ) return;
    vs10xx_writ_reg( REG_VS10xx_SCI_MODE, ( 1<<11 | 1<<6 | 1<<5 ) );// stream mode and MSB first turn on
    vs10xx_writ_reg( REG_VS10xx_SCI_BASS, 0x7af6 );                 //configures BASS
    vs10xx_writ_reg( REG_VS10xx_SCI_CLOCKF, (SC_MULT4 | SC_ADD3) );
    VOL_SETTING( 160, 160 ); //initialize the vol left:-2dB, right:-2dB
    
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_MODE ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_BASS ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_CLOCKF ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_VOL ) );
    
    /* start to play music 256 bytes */
    audioDataBuffer = (u8*)malloc( sizeof(u8)*AUDIO_BUF_LEN );
    
    /* vs10xx sin test */
#define SINTEST 1
#if SINTEST == 1
    vs10xx_sin_test( 100 );
#endif

    DEBUG_PRINT( "VS10xx init ok.\n" );
}

void vs10xx_reset( void ) {
    
    
}

u8 vs10xx_trans_byte( u8 byte ) {
    while ( !( VS10xx_SPI->SR & SPI_I2S_FLAG_TXE ) );
    VS10xx_SPI->DR = byte;
    while ( !( VS10xx_SPI->SR & SPI_I2S_FLAG_RXNE ) );
    return (u8)VS10xx_SPI->DR;
}

u16 vs10xx_read_reg( u8 reg ) {

    u16 value = 0x94;
    VS10xx_XDCS_HIGH;
    VS10xx_CS_LOW;
    
    vs10xx_trans_byte( CMD_VS10xx_READ );
    vs10xx_trans_byte( reg );
    value = vs10xx_trans_byte( DUMMY ) << 8;
    value |= vs10xx_trans_byte( DUMMY );
    
    VS10xx_CS_HIGH;
    return value;
}

void vs10xx_writ_reg( u8 reg, u16 dat ) {

    VS10xx_XDCS_HIGH;
    VS10xx_CS_LOW;
    
    vs10xx_trans_byte( CMD_VS10xx_WRIT );
    vs10xx_trans_byte( reg );
    vs10xx_trans_byte( dat >> 8 );          // 8bit of high
    vs10xx_trans_byte( (u8)dat );           // 8bit of low
    
    VS10xx_CS_HIGH;
}

void vs10xx_sin_test( u8 test_time ) {
    u8 i = 0;
    for ( i = 0; i < 8; i++ ) {
        vs10xx_trans_byte( sinStart[i] );
    }
    DEBUG_PRINT( "VS10xx test begin.\n" );
    // delay( test_time );
//    for ( i = 0; i < 8; i++ ) {
//        vs10xx_trans_byte( sinEnd[i] );
//    }
    
    DEBUG_PRINT( "VS10xx test done.\n" );
}


void vs10xx_send_audio_dat( u8* dat, u16 len ) {
    
}

