#include "buzzer.h"    //蜂鸣器头文件

 unsigned char buzzer_flag=0;             //蜂鸣器执行标志位
 unsigned char buzzer_display_flag=0;    // 蜂鸣器显示标志位 
	
  //蜂鸣器初始化
 void buzzer_init(void)
 {
    GPIO_InitTypeDef  GPIO_InitStructure;
	 
	  GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;//配置使能GPIO管脚
	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	  GPIO_Init( BUZZER_GPIO , &GPIO_InitStructure); 
	  BUZZER_RESET; 
	 
} 


void buzzer_execute(uint16_t Buzzer) //蜂鸣器执行函数
{
			
   BUZZER_GPIO->ODR ^=Buzzer;//指定管脚输出异或 ，实现对应的蜂鸣器状态取反的目的 。
	    
}


  
	void buzzer_display_execute()  //蜂鸣器显示和执行函数
 {
		
				if(buzzer_display_flag==1) //蜂鸣器检测显示标志位置1
		{
			
			     CarStop();
			     sudu=100;                           //设置小车速度为100
				   OLED_Clear();	                    //OLED屏幕清屏
           OLED_ShowCH(40,1,"蜂鸣器");	     //显示 蜂鸣器 文字
			     OLED_ShowCH(3,6,"设置速度:");	  //显示 设置速度 文字
           OLED_ShowCH(110,6,"%");		     //显示 % 符号
           OLED_ShowNum(80,6,sudu,3,0);   //显示 当前设置速度 数值
			     buzzer_display_flag=0;        //蜂鸣器显示执行标志位置0
			     speed_stop=0;//速度测量和总里程测量变量为零
			     IrDA=0;                      //红外码值清零，防止干扰
			
		}
				
		       if(buzzer_flag==1)//遥控按键5 蜂鸣器执行标志位置1
		{			
		       
             buzzer_execute(BUZZER_PIN); //蜂鸣器执行函数
		 		
		}		

 }
