
/******************************************************
  功能：1.两路红外避障       
        2.两路红外循迹
        3.速度和总里程测量
        4.超声波舵机旋转避障
        5.蜂鸣器鸣叫
        6.电池电压测量
        7.OLED显示屏显示
        8.PWM速度调节
        9.红外遥控控制
       10.安卓蓝牙APP控制(选配)
	   11.接收声源信息寻迹


  汇总程序：请参考interface.c
  接口定义：请参考interface.h
 			
******************************************************/


#include "interface.h" //模块接口定义头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float sound_Kp=00.60,sound_Ki=0.000,sound_Kd=0;
int pack;


    int main(void)
 { 	 
	
	all_init();//初始化所有想要的函数	
	OLED_Clear();
	 
    while(1)
  {	 

//	 stop_choice();//函数定时执行，LED闪烁，电机停止选择函数。
//		
//	 stop_display_execute(); //电机暂停显示函数
//		
//	 speed_set_display_execute();//速度设置显示和执行函数  
//	    		  
//	 search_display_execute(); //两路红外循迹显示和执行函数
//	  
//	 obstacle_avoidance_display_execute();//红外避障显示和执行函数	
//	  
//	 decode_choice();//红外遥控，蓝牙控制，选择功能函数
	  
	  if (packet_ready)
	  {
            packet_ready = 0;
            
            // 解析字符串到浮点数
            received_angle = atoi(uart_rx_buffer);
		    OLED_ShowCH(0,0,"angle=");   
			OLED_ShowNum(56,0,received_angle,4,1);
		  
			OLED_ShowCH(0,4,"speed=");
			OLED_ShowNum(60,4,speed_variate,5,0); 
			if(sound_pwm>=0)OLED_ShowCH(0,2,"data="),OLED_ShowChar(48,2,'+'),OLED_ShowNum(56,2,sound_pwm,4,1);
			if(sound_pwm<0)OLED_ShowCH(0,2,"data="),OLED_ShowChar(48,2,'-'),OLED_ShowNum(56,2,-sound_pwm,4,1);
		
	  }

   }	
 
 }


	
	

