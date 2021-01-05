/************************************************
 * @file sht20.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-30
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "sht20.h"
#include "iic_conf.h"

static double RH = 0;
static double TEMP = 0;

static HAL_StatusTypeDef sht_reset(void);


/************************************************
 * @brief reset sht
 *
 * @return HAL_StatusTypeDef
 ************************************************/
static HAL_StatusTypeDef sht_reset(void)
{
    HAL_StatusTypeDef err = HAL_ERROR;

    uint8_t dat = 0xFE;
    err = i2c_send(&h_i2c1, SHT20_ADDR, &dat, 1);
    if (err != HAL_OK)
        return err;

    return err;
}

/************************************************
 * @brief sht init
 ************************************************/
void sht_init(void)
{
    static uint8_t flag = 0;
    if (!flag) {
        sht_reset();

        uint8_t dat = 0x3;
        sht_writ_user_reg(&dat);
        dat = 0;
        sht_read_user_reg(&dat);
        DEBUG_PRINT("ureg: %x\n", dat);

        flag = 1;
    }

    sht_read_RH_TEMP(0);
    sht_read_RH_TEMP(1);

}
#if USER_USE_RTTHREAD == (1u)
MSH_CMD_EXPORT(sht_init, sht_init);
#endif

/************************************************
 * @brief 读sht的user寄存器
 *
 * @param dat
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef sht_read_user_reg(uint8_t* dat)
{
    HAL_StatusTypeDef err = HAL_ERROR;
    uint8_t buf = 0;

    buf = 0xE7;
    err = i2c_send(&h_i2c1, SHT20_ADDR, &buf, 1);
    if (err != HAL_OK)
        return err;

    err = i2c_read(&h_i2c1, SHT20_ADDR, dat, 1);
    if (err != HAL_OK)
        return err;

    return err;
}

/************************************************
 * @brief 写user寄存器
 *
 * @param dat
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef sht_writ_user_reg(uint8_t* dat)
{
    HAL_StatusTypeDef err = HAL_ERROR;
    uint8_t buf[2] = { 0 };

    buf[0] = 0xE6;
    buf[1] = *dat;
    err = i2c_send(&h_i2c1, SHT20_ADDR, buf, 2);
    if (err != HAL_OK)
        return err;

    return err;
}


/************************************************
 * @brief 读温湿度
 *
 * @param which 0:RH  1:TEMP
 * @return double
 ************************************************/
double sht_read_RH_TEMP(uint8_t which)
{
    HAL_StatusTypeDef err = HAL_ERROR;
    uint8_t buf[3] = { 0 };
    uint8_t cmd = 0;
    uint8_t delay_time = 0;

    if (!which) {
        cmd = 0xF5;
        delay_time = 25;
    } else if (which) {
        cmd = 0xF3;
        delay_time = 80;
    }

    err = i2c_send(&h_i2c1, SHT20_ADDR, &cmd, 1);
    if (err != HAL_OK)
        return err;

    DELAY(delay_time);

    err = i2c_read(&h_i2c1, SHT20_ADDR, buf, 3);
    if (err != HAL_OK)
        return err;


    uint16_t res_bin = 0;
    res_bin = buf[0] << 6;
    res_bin |= buf[1] >> 2;

    if (!which) {
        RH = 125 * (res_bin << 2) / (1 << 16) - 6;
        printf("RH : %0.2f %%\n", RH);
        return RH;
    } else if (which) {
        TEMP = 175.72 * (res_bin << 2) / (1 << 16) - 46.85;
        printf("TEMP : %0.2f ℃\n", TEMP);
        return TEMP;
    }

    return 0xFF;
}
