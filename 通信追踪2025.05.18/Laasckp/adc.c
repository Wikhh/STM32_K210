#include "adc.h"//adc头文件
              
void  adc1_init(void)//ADC1初始化函数  
{      
 ADC_InitTypeDef ADC_InitStructure; 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );    //使能ADC1通道时钟
 RCC_ADCCLKConfig(RCC_PCLK2_Div6);                         //设置ADC时钟分频
 ADC_DeInit(ADC1);                                         //复位ADC1,将外设ADC1的全部寄存器重设为缺省值
 //ADC32_InitStructure参数设置
 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   //独立模式
 ADC_InitStructure.ADC_ScanConvMode = DISABLE;                        //关闭扫描模式
 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                  //单次转换模式
 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //软件触发启动
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               //数据右对齐
 ADC_InitStructure.ADC_NbrOfChannel = 1;                              //顺序进行规则转换的ADC通道的数目
 ADC_Init(ADC1, &ADC_InitStructure);   
 ADC_Cmd(ADC1, ENABLE);                                               //使能指定的ADC1
 //下面四个函数用于校准
 ADC_ResetCalibration(ADC1);                                          //使能复位校准
 while(ADC_GetResetCalibrationStatus(ADC1));                          //等待复位校准结束
 ADC_StartCalibration(ADC1);                                          //开启AD校准
 while(ADC_GetCalibrationStatus(ADC1));                               //等待校准结束
}   

//获得 ADC 值
//ch:通道值 0~3
u16 get_adc1(u8 ch)   
{
 //设置指定ADC的规则组通道，四个入口参数
 ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );          
 ADC_SoftwareStartConvCmd(ADC1, ENABLE);         //使能指定的ADC1的软件转换启动功能  
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )); //等待转换结束
 return ADC_GetConversionValue(ADC1);            //返回最近一次ADC1规则组的转换结果
} 


u16 get_adc1_average(u8 adc1_chx)//得到adc1_channel通道VOLTAGE_TIME次采样的平均值
{
  u16 voltage_average_value=0;
	u8 t;
	for(t=0;t<VOLTAGE_TIME;t++)
	{
		voltage_average_value+=get_adc1(adc1_chx);	//读取ADC1 VOLTAGE_TIME次相加的值
		delay_ms(5);
	}
	  voltage_average_value/=VOLTAGE_TIME;//得到平均值
    return (u16)voltage_average_value;//返回平均值
}
