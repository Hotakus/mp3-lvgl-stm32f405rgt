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
#include "uart.h"
#include "pro_conf.h"
#include "ff_user.h"
#include "tim.h"

/************************************************
 * @brief STATIC PROPOTYPE
 ************************************************/
static void vs10xx_config(void);
static uint8_t vs10xx_trans_byte(uint8_t byte);
static uint16_t vs10xx_read_reg(uint8_t reg);
static void vs10xx_writ_reg(uint8_t reg, uint16_t dat);
static ErrorStatus vs10xx_wait(uint32_t timeout);
static void vs10xx_reset_decodeTime(void);

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
/* sin test */
static uint8_t sin_start[] = { 0x53, 0xEF, 0x6E, 0x6f, 0, 0, 0, 0 };
static uint8_t sin_end[] = { 0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0 };

static SPI_HandleTypeDef h_vs10xx_spi = {
  .Instance = VS10xx_SPI,
};

/************************************************
 * @brief GPIO配置
 ************************************************/
void vs10xx_config(void)
{

  GPIO_InitTypeDef vs10xx_g;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* GPIO */
  vs10xx_g.Pin = VS10xx_DREQ;       // PC
  vs10xx_g.Mode = GPIO_MODE_INPUT;
  vs10xx_g.Pull = GPIO_NOPULL;
  vs10xx_g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &vs10xx_g);

  vs10xx_g.Pin = VS10xx_XCS | VS10xx_XDCS | VS10xx_XRESET;        // PC
  vs10xx_g.Mode = GPIO_MODE_OUTPUT_PP;
  vs10xx_g.Pull = GPIO_PULLUP;
  vs10xx_g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &vs10xx_g);

}


void vs10xx_reset_decodeTime(void)
{
  vs10xx_writ_reg(REG_VS10xx_SCI_DECODETIME, 0x0000);
  vs10xx_writ_reg(REG_VS10xx_SCI_DECODETIME, 0x0000);//操作两次
}

/************************************************
 * @brief vs10xx初始化
 ************************************************/
void vs10xx_init(void)
{
  spi_conf(
    &h_vs10xx_spi,
    SPI_BAUDRATEPRESCALER_256,
    SPI_MODE_1,
    SPI_DIRECTION_2LINES
  );

  vs10xx_config();

  vs10xx_hw_reset();
  vs10xx_sw_reset();

  /* 读寄存器状态 */
  DEBUG_PRINT("%x\n", vs10xx_read_reg(REG_VS10xx_SCI_MODE));
  DEBUG_PRINT("%x\n", vs10xx_read_reg(REG_VS10xx_SCI_BASS));
  DEBUG_PRINT("%x\n", vs10xx_read_reg(REG_VS10xx_SCI_CLOCKF));
  DEBUG_PRINT("%x\n", vs10xx_read_reg(REG_VS10xx_SCI_VOL));


  spi_conf(
    &h_vs10xx_spi,
    SPI_BAUDRATEPRESCALER_128,
    SPI_MODE_1,
    SPI_DIRECTION_2LINES
  );

  /* vs10xx sin test */
// #define SINTEST 1
// #if SINTEST == 1
//     vs10xx_sin_test( 200 );
// #endif

  DEBUG_PRINT("VS10xx init done.\n");
}

/************************************************
 * @brief DREQ等忙
 *
 * @param timeout 超时时间
 * @return ErrorStatus 等待状态
 ************************************************/
static ErrorStatus vs10xx_wait(uint32_t timeout)
{
  while (--timeout && !DREQ_STAT);
  if (!timeout)
    return ERROR;
  return SUCCESS;
}

void vs10xx_hw_reset(void)
{
  VS10xx_XRESET_LOW;
  DELAY(20);
  VS10xx_CS_HIGH;
  VS10xx_XDCS_HIGH;
  VS10xx_XRESET_HIGH;
  vs10xx_wait(0xFFFFFFFF);
}

/************************************************
 * @brief 软复位并配置相关寄存器
 ************************************************/
void vs10xx_sw_reset(void)
{
  ErrorStatus err = ERROR;

  /* 新模式、使能SinTest、使能流模式、软复位 */
  vs10xx_writ_reg(REG_VS10xx_SCI_MODE, (1 << 11 | 1 << 5 | 1 << 2 | 1 << 1));
  err = vs10xx_wait(0xFFFFFFFF);  // 等软复位结束
  if (err != SUCCESS) {
    DEBUG_PRINT("Soft reset occured an error.\n");
    return;
  }

  /* 相关配置 */
  vs10xx_writ_reg(REG_VS10xx_SCI_BASS, 0x7af6);                         //configures BASS
  vs10xx_writ_reg(REG_VS10xx_SCI_CLOCKF, (SC_MULT6 | SC_ADD3));
  VOL_SETTING(180, 180);    //initialize the vol left:-2dB, right:-2dB

  err = vs10xx_wait(0xFFFFFFFF);  // 等配置结束
  if (err != SUCCESS) {
    DEBUG_PRINT("Configuration occured an error.\n");
    return;
  }
}

uint8_t vs10xx_trans_byte(uint8_t byte)
{
  return spi_full_duplex_trans_byte(&h_vs10xx_spi, byte);
}

uint16_t vs10xx_read_reg(uint8_t reg)
{

  uint16_t value = 0x94;
  VS10xx_XDCS_HIGH;
  VS10xx_CS_LOW;

  vs10xx_trans_byte(CMD_VS10xx_READ);
  vs10xx_trans_byte(reg);
  value = vs10xx_trans_byte(0xFF) << 8;
  value |= vs10xx_trans_byte(0xFF);

  VS10xx_CS_HIGH;
  return value;
}

void vs10xx_writ_reg(uint8_t reg, uint16_t dat)
{

  VS10xx_XDCS_HIGH;
  VS10xx_CS_LOW;

  vs10xx_trans_byte(CMD_VS10xx_WRIT);
  vs10xx_trans_byte(reg);
  vs10xx_trans_byte(dat >> 8);                  // 8bit of high
  vs10xx_trans_byte((uint8_t)dat);              // 8bit of low

  VS10xx_CS_HIGH;
}

/************************************************
 * @brief sin测试
 *
 * @param test_time
 ************************************************/
void vs10xx_sin_test(uint16_t test_time)
{

  DEBUG_PRINT("VS10xx test begin.\n");

  if (vs10xx_wait(0xFFFFFFFF) != SUCCESS)
    return;

  VS10xx_XDCS_LOW;
  VS10xx_CS_HIGH;

  for (uint8_t i = 0; i < 8; i++)
    vs10xx_trans_byte(sin_start[i]);

  DELAY(test_time);

  for (uint8_t i = 0; i < 8; i++)
    vs10xx_trans_byte(sin_end[i]);

  VS10xx_XDCS_HIGH;

  DEBUG_PRINT("VS10xx test done.\n");
}


/************************************************
 * @brief 播放支持的音频
 *
 * @param audio_path
 ************************************************/
void vs10xx_playback(const char* audio_path)
{
  FIL* music_file = (FIL*)MALLOC(sizeof(FIL));
  FRESULT fres;

  fres = f_open(music_file, audio_path, FA_OPEN_EXISTING | FA_READ);
  if (fres != FR_OK) {
    DEBUG_PRINT("playback open \"%s\" error. (%d)\n", audio_path, fres);
    return;
  }

  uint8_t h_buf[4] = { 0 };
  f_lseek(music_file, 0);
  f_read(music_file, h_buf, 4, NULL);

  /* mem释放 */
  f_close(music_file);
  FREE(music_file);

  /* 判断音频文件格式 */
  /* MP3 */
  if (MEMCMP("ID3", h_buf, 3) == 0) {
    DEBUG_PRINT("play MP3.\n");
    vs10xx_play_mp3(audio_path);
  }

}

/************************************************
 * @brief 播放MP3
 *
 * @param mp3_file_path
 ************************************************/
#define BYTES_PER_TRANS     32
void vs10xx_play_mp3(const char* path)
{

  mp3_info_t mp3;

  mp3_analyse(path, &mp3);

  DEBUG_PRINT("mp3 size  : %d Bytes\n", mp3.file_size);
  DEBUG_PRINT("mp3 times : %d:%d min.\n", mp3.play_times / 60, mp3.play_times % 60);

  FIL mp3_fil;
  FRESULT res = f_open(&mp3_fil, path, FA_OPEN_EXISTING | FA_READ);
  if (res != FR_OK) {
    DEBUG_PRINT("open mp3 file : %s error.\n", path);
    return;
  }

  /* 得到首帧地址 */
  // size_t cur_pos = mp3.frame_spos;
  size_t cur_pos = mp3.frame_spos;
  uint8_t* a_buf = (uint8_t*)MALLOC(sizeof(uint8_t) * BYTES_PER_TRANS);

  DEBUG_PRINT("mp3.sample_rate  : %d \n", mp3.sample_rate);


  vs10xx_reset_decodeTime();
  VS10xx_CS_HIGH;
  f_lseek(&mp3_fil, cur_pos);
  while (cur_pos < mp3.frame_epos) {
    f_read(&mp3_fil, a_buf, BYTES_PER_TRANS, NULL);
    if (vs10xx_wait(0xFFFFFFFF) != SUCCESS) {
      DEBUG_PRINT("Play MP3 file occured an error.\n");
      DEBUG_PRINT("DREQ : %d\n", DREQ_STAT);
      break;
    }
    VS10xx_XDCS_LOW;
    for (uint16_t len = 0; len < BYTES_PER_TRANS; len++)
      vs10xx_trans_byte(a_buf[len]);
    cur_pos += BYTES_PER_TRANS;
    VS10xx_XDCS_HIGH;
    // uint16_t c_time = vs10xx_read_reg(REG_VS10xx_SCI_DECODETIME);
    // printf("Cur time : %d : %d", c_time / 60, c_time % 60);
    // my_putc('\r');
  }

  f_close(&mp3_fil);
  FREE(a_buf);
}
