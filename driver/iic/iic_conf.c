/************************************************
 * @file iic_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-18
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/

#include "iic_conf.h"
#include "uart.h"

#define  I2C_NUM    3

I2C_HandleTypeDef h_i2c1 = {
    .Instance = I2C1,
};

static uint16_t i2c_clk_m = 1000;


void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (i2cHandle->Instance == I2C1) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = I2C1_SCL | I2C1_SDA;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2C1 clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

    if (i2cHandle->Instance == I2C1) {

        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(GPIOB, I2C1_SCL);
        HAL_GPIO_DeInit(GPIOB, I2C1_SDA);
    }
}

/************************************************
 * @brief I2C configure
 *
 * @param h_i2c
 * @param i2c_clk
 * @param own_addr
*************************************************/
void i2c_conf(I2C_HandleTypeDef* h_i2c, uint16_t i2c_clk, uint8_t own_addr)
{
    if (i2c_clk > 400) {
        DEBUG_PRINT("i2c clk error\n");
        return;
    }

    if (h_i2c->Instance == NULL)
        return;

    HAL_I2C_DeInit(h_i2c);

    h_i2c->Init.ClockSpeed = i2c_clk * i2c_clk_m;
    h_i2c->Init.OwnAddress1 = own_addr;
    h_i2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
    h_i2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    h_i2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    h_i2c->Init.OwnAddress2 = 0;
    h_i2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    h_i2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(h_i2c) != HAL_OK) {
        DEBUG_PRINT("I2C init Error.\n");
    }

}

/************************************************
 * @brief I2Cx 发送
 *
 * @param I2Cx 对应1...3
 * @param dev_addr
 * @param send_buf
 * @param size
 * @return HAL_StatusTypeDef
*************************************************/
HAL_StatusTypeDef i2c_send(I2C_HandleTypeDef* h_i2c, uint16_t dev_addr, uint8_t* send_buf, size_t size)
{
    if (h_i2c->Instance == NULL)
        return HAL_ERROR;
    return HAL_I2C_Master_Transmit(h_i2c, dev_addr, send_buf, size, I2C_TIMEOUT);
}


HAL_StatusTypeDef i2c_read(I2C_HandleTypeDef* h_i2c, uint16_t dev_addr, uint8_t* rec_buf, size_t size)
{
    if (h_i2c->Instance == NULL)
        return HAL_ERROR;
    return HAL_I2C_Master_Receive(h_i2c, dev_addr, rec_buf, size, I2C_TIMEOUT);
}
