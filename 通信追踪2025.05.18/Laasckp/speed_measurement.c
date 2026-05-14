#include "speed_measurement.h"//速度测量头文件

//轮子直径66mm，光电码盘齿数为20，轮子周长 207mm = 20.7cm 
//程序采用判断高低电平变化次数计数，也就是说轮子转一周计数次数为40
//一个计数变化表示轮子跑过的距离为 20.7/40 = 0.5175cm

unsigned int count_1ms=0;//测量时间
unsigned int speed_io=0;//IO变化次数
unsigned int speed_10us = 0;//10us计算，作为设置速度的基本计算器
static unsigned int speed_io_contrast=0;//IO电平变化比较

unsigned int  speed_variate=0;//速度计算变量
unsigned char speed_measurement_flag=0;          //速度测量标志位
unsigned char speed_measurement_display_flag=0; //速度测量显示标志位

unsigned int  total_distance_variate=0;//总里程测量变量
unsigned char total_distance_flag=0;//总里程测量标志位
unsigned char total_distance_display_flag=0;//总里程测量显示标志位

unsigned char speed_stop=0;//测速，总里程测量开始暂停变量

void measure_init(void)//速度测量引脚初始化
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = coded_disc_PIN ;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_Init(coded_disc_GPIO , &GPIO_InitStructure); 
		
}

/*******************************************************************************
* 函 数 名 ：measure_speed
* 函数功能 ：速度测量，计算IO变化次数
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void measure_speed(void)
{
	
	count_1ms++;
		
	if(coded_disc_IO != speed_io_contrast)//发生电平变化
	{
		speed_io++;  //电平变化次数加一
		speed_io_contrast = coded_disc_IO;
	}
	
	if(count_1ms == 500)//500ms
	{
		count_1ms = 0;
		
		speed_variate = speed_io*2;//1sIO口变化的次数 
		
		speed_io = 0;
				
		speed_variate = (unsigned char)(0.5175 * (double)speed_variate + 0.1);//计算速度 cm/s 四舍五入
		total_distance_variate=total_distance_variate+speed_variate/2;
	}
}


		void speed_measurement_display_execute(void)//速度测量显示和执行函数
			
    {
		
			if(speed_measurement_display_flag==1)//速度测量显示标志位
		{
			          Car_Brake();//小车刹车	
			         
				        OLED_Clear();//OLED屏幕清屏
                sudu=30;//速度30
			          speed_variate=0;//显示当前 真实速度 为零
			          OLED_ShowCH(3,0,"速度:");//显示 速度 文字
                OLED_ShowCH(90,0,"cm/s");//显示cm/s 米每秒
                OLED_ShowNum(60,0,speed_variate,3,0); //显示当前 真实速度 数值
			          OLED_ShowCH(3,6,"设置速度:");//显示 设置速度 文字
                OLED_ShowCH(110,6,"%");//显示 % 符号
                OLED_ShowNum(80,6,sudu,3,0); //显示 当前设置速度 数值
			          speed_measurement_display_flag=0; //速度测量显示标志位置0
			          speed_stop++;
			         
			          IrDA=0; //红外码值清零，防止干扰  
			          CarStop();//小车暂停
		}
			if(speed_stop==1)
			{
				
			 	if(  speed_measurement_flag==1)//速度测量标志位
							
			{
				
				 OLED_ShowNum(60,0,speed_variate,3,0);   //显示当前 真实速度 数值
				
			   CarGo();//小车前进
				
			}		
				
		}
			
			if(speed_stop==2)
			{	
									    
				sudu=80;//速度80
				OLED_ShowNum(80,6,sudu,3,0); //显示 当前设置速度 数值
			  OLED_ShowNum(58,3,total_distance_variate,5,0);    //显示当前 总里程 数值
			  speed_measurement_flag=0;//速度测量标志位为零
				total_distance_flag=0;//总里程测量显示标志位为零				
				speed_stop=0;//测速，总里程测量开始暂停变量为零
				CarStop();//小车暂停
				
			}
									
    }

   

 
void total_distance(void)
{
	
	 OLED_ShowNum(58,3,total_distance_variate,5,0);    //显示当前 总里程 数值
	
}


		void total_distance_display_execute(void)//总里程测量显示和执行函数
			
    {
												
			 	if(total_distance_display_flag==1)//总里程测量显示标志位
							
			{			                    	         
			          OLED_ShowCH(0,3,"总里程:");	  //显示 总里程 文字
                OLED_ShowCH(100,3,"cm");		     //显示 % 符号               
			          total_distance_display_flag=0;//总里程测量显示标志位清零
			          IrDA=0; //红外码值清零，防止干扰						
			}		
		
			 	if(total_distance_flag==1)//总里程测量显示标志位
							
			{
				        total_distance();				
				
			}		
								
		
    }

  		
			

