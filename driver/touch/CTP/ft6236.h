#ifndef FT6236_H
#define FT6236_H

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>


/* definations */
#define FT6236_ADDR     0x70

#define FT6236_WRIT     0x00
#define FT6236_READ     0x01

#define FT6236_I2C      I2C1

#define FT6236_RST      GPIO_Pin_0  // PA
#define FT6236_INT      GPIO_Pin_1  // PA

#define FT6236_RST_HIGH    GPIOA->BSRRL |= FT6236_RST
#define FT6236_RST_LOW     GPIOA->BSRRH |= FT6236_RST

//FT6236 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT6236模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT6236中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器  
#define Chip_Vendor_ID          0xA3        //芯片ID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

/* functions */
void ctp_ft6236_init( void );
void ctp_ft6236_read_reg( uint8_t reg_addr, uint8_t *val, u32 len );
void ctp_ft6236_writ_reg( uint8_t reg_addr, uint8_t *val, u32 len );

#endif
