#include "search.h"    //红外循迹头文件

unsigned char search_flag=0;          //红外循迹检测标志位
unsigned char search_display_flag=0; //红外循迹检测显示标志位

  //红外循迹初始化
 void search_init(void)
 {
	 
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = SEARCH_R_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_Init(SEARCH_R_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SEARCH_L_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_Init(SEARCH_L_GPIO , &GPIO_InitStructure); 
	
 }

//search_detector()红外循迹检测函数说明
//通过判断两个光电对管的状态来控制小车运动
//左边检测到   左转
//右边检测到   右转
//两边检测到   后退
//没检测到     直行

void search_execute()  //红外循迹函数
{
	
	 if(SEARCH_L_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)//两路没有检测到
	 {
		
		CarGo();   //小车前进
	 }
	 
	
	
		
	 if(SEARCH_L_IO == BLACK_AREA&&SEARCH_R_IO == WHITE_AREA)//左边检测到
	{
		CarLeft_Trailing();  //小车左转
		delay_ms(10);		          //延时10毫秒
	}
	
	
	 if (SEARCH_R_IO == BLACK_AREA&&SEARCH_L_IO == WHITE_AREA)//右边检测到
		 
	{
		CarRight_Trailing(); //小车右转
		delay_ms(100);		          //延时10毫秒
	}
	
	
	
	 if (SEARCH_R_IO == BLACK_AREA&&SEARCH_L_IO == BLACK_AREA)// 两路都检测到
	{
		Car_Brake();  //小车刹车暂停
	}


	}
	
	void search_display_execute()  //红外循迹显示和执行函数
 {
		
				if(search_display_flag==1) //红外循迹检测显示标志位置1
		{
			
			     sudu=25;                             //设置红外循迹速度为25
				   OLED_Clear();	                     //OLED屏幕清屏
           OLED_ShowCH(28,1,"红外寻迹");	    //显示 红外寻迹 文字
			     OLED_ShowCH(3,6,"设置速度:");	   //显示 设置速度 文字
           OLED_ShowCH(110,6,"%");		      //显示 % 符号
           OLED_ShowNum(80,6,sudu,3,0);    //显示 当前设置速度 数值
			     search_display_flag=0;       //红外循迹检测显示执行标志位置0
			     
			      
		}
				
		
		       if(search_flag==1)//遥控按键2  红外循迹检测执行标志位置1
		{			
		         IrDA=0;               //红外码值清零，防止干扰
             search_execute(); //红外循迹函数
		 			
		}		
	
		
	
 }





