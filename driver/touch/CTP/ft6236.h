#ifndef FT6236_H
#define FT6236_H

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>


/* definations */
/************************************************
 * @brief FT6236 I2C 通讯定义 
 ************************************************/
#define FT6236_ADDR         0x70

#define FT6236_WRIT         0x00
#define FT6236_READ         0x01

#define FT6236_MAX_TOUCH_NUM    2
#define FT6236_MAX_X            240
#define FT6236_MAX_Y            320

/************************************************
 * @brief FT6236 坐标递增方向
 ************************************************/
#define FT6236_NOMAL            0       // 普通模式 原点在左上角
#define FT6236_ROTATE_1         1       // 原点在右上角
#define FT6236_ROTATE_2         2       // 原点在右下角
#define FT6236_ROTATE_3         3       // 原点在左下角
#define FT6236_DIRECTION        FT6236_ROTATE_1

/************************************************
 * @brief FT6236 I2C 接口定义
 ************************************************/
#define FT6236_I2C          I2C1
#define FT6236_RST          GPIO_Pin_0  // PA
#define FT6236_INT          GPIO_Pin_1  // PA
#define FT6236_RST_HIGH     GPIOA->BSRRL  |= FT6236_RST
#define FT6236_RST_LOW      GPIOA->BSRRH  |= FT6236_RST

/************************************************
 * @brief FT6236 部分寄存器定义 
 ************************************************/
#define FT_DEVIDE_MODE 			0x00   		// FT6236模式控制寄存器
#define FT_GEST_ID              0x01        // 手势寄存器
#define FT_TD_STATUS            0x02		// 触摸状态寄存器

#define FT_TP1_REG 				0X03	  	// 第一个触摸点高4位数据地址
#define FT_TP2_REG 				0X09		// 第二个触摸点高4位数据地址
// #define FT_TP3_REG 				0X0F		// 第三个触摸点高4位数据地址
// #define FT_TP4_REG 				0X15		// 第四个触摸点高4位数据地址
// #define FT_TP5_REG 				0X1B		// 第五个触摸点高4位数据地址

#define FT_TP1  FT_TP1_REG
#define FT_TP2  FT_TP2_REG
// #define FT_TP3  FT_TP3_REG
// #define FT_TP4  FT_TP4_REG
// #define FT_TP5  FT_TP5_REG

#define FT_THGROUP			    0x80   		// 触摸有效值设置寄存器
#define FT_THDIFF               0x85        
#define FT_PERIODACTIVE	        0x88   		// 激活状态周期设置寄存器  
 
#define	FT_LIB_VERSIONH         0xA1		// 版本H4
#define	FT_LIB_VERSIONL	        0xA2		// 版本L4
#define FT_CIPHER               0xA3        // 芯片ID
#define FT_MODE 			    0xA4   		// FT6236中断模式控制寄存器 0:open 1:close
#define FOCALTECH_ID    		0xA8		// 芯片ID



/************************************************
 * @brief FT6236 触摸点结构体
 ************************************************/
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t  event_flag;   
} touch_coordinate_s;

/************************************************
 * @brief FT6236 触摸姿态定义
 ************************************************/
typedef enum {
    FT_GEST_NO_GEST         = 0x00,
    FT_GEST_MOVE_UP         = 0x10,
    FT_GEST_MOVE_LEFT       = 0x14,
    FT_GEST_MOVE_DOWN       = 0x18,
    FT_GEST_MOVE_RIGHT      = 0x1C,
    FT_GEST_ZOOM_IN         = 0x48,
    FT_GEST_ZOOM_OUT        = 0x49,
} touch_gesture_t;

/************************************************
 * @brief FT6236 触摸姿态配置寄存器
 ************************************************/
enum ft6236_gestrue_reg {
    RADIAN_VALUE        = 0x91,     // 
    OFFSET_LEFT_RIGHT         ,     // 
    OFFSET_UP_DOWN            ,     // 
    DISTANCE_LEFT_RIGHT       ,     // 
    DISTANCE_UP_DOWN          ,     // 
    DISTANCE_ZONE             ,     // 
};

/************************************************
 * @brief FT6236 函数
 ************************************************/
void ctp_ft6236_init( void );
void ctp_ft6236_read_reg( uint8_t reg_addr, uint8_t *val, u32 len );
void ctp_ft6236_writ_reg( uint8_t reg_addr, uint8_t *val, u32 len );
touch_coordinate_s *ctp_ft6236_get_coordinate( uint8_t TPx );
touch_gesture_t cp_ft6236_get_gesture( void );

#endif
