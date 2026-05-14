

#ifndef __ADC_H
#define __ADC_H	

#include "interface.h"//IO口定义头文件 很重要		  

u16  get_adc1(u8 ch);//得到ADC1某个通道值   
u16  get_adc1_average(u8 adc1_chx);//得到adc1_chx通道VOLTAGE_TIME次采样的平均值
void adc1_init(void);//ADC1初始化

#endif 


