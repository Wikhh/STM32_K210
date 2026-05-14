
#include "ultrasonic.h"

volatile unsigned int distance_cm = 0;//当前距离

static unsigned int ultick_5ms = 0;//5ms计数器

volatile unsigned char status = 0;//程序当前状态，0,空闲 1 发送触发信号，2 等待信号返回,3 

unsigned char ultrasonic_flag= 0;          //超声波舵机避障执行标志位
unsigned char ultrasonic_display_flag= 0; //超声波舵机避障显示标志位

unsigned int dis_count = 0;//脉宽长计时
unsigned int dis_left;//左边距离
unsigned int dis_right;//右边距离
unsigned int dis_direct;//前方距离


void Time4Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

//外部中断配置 超声波,这里没用到中断计数方式
void ultrasonic_init(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//定义一个外部中断相关的结构体
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(Echo_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);

	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//将GPIO口配置成外部中断 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //阶级为0，不可嵌套
	NVIC_InitStructure.NVIC_IRQChannel = Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//主优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//子优先级，最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能该模块中断
	NVIC_Init(&NVIC_InitStructure);	//中断初始化，将结构体定义的数据执行
	
	Time4Init();
	Trig_RESET;
}

void Distance(void)
{
	ultick_5ms++;
	if(ultick_5ms >= 15)
	{
		ultick_5ms = 0;
		GetDistanceDelay();
	}
}

//延时的方式读取距离值
void GetDistanceDelay(void)
{
		//发送触发信号
		Trig_SET;
		status = 1;
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);//打开定时器
		while(TIM_GetCounter(TIM4) < 11);//延时超过10us
		status = 2;
		Trig_RESET;
		TIM_SetCounter(TIM4, 0);
	
		while(Echo == 0)//等待回向信号起始位置
		{
			if(TIM_GetCounter(TIM4) > 60000)//回应时间超出正常范围 
			{
				status = 0;
				distance_cm = 0;//失败后就后退
				TIM_Cmd(TIM4, DISABLE);//关闭定时器
				return;//本次失败
			}
		}
		TIM_SetCounter(TIM4, 0);
		while(Echo == 1)//开始计算长度
		{
			int count = TIM_GetCounter(TIM4);
			if(count > 60000)//回应时间超出正常范围 
			{
				status = 0;
				distance_cm = 0;//失败后就后退
				TIM_Cmd(TIM4, DISABLE);//关闭定时器
				return;//本次失败
			}
		}
		dis_count = TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4, DISABLE);//关闭定时器
		distance_cm = (unsigned int)(((long)(dis_count) * 34)/2000);//声速340m/s
		status = 0;//准备下次发送	
}
///获取三个方向的距离,进来前舵机方向为向前
void GetAllDistance(unsigned int *dis_left,unsigned int *dis_right,unsigned int *dis_direct)
{

	Car_Brake();	//电机刹车暂停
	DuojiRight();
	delay_ms(50);
	GetDistanceDelay();//获取右边距离
	*dis_right = distance_cm;
	
	DuojiMid();
	DuojiLeft();
	delay_ms(50);
	GetDistanceDelay();//获取左边距离
	*dis_left = distance_cm;
	
	DuojiMid();//归位
	GetDistanceDelay();
	*dis_direct = distance_cm;
}
void BarrierProc()
{
	

      

    	if(distance_cm < 26)//前方障碍物距离小于26厘米
	{
		
			  GetDistanceDelay();
		    delay_ms(10);
		    GetDistanceDelay();
	 	if(distance_cm < 26)//前方有障碍物
	{
			

	    GetAllDistance(&dis_left,&dis_right,&dis_direct);
		
      if(dis_direct<26)
	  {
		

    if(dis_direct<8)
			{
				CarBack();
				delay_ms(500);
				
			}
		  
			 if(dis_left<dis_right)//右转
			{
				
				CarRight_Trailing();
				delay_ms(300);
				
			}
			 if(dis_left>dis_right)
			{
				CarLeft_Trailing();
				delay_ms(300);
				
			}
			 if((dis_left<8)||(dis_right<8))
			{
				 CarBack();
				 delay_ms(500);
				 
			}
										
	}
}
	}
	else
		
	{
		if(distance_cm > 26)
		{
		CarGo();
		}
		
			}
}
void ultrasonic_execute()//超声波舵机避障执行函数
{
	      Distance();
			  BarrierProc();
	
}
 void ultrasonic_display_execute()//超声波舵机避障显示函数
  {
	
				if(ultrasonic_display_flag==1)//超声波舵机避障显示标志位
		{
			          
			          sudu=28;        //超声波舵机避障默认速度为28
				        OLED_Clear();	//OLED屏幕清屏
                OLED_ShowCH(2,1,"超声波舵机避障");		//显示 超声波舵机避障 文字
			          OLED_ShowCH(3,6,"设置速度:");	   //显示 设置速度 文字
                OLED_ShowCH(110,6,"%");		      //显示 % 符号
                OLED_ShowNum(80,6,sudu,3,0);    //显示 当前设置速度 数值
			          ultrasonic_display_flag=0;//超声波舵机避障显示标志位为0
			          speed_stop=0;//速度测量和总里程测量变量为零
			          IrDA=0; //红外码值清零，防止干扰  
			      
		}
		
		
		
	     if(ultrasonic_flag==1)////超声波舵机避障执行标志位
		
		{
			
		  ultrasonic_execute();//超声波舵机避障执行函数
		
			
		}
		
	
	
	

		
}


