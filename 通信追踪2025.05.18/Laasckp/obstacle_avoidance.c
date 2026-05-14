#include "obstacle_avoidance.h"                     //红外避障头文件

unsigned char obstacle_avoidance_flag=0;             //红外避障检测标志位
unsigned char obstacle_avoidance_display_flag=0;    //红外避障检测显示标志位

//红外避障初始化
void obstacle_avoidance_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
			
	GPIO_InitStructure.GPIO_Pin = VOID_R_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_Init(VOID_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = VOID_L_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉	
	GPIO_Init(VOID_L_GPIO , &GPIO_InitStructure); 

}

//obstacle_avoidance_detector(void)避障检测函数说明
//红外避障，通过判断两个避障模块的状态来控制小车运动
//          两路没有检测到 前进
//			左边检测到   右转
//			右边检测到   左转
//			两边检测到   后退

void obstacle_avoidance_detector(void)//红外避障检测函数
{
	
	 if(VOID_L_IO == BARRIER_N && VOID_R_IO == BARRIER_N)//两路没有检测到
	 {		
		CarGo();   //小车前进		 
	 }
	 	
	
	 if(VOID_L_IO == BARRIER_Y&&VOID_R_IO== BARRIER_N)//左边检测到
	{
					
		CarRight_obstacle_avoidance();  //小车右转
		delay_ms(100);		           //延时100毫秒
	}
	
	
	if(VOID_R_IO == BARRIER_Y&&VOID_L_IO == BARRIER_N)//右边检测到
	{
			
		 CarLeft_obstacle_avoidance(); //小车左转
		 delay_ms(100);		          //延时100毫秒
	}
	
	
	if(VOID_R_IO == BARRIER_Y&&VOID_L_IO == BARRIER_Y)// 两路都检测到
	{ 
				
		CarBack_obstacle_avoidance();  //小车后退
		delay_ms(50);				  //延时50毫秒
	}
	
}


   void obstacle_avoidance_display_execute(void)//红外避障显示和执行函数
  {
	
				if(obstacle_avoidance_display_flag==1)//避障屏幕显示标志位
		{
			          
			       
			          sudu=25;        //避障默认速度为25
				        OLED_Clear();	//OLED屏幕清屏
                OLED_ShowCH(28,1,"红外避障");		//显示 红外避障文字
			          OLED_ShowCH(3,6,"设置速度:");	   //显示 设置速度 文字
                OLED_ShowCH(110,6,"%");		      //显示 % 符号
                OLED_ShowNum(80,6,sudu,3,0);    //显示 当前设置速度 数值
			          obstacle_avoidance_display_flag=0;			        
			          IrDA=0; //红外码值清零，防止干扰  
		}
		
		
		
	if(obstacle_avoidance_flag==1)//遥控按键1
		
		{
			
		  
		  obstacle_avoidance_detector();//红外避障检测函数
			
		}
		
	
}
  



	
		



