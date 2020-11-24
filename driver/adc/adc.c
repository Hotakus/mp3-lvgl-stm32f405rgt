/************************************************
 * @file adc.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "adc.h"

static ADC_InitTypeDef          adc_init_t;
static ADC_CommonInitTypeDef    adc_comm_t;
static GPIO_InitTypeDef         adc_g;

static uint8_t adc_conf_flag = 0;

void adc_init( ADC_TypeDef *ADCx )
{

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , ENABLE );

    adc_g.GPIO_Pin       = GPIO_Pin_1;
    adc_g.GPIO_Mode      = GPIO_Mode_AN;
    adc_g.GPIO_OType     = GPIO_OType_OD;
    adc_g.GPIO_PuPd      = GPIO_PuPd_DOWN;
    adc_g.GPIO_Speed     = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &adc_g );

    adc_init_t.ADC_Resolution           = ADC_Resolution_12b;
    adc_init_t.ADC_DataAlign            = ADC_DataAlign_Right;
    adc_init_t.ADC_ContinuousConvMode   = DISABLE;
    adc_init_t.ADC_ScanConvMode         = DISABLE;
    adc_init_t.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T1_CC1;
    adc_init_t.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    adc_init_t.ADC_NbrOfConversion      = 1;
    ADC_Init( ADCx, &adc_init_t );

    adc_comm_t.ADC_Mode                 = ADC_Mode_Independent;
    adc_comm_t.ADC_DMAAccessMode        = ADC_DMAAccessMode_Disabled;
    adc_comm_t.ADC_Prescaler            = ADC_Prescaler_Div2;
    adc_comm_t.ADC_TwoSamplingDelay     = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit( &adc_comm_t );

    ADC_Cmd( ADCx, ENABLE );
}

uint16_t adc_get_value( ADC_TypeDef *ADCx, uint8_t adc_channel )   
{

    if ( !adc_conf_flag ) {
        adc_init( ADC1 );
        adc_conf_flag = 1;
    }

  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADCx, adc_channel, 1, ADC_SampleTime_144Cycles );		    
	ADC_SoftwareStartConv( ADCx );		        //使能指定的ADC1的软件转换启动功能	
	while(!( ADCx->SR & ADC_SR_EOC ));          //等待转换结束
	return ADC_GetConversionValue( ADCx );	    //返回最近一次ADC1规则组的转换结果
}
