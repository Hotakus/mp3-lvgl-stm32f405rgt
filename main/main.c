/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-01
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "main.h"
#include "lvgl.h"
#include "iic_conf.h"
#include "spi_conf.h"

#include "ft6236.h"

static void periphal_init(void)
{
#if USER_USE_OS == 0
    HAL_Init();
    uart1_conf(115200);
#endif

    DELAY(100);

    /* I2C1 init */
    i2c_conf(&h_i2c1, 100, 0x94);

    /* SPI1 init */
    spi_conf(
      &h_spi1,
      SPI_BAUDRATEPRESCALER_2,
      SPI_MODE_4,
      SPI_DIRECTION_2LINES
    );
}

static void device_init(void)
{
    // ctp_ft6236_init();
}

int main()
{
    periphal_init();
    device_init();

    return 0;
}
