
#include "interface.h"//模块接口定义头文件


void GPIO_clock_init(void)//初始化GPIOA B C时钟
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	
}


//初始化所有想要的函数	
void all_init(void)
{
	
	 delay_init();                                     //延时函数初始化函数
	 GPIO_clock_init();                               //GPIOA B C时钟初始化函数
	 led_init();                                     //LED初始化函数
	 IRCtrolInit();                                 //外部中断配置和遥控配置函数
	 TIM2_Init();                                  //定时器2初始化函数
	 motor_init();                                //电机驱动初始化函数
     OLED_Init();	                               //OLED屏幕初始化函数
	 OLED_Clear();	                            //OLED屏幕清屏函数
     search_init();                            //红外循迹初始化函数
	 obstacle_avoidance_init();               //红外避障初始化函数
	 measure_init();                         //真实速度测量初始化函数
   USART3Conf(9600);                      //串口3初始化函数	
   OLED_ShowCH(15,1,"遥控选择模式");     //显示 遥控选择模式 文字
   OLED_ShowCH(3,6,"设置速度:");	      //设置 速度 文字
   OLED_ShowCH(110,6,"%");		         //显示 % 符号
   OLED_ShowNum(80,6,sudu,3,0);       //显示 默认设置速度 数值
	 ServoInit();                      //舵机初始化函数
	 ultrasonic_init();               //超声波初始化函数
	 DuojiMid();                     //舵机居中函数
     buzzer_init();                 //蜂鸣器初始化函数
	 voltage_init();               //电池电压引脚采集引脚初始化
	 
	 adc1_init();//ADC1初始化函数  
	 
	 Serial_Init();//接收K210数据串口2初始化
	 
	 
}
	  //电机停止，led闪烁函数
 void stop_choice(void)
 {
	    if(Time_1ms >= 5)  // 5ms的周期
	{
			Time_1ms = 0;
		  Time_5ms++;
			tick_200ms++;
			if(tick_200ms >= 40)//5x40=200ms
			{
				tick_200ms = 0;
				
      //选择想要闪烁的颜色			
       //led_colour_red(LED_PIN_0);   //LED红色闪烁
       //led_colour_green(LED_PIN_1);//LED绿色闪烁
       led_colour_blue(LED_PIN_2);//LED蓝色闪烁
				
			}		
			 ultrasonic_display_execute();//超声波舵机避障显示函数 
			
			if(IrDA==0x18||IrDA==0x52||IrDA==0x5A||IrDA==0x08) // 遥控 前进 后退  左转 右转 才有效
			{
			   continue_time--;// 5x150=750ms无接收指令就停车
				
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();   //小车暂停 
				IrDA=0;
			}	
			
		   }
		
      
	}
	 	if(Time_5ms>30)//5x30=150ms的周期
	 {
		 Time_5ms = 0;
		 speed_measurement_display_execute();//速度测量显示和执行函数     
		 buzzer_display_execute();//蜂鸣器显示和执行函数
		 total_distance_display_execute();//总里程测量显示和执行函数
		 battery_voltage_display_execute();//电池电压显示和执行函数
		 
	 }
      	
	
}

  void decode_choice(void)//红外遥控，蓝牙控制，选择功能函数
	  
   {
		 
	    if(ir_rec_flag == 1|BT_flag == 1 )//接收到红外信号,接收到蓝牙信号
				
			  {
					
				  if(ir_rec_flag == 1) //接收到红外信号
				  {	
					 
				          IRIntIsr();       //红外遥控发射数据解码
                  ir_rec_flag = 0;	//红外遥控标志位置零			 
			  			  
				  }
				  				 
                  BT_flag = 0;//蓝牙标志位置零			  	 
        
				 
			   switch(IrDA)  //红外遥控值或者蓝牙值选择执行对应的操作
				   
				{
					case 0x18: CarGo();  
 					           break;//小车前进 上箭头键 
					
					case 0x52: CarBack();  
					           break;//小车后退 下箭头键
					
					case 0x08: CarLeft();  
					           break;//小车左转 左箭头键
					
					case 0x5A: CarRight();  
					           break;//小车右转 右箭头键
					
					case 0x1C: CarStop(); stop_display_flag=1;obstacle_avoidance_flag=0;search_flag=0;
				             speed_measurement_flag=0; ultrasonic_flag=0; buzzer_flag=0;
					           BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;
					           break;//小车暂停 ok键	
					
					case 0x45: obstacle_avoidance_flag=1;obstacle_avoidance_display_flag=1;search_flag=0;
					           speed_measurement_flag=0;ultrasonic_flag=0; buzzer_flag=0;
					           BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;
					           break; //两路红外避障 1键
					
					case 0x46: search_flag=1; search_display_flag=1;obstacle_avoidance_flag=0;
					           speed_measurement_flag=0;ultrasonic_flag=0; buzzer_flag=0;
										 BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;
										 break;//两路红外循迹 2键
					
					case 0x47: speed_measurement_flag=1; speed_measurement_display_flag=1;
					           total_distance_flag=1;total_distance_display_flag=1;
                		 search_flag=0; obstacle_avoidance_flag=0;
					           ultrasonic_flag=0; buzzer_flag=0;
										 BUZZER_RESET;battery_voltage_flag=0;
										 break;//小车测速 3键
													
					case 0x44: ultrasonic_flag=1;ultrasonic_display_flag=1;search_flag=0; 
					           obstacle_avoidance_flag=0; speed_measurement_flag=0; buzzer_flag=0;
										 BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;
										 break;//超声波舵机旋转避障 4键	

          case 0x40: buzzer_flag=1;buzzer_display_flag=1;ultrasonic_flag=0;search_flag=0; 
					           obstacle_avoidance_flag=0; speed_measurement_flag=0;total_distance_flag=0;
 										 battery_voltage_flag=0;
										 break;//蜂鸣器 5键
												 
					case 0x43: battery_voltage_flag=1;battery_voltage_display_flag=1; buzzer_flag=0;
					           ultrasonic_flag=0;search_flag=0;obstacle_avoidance_flag=0; 
										 speed_measurement_flag=0;total_distance_flag=0; BUZZER_RESET;
										 break;//电池电压测量6键							 
					
					case 0x16: speed_and_flag=1;       
					           break;//设置速度加并显示当前设置速度 *键
					
					case 0x0D: speed_subtract_flag=1; 
					           break;//设置速度减并显示当前设置速度 #键
																						
					default :  break;
					
				}
							 				
			}
	
    }

	
