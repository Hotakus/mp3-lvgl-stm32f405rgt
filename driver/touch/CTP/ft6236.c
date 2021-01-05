/************************************************
 * @file ft6236.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-20
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "iic_conf.h"
#include "ft6236.h"


/************************************************
 * @brief FUNCTION PROPOTYPE
 ************************************************/
static void ctp_ft6236_gpio(void);
static void ctp_ft6236_reset(void);

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
static touch_coordinate_s touch_point[FT6236_MAX_TOUCH_NUM];

/************************************************
 * @brief GLOBAL VARIABLE
 ************************************************/
uint8_t ft6236_init_flag = 0;
uint8_t ft6236_error_times;

/************************************************
 * @brief FUNCTION REALIZED
 ************************************************/
static void ctp_ft6236_gpio(void)
{
    GPIO_InitTypeDef ctp_g;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    ctp_g.Mode = GPIO_MODE_OUTPUT_PP;
    ctp_g.Pin = FT6236_RST;
    ctp_g.Pull = GPIO_PULLUP;
    ctp_g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &ctp_g);

    //    ctp_g.GPIO_Mode     = GPIO_Mode_IN;
    //    ctp_g.GPIO_Pin      = FT6236_INT;
    //    ctp_g.GPIO_PuPd     = GPIO_PuPd_UP;
    //    ctp_g.GPIO_Speed    = GPIO_Speed_100MHz;
    //    GPIO_Init( GPIOA, &ctp_g );
}

/************************************************
 * @brief ft6236 initialized function
 ************************************************/
void ctp_ft6236_init(void)
{
    uint8_t val = 0x0;

    ctp_ft6236_gpio();
    ctp_ft6236_reset();

    DEBUG_PRINT("FT6236 configured start.\n");
    /* gesture 配置 */
    val = 0x0A;
    ctp_ft6236_writ_reg(RADIAN_VALUE, &val, 1);
    val = 0x19;
    ctp_ft6236_writ_reg(OFFSET_LEFT_RIGHT, &val, 1);
    ctp_ft6236_writ_reg(OFFSET_UP_DOWN, &val, 1);
    ctp_ft6236_writ_reg(DISTANCE_LEFT_RIGHT, &val, 1);
    ctp_ft6236_writ_reg(DISTANCE_UP_DOWN, &val, 1);
    val = 0x32;
    ctp_ft6236_writ_reg(DISTANCE_ZONE, &val, 1);

    /* 初始化配置 */
    val = 22;
    ctp_ft6236_writ_reg(FT_THGROUP, &val, 1);
    val = 12;
    ctp_ft6236_writ_reg(FT_PERIODACTIVE, &val, 1);
    val = 0;
    ctp_ft6236_writ_reg(FT_MODE, &val, 1);
    val = 0;
    ctp_ft6236_writ_reg(FT_DEVIDE_MODE, &val, 1);

    DEBUG_PRINT("FT6236 configured done.\n");
}

/************************************************
 * @brief read x bytes from register indicated
 *
 * @param reg_addr
 * @param val
 * @param len
 * @return HAL_StatusTypeDef
 ************************************************/
HAL_StatusTypeDef ctp_ft6236_read_reg(uint8_t reg_addr, uint8_t* val, uint32_t len)
{
    HAL_StatusTypeDef err;

    err = HAL_I2C_Master_Transmit(&h_i2c1, FT6236_ADDR, &reg_addr, 1, 0xFFFFFFFF);
    if (err != HAL_OK)if (err != HAL_OK) {
        DEBUG_PRINT("r1err (%d)\n", err);
        return err;
    }

    err = HAL_I2C_Master_Receive(&h_i2c1, FT6236_ADDR, val, 1, 0xFFFFFFFF);
    if (err != HAL_OK)if (err != HAL_OK) {
        DEBUG_PRINT("r2err (%d)\n", err);
        return err;
    }
    return HAL_OK;
}

/************************************************
 * @brief write x bytes to register indicated
 *
 * @param reg_addr
 * @param val
 * @param len
 ************************************************/
void ctp_ft6236_writ_reg(uint8_t reg_addr, uint8_t* val, uint32_t len)
{
    HAL_StatusTypeDef err;
    uint8_t buf[2] = { 0 };

    buf[0] = reg_addr;
    buf[1] = *val;
    err = i2c_send(&h_i2c1, FT6236_ADDR, buf, sizeof(buf));
    if (err != HAL_OK) {
        DEBUG_PRINT("w1err (%d)\n", err);
        return;
    }
}

/************************************************
 * @brief ft6236 reset
 ************************************************/
static void ctp_ft6236_reset(void)
{
    FT6236_RST_LOW;
    DELAY(6);
    FT6236_RST_HIGH;
    DELAY(310);
}

/************************************************
 * @brief 扫描
 ************************************************/
void ctp_ft6236_scan(void)
{
    ctp_ft6236_get_coordinate(FT_TP1);
    DEBUG_PRINT("x : %d y : %d \n", touch_point[0].x, touch_point[0].y);
    //cp_ft6236_get_gesture();
}

/************************************************
 * @brief 获取触摸点坐标
 *
 * @param TPx
 * @return touch_coordinate_s*
 ************************************************/
touch_coordinate_s* ctp_ft6236_get_coordinate(uint8_t TPx)
{
    uint8_t xh, xl;
    uint8_t yh, yl;
    uint8_t tp;

    /* 参数错误检查 */
    switch (TPx) {
    case FT_TP1:
    case FT_TP2:
        break;
    default:
        return NULL;
    }

    ctp_ft6236_read_reg(TPx, &xh, 1);  // 读触摸点状态和x高4位
    ctp_ft6236_read_reg(TPx + 1, &xl, 1);  // 读x低8位
    ctp_ft6236_read_reg(TPx + 2, &yh, 1);  // 读y高4位
    ctp_ft6236_read_reg(TPx + 3, &yl, 1);  // 读y低8位

    if (TPx == FT_TP1)
        tp = 0;
    else if (TPx == FT_TP2)
        tp = 1;

    touch_point[tp].event_flag = xh >> 6;
#if FT6236_DIRECTION == FT6236_NOMAL
    touch_point[tp].x = (xh & 0x0F) << 8;
    touch_point[tp].x |= xl;
    touch_point[tp].y = (yh & 0x0F) << 8;
    touch_point[tp].y |= yl;
#elif FT6236_DIRECTION == FT6236_ROTATE_1
    touch_point[tp].y = (xh & 0x0F) << 8;
    touch_point[tp].y |= xl;
    touch_point[tp].y = FT6236_MAX_X - touch_point[tp].y;                // y坐标颠倒
    touch_point[tp].x = (yh & 0x0F) << 8;
    touch_point[tp].x |= yl;
#elif FT6236_DIRECTION == FT6236_ROTATE_2

#elif FT6236_DIRECTION == FT6236_ROTATE_3 

#endif

    return &touch_point[tp];
}

/************************************************
 * @brief 获取姿态
 ************************************************/
touch_gesture_t cp_ft6236_get_gesture(void)
{
    uint8_t gesture = 0;
    ctp_ft6236_read_reg(FT_GEST_ID, &gesture, 1);
    if (gesture != 0)
        DEBUG_PRINT("gesture : %02d\n", gesture);
    return FT_GEST_NO_GEST;
}



