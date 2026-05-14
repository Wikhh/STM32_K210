


#ifndef __BATTERY_VOLTAGE_H
#define __BATTERY_VOLTAGE_H	

 #include "interface.h"//IO口定义头文件 很重要		  
   
extern unsigned char battery_voltage_flag;          //电池电压测量标志位
extern unsigned char battery_voltage_display_flag; //电池电压显示标志位
extern unsigned int battery_voltage_int;
extern unsigned char battery_voltage_decimals;
extern float battery_voltage;//电池电压变量

void voltage_init(void); //电池电压引脚采集引脚初始化
void battery_voltage_display_execute(void);       //电池电压测量函数
void battery_voltage_execute(void);   //电池电压显示函数
float get_battery_voltage(void);//得到电池的电压值


	
#define VOLTAGE_TIME	 20				//定义电压采集读取次数



#endif 


