/************************************************
 * @file vs10xx.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-24
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "vs10xx.h"
#include "usart.h"
#include "pro_conf.h"
#include "nvic_conf.h"
#include "ff_user.h"
#include "tim.h"

/************************************************
 * @brief STATIC PROPOTYPE
 ************************************************/
static void vs10xx_config( void );
static uint8_t vs10xx_trans_byte( uint8_t byte );
static uint16_t vs10xx_read_reg( uint8_t reg );
static void vs10xx_writ_reg( uint8_t reg, uint16_t dat );
static ErrorStatus vs10xx_wait( uint32_t timeout );

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
static uint8_t version = 0;

/* sin test */
static uint8_t sin_start[] = { 0x53, 0xEF, 0x6E, 0x6f, 0, 0, 0, 0 };
static uint8_t sin_end[]   = { 0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0 };

void vs10xx_config( void ) 
{
    
    GPIO_InitTypeDef vs10xx_g;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    
    /* GPIO */
    vs10xx_g.GPIO_Pin               = VS10xx_DREQ;       // PB
    vs10xx_g.GPIO_Mode              = GPIO_Mode_IN;
    vs10xx_g.GPIO_OType             = GPIO_OType_OD;
    vs10xx_g.GPIO_PuPd              = GPIO_PuPd_UP;
    vs10xx_g.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOC, &vs10xx_g );

    vs10xx_g.GPIO_Pin               = VS10xx_XCS | VS10xx_XDCS | VS10xx_XRESET;        // PC
    vs10xx_g.GPIO_Mode              = GPIO_Mode_OUT;
    vs10xx_g.GPIO_OType             = GPIO_OType_PP;
    vs10xx_g.GPIO_PuPd              = GPIO_PuPd_UP;
    vs10xx_g.GPIO_Speed             = GPIO_Speed_100MHz;
    GPIO_Init( GPIOC, &vs10xx_g );

}

void vs10xx_init( void ) 
{ 
    spi_conf( VS10xx_SPI, SPI_BaudRatePrescaler_256, SPI_MODE_1, SPI_Direction_2Lines_FullDuplex );
    vs10xx_config();
    vs10xx_hw_reset();

    version = vs10xx_read_reg( REG_VS10xx_SCI_STATUS )>>4;
    if( version == 0x0f )
        return;    
    
    DEBUG_PRINT( "VS10xx init begin. (%x)\n", version );
    
    vs10xx_writ_reg( REG_VS10xx_SCI_MODE, ( 1<<11 | 1<<6 | 1<<5 ) );        // stream mode and MSB first turn on
    vs10xx_writ_reg( REG_VS10xx_SCI_BASS, 0x7af6 );                         //configures BASS
    vs10xx_writ_reg( REG_VS10xx_SCI_CLOCKF, (SC_MULT6 | SC_ADD0) );

    VOL_SETTING( 150, 150 );    //initialize the vol left:-2dB, right:-2dB
    
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_MODE ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_BASS ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_CLOCKF ) );
    DEBUG_PRINT( "%x\n", vs10xx_read_reg( REG_VS10xx_SCI_VOL ) );

    
//    spi_conf( VS10xx_SPI, SPI_BaudRatePrescaler_128, SPI_MODE_1, SPI_Direction_2Lines_FullDuplex );

    /* vs10xx sin test */
#define SINTEST 1
#if SINTEST == 1
    vs10xx_sin_test( 200 );
#endif
    
    DEBUG_PRINT( "VS10xx init done.\n" );
}

static ErrorStatus vs10xx_wait( uint32_t timeout )
{
    while( !(DREQ_STAT) && timeout-- );
    if ( !timeout )
        return ERROR;
    return SUCCESS;
}

void vs10xx_hw_reset( void ) 
{
    VS10xx_XRESET_LOW;
    DELAY(2);
    VS10xx_XRESET_HIGH;
    DELAY(2);
    vs10xx_wait( 0xFFFF );
}

void vs10xx_sw_reset( void )
{
    
}

uint8_t vs10xx_trans_byte( uint8_t byte ) 
{
    return spi_full_duplex_trans_byte( VS10xx_SPI, byte );
}

uint16_t vs10xx_read_reg( uint8_t reg ) 
{

    uint16_t value = 0x94;
    VS10xx_XDCS_HIGH;
    VS10xx_CS_LOW;
    
    vs10xx_trans_byte( CMD_VS10xx_READ );
    vs10xx_trans_byte( reg );
    value = vs10xx_trans_byte( 0xFF ) << 8;
    value |= vs10xx_trans_byte( 0xFF );
    
    VS10xx_CS_HIGH;
    return value;
}

void vs10xx_writ_reg( uint8_t reg, uint16_t dat ) 
{

    VS10xx_XDCS_HIGH;
    VS10xx_CS_LOW;
    
    vs10xx_trans_byte( CMD_VS10xx_WRIT );
    vs10xx_trans_byte( reg );
    vs10xx_trans_byte( dat >> 8 );                  // 8bit of high
    vs10xx_trans_byte( (uint8_t)dat );              // 8bit of low
    
    VS10xx_CS_HIGH;
}

/************************************************
 * @brief sin测试
 * 
 * @param test_time 
 ************************************************/
void vs10xx_sin_test( uint16_t test_time ) 
{

    DEBUG_PRINT( "VS10xx test begin.\n" );
    
    if (vs10xx_wait( 0xFFFF ) != SUCCESS)
        return;

    VS10xx_XDCS_LOW;
    VS10xx_CS_HIGH;

    for ( uint8_t i = 0; i < 8; i++ )
        vs10xx_trans_byte( sin_start[i] );
    
    DELAY( test_time );
    
    for ( uint8_t i = 0; i < 8; i++ )
        vs10xx_trans_byte( sin_end[i] );
    
    VS10xx_XDCS_HIGH;
    
    DEBUG_PRINT( "VS10xx test done.\n" );
}


/************************************************
 * @brief 播放MP3
 * 
 * @param mp3_file_path 
 ************************************************/
void vs10xx_play_mp3( const char *mp3_file_path )
{
    FIL mp3_fil;
    FRESULT res = f_open( &mp3_fil, mp3_file_path, FA_OPEN_EXISTING | FA_READ );
    if ( res != FR_OK ) {
        DEBUG_PRINT( "open mp3 file : %s error.\n", mp3_file_path );
        return;
    }


    f_close( &mp3_fil );
}

/************************************************
 * @brief 从mp3文件中提取专辑图片
 * 
 * @param mp3_file_path 
 * @param out_path 
 ************************************************/
void extract_mp3_pic( const char *mp3_file_path, const char *out_path )
{
    FIL mp3_fil;
    FRESULT res = f_open( &mp3_fil, mp3_file_path, FA_OPEN_EXISTING | FA_READ );
    if ( res != FR_OK ) {
        DEBUG_PRINT( "open mp3 file : %s error.\n", mp3_file_path );
        return;
    }

    uint32_t jpeg_spos = 0;     // JPEG头 偏移
    uint32_t jpeg_epos = 0;     // JPEG尾 偏移
    uint8_t  pattern_buf[2][2] = {
        { 0xFF, 0xD8 },     // JPEG头
        { 0xFF, 0xD9 }      // JPEG尾
    };
    uint8_t  fbuf[2] = {0};

    /* 找到JPEG文件头 */
    while (1) {
        f_lseek( &mp3_fil, jpeg_spos );
        f_read( &mp3_fil, fbuf, 2, NULL );
        if ( MEMCMP( pattern_buf[0], fbuf, 2 ) == 0 )
            break;
        jpeg_spos += 2;
    }

    jpeg_epos = jpeg_spos;
    
    /* 找到JPEG文件尾 */
    while (1) {
        f_lseek( &mp3_fil, jpeg_epos );
        f_read( &mp3_fil, fbuf, 2, NULL );
        if ( MEMCMP( pattern_buf[1], fbuf, 2 ) == 0 )
            break;
        jpeg_epos += 2;
    }
    f_close( &mp3_fil );
    

    /* 创建文件 */
    uint32_t jpeg_size = jpeg_epos - jpeg_spos;
    uint16_t jpeg_buf_size = 512;
    uint8_t *jpeg_buf = (uint8_t *)MALLOC( sizeof(uint8_t)*jpeg_buf_size );

    uint32_t tt    = jpeg_size / jpeg_buf_size;
    uint32_t sdatn = jpeg_size % jpeg_buf_size;
    if ( sdatn )
        tt += 1;

    uint8_t retry = 0;

    FIL out_fil;
    FRESULT src_fres, dest_fres;

    f_open( &mp3_fil, mp3_file_path, FA_READ | FA_OPEN_EXISTING );
    f_open( &out_fil, out_path, FA_WRITE | FA_OPEN_ALWAYS );

    f_lseek( &mp3_fil, jpeg_spos );
    f_lseek( &out_fil, 0 );

    Clock_Start();
    do {
        /* 传输 */
        retry = 5;
        do {
            src_fres   = f_read( &mp3_fil, jpeg_buf, (tt==1)?(sdatn):(jpeg_buf_size), NULL );
            dest_fres  = f_write( &out_fil, jpeg_buf, (tt==1)?(sdatn):(jpeg_buf_size), NULL );
        } while ( (dest_fres || src_fres) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 : trans error! ( %d %d )\n", dest_fres, src_fres  );
            goto trans_end;
        }
    } while ( --tt );

    
trans_end:
    f_close( &mp3_fil );
    f_close( &out_fil );
    FREE( jpeg_buf );

    double ttime = 0;
    double sec   = 0;
    double speed = 0;
    
    /* 计算传输速度 */
    ttime = Clock_End();
    sec   = ttime / 84000.0;
    speed = (double)jpeg_size / sec;
    printf( "\nsec         : %f s\n", sec  );
    printf( "preriod     : %d s\n", (int)ttime  );
    printf( "trans speed : %0.4f MiB/s\n", speed/(1<<20) );


    DEBUG_PRINT( "%d %d\n", jpeg_spos, jpeg_epos );

}
