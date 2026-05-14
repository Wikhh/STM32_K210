#include "battery_voltage.h"//电池电压测量头文件

unsigned char battery_voltage_flag=0;          //电池电压测量标志位
unsigned char battery_voltage_display_flag=0; //电池电压显示标志位
unsigned int battery_voltage_int=0;
unsigned char battery_voltage_decimals=0;
float battery_voltage=0;

//电池电压引脚采集引脚初始化
void voltage_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			//PA4 模拟输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
}


//得到电池的电压值
float get_battery_voltage(void)
{
  
	
	battery_voltage = get_adc1_average(VOLTAGE_ADC1_CHx)*3.3*11.0*1.008/4096;	//

	return battery_voltage;
}


void battery_voltage_execute(void) //电压测量执行函数
{
	
	  get_battery_voltage();
	  battery_voltage_int=battery_voltage*100;
	  OLED_ShowNum(40,3,battery_voltage_int/100,1,0);   
	  OLED_ShowNum(60,3,battery_voltage_int%100,2,0);  
    if(battery_voltage_int%100<10) 
		OLED_ShowNum(60,3,0,1,0);  
}


  
	void battery_voltage_display_execute()  //蜂鸣器显示和执行函数
 {
		
				if(battery_voltage_display_flag==1) //蜂鸣器检测显示标志位置1
		{
			
			     CarStop();
			     sudu=100;                           //设置小车速度为100
				   OLED_Clear();	                    //OLED屏幕清屏
           OLED_ShowCH(15,0,"测量电池电压");	     //显示电池电压文字
			     OLED_ShowCH(0,3,"电压:");	     //显示电压： 文字
			     OLED_ShowCH(50,3,".");	     //显示.
			     OLED_ShowCH(80,3,"V");	     //显示V
			     OLED_ShowCH(3,6,"设置速度:");	  //显示 设置速度 文字
           OLED_ShowCH(110,6,"%");		     //显示 % 符号
           OLED_ShowNum(80,6,sudu,3,0);   //显示 当前设置速度 数值
			     battery_voltage_display_flag=0;        //电池电压显示执行标志位置0			   
			     IrDA=0;                      //红外码值清零，防止干扰
			
		}
				
		       if(battery_voltage_flag==1)//遥控按键6 电池电压测量标志位变成1
		{			
		       
            battery_voltage_execute();
		 			
		}		

 }
