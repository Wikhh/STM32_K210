
//小车用到了四个减速电机
//电机减速比为1：48
//电机在6V电压下空转速为210转

#include "motor.h"//电机驱动头文件

unsigned int speed_count=0;//占空比计数器 100次一周期
unsigned char move_rec_flag=0;//电机标志位 1 有新数据 0 没有
unsigned char stop_display_flag=0;//暂停显示标志位
unsigned char Time_10us_motor = 0;//10us计数器，作为电机驱动的基本计数器
int left_speed_duty=0;  //左边电机PWM
int right_speed_duty=0;//右边电机PWM
int sound_pwm=0;
//电机驱动IO定义

 
//LEFT_F_PIN	PA7	 
//LEFT_B_PIN	PA6	 
//RIGHT_F_PIN	PA4	 
//RIGHT_B_PIN	PA5  
//GPIO配置函数
void MotorGPIO_Configuration(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LEFT_F_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFT_F_GPIO, &GPIO_InitStructure);    
	
	GPIO_InitStructure.GPIO_Pin = LEFT_Z_PIN;	
	GPIO_Init(LEFT_Z_GPIO, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_F_PIN;	
	GPIO_Init(RIGHT_F_GPIO, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_Z_PIN;	
	GPIO_Init(RIGHT_Z_GPIO, &GPIO_InitStructure); 
	
}

//根据占空比驱动电机转动
void CarMove(void)
{   
	
  //左轮
	if(left_speed_duty > 0)
	{
		if(speed_count < left_speed_duty)
		{
			LEFT_GO;//向前
		}else
		{
			LEFT_STOP;//停止
		}
	}
	else if(left_speed_duty < 0)
	{
		if(speed_count < (-1)*left_speed_duty)
		{
			LEFT_BACK;//向后
		}else
		{
			LEFT_STOP; //停止
		}
	}
	else                
	{
		 LEFT_STOP;//停止
	}
	if(left_speed_duty==1)
	{
		LEFT_BRAKE;//刹车
	}
	
	//右轮
	if(right_speed_duty > 0)
	{
		if(speed_count < right_speed_duty)
		{
			RIGHT_GO;//向前
		}else                
		{
			RIGHT_STOP;//停止
		}
	}
	else if(right_speed_duty < 0)
	{
		if(speed_count < (-1)*right_speed_duty)
		{
			RIGHT_BACK;//向后
		}else               
		{
			RIGHT_STOP; //停止
		}
	}
	else               
	{
		RIGHT_STOP; //停止
	}
	
	if(right_speed_duty==1)//刹车
	{
		RIGHT_BRAKE;//刹车
	}

}
int sound_PID(int angle,int TargetVelocity)
{
    int Bias;
    static int ControlVelocityA, Last_biasA, Last_Last_biasA;

    Bias = TargetVelocity - angle;

    // 增量式PID公式（严格时间离散化）
    float delta = 
        sound_Kp * (Bias - Last_biasA) +           // 比例项
        sound_Ki * Bias  +                     // 积分项
        sound_Kd * (Bias - 2*Last_biasA + Last_Last_biasA) ; // 微分项

    ControlVelocityA += (int)delta;

    // 更新历史误差（保留两拍历史值）
    Last_Last_biasA = Last_biasA;
    Last_biasA = Bias;

    // 抗饱和限幅（冻结积分）
    if (ControlVelocityA > 100) {
        ControlVelocityA = 100;
        Last_biasA = Bias; // 核心修正：冻结积分累积
    } 
    else if (ControlVelocityA < -100) {
        ControlVelocityA = -100;
        Last_biasA = Bias;
    }

    return ControlVelocityA; 
}
//speed说明，这个变量的区间15-100 对应15%-100%的PWM占空比。
//面对小车轮子。sudu为正，轮子顺时针转动。sudu为负，轮子逆时针转动。
//左边的两个电机和右边两个电机的转动方向不一样。

//向前
void CarGo(void)
{
	left_speed_duty=-sudu;
	right_speed_duty=sudu;

}

//后退
void CarBack(void)
{
  left_speed_duty=sudu;
	right_speed_duty=-sudu;

}

//向左 
void CarLeft(void)
{
	left_speed_duty=sudu;
	right_speed_duty=sudu;

}

//向右
void CarRight(void)
{
	left_speed_duty=-sudu;
	right_speed_duty=-sudu;

}

//小车滑行停止
//电机驱动给电机的电压电流会慢慢减小直到为零
//所以小车会滑行一段时间后停止
//此时电机驱动芯片为休眠状态
void CarStop(void) 
{
	left_speed_duty=0;
	right_speed_duty=0;
  
}

//小车刹车
//电机驱动给电机的电压电流会立马为零
//所以刹车后小车会瞬间停止
void Car_Brake(void)
{
	left_speed_duty=1;
	right_speed_duty=1;
	
}

//红外循迹后退
void CarBack_Trailing(void)
{
	left_speed_duty=sudu;
	right_speed_duty=-sudu;

}

//红外循迹向左
void CarLeft_Trailing(void)
{
	left_speed_duty=70;
	right_speed_duty=70;

}

//红外循迹向右
void CarRight_Trailing(void)
{
	left_speed_duty=-70;
	right_speed_duty=-70;

}


//红外避障后退
void CarBack_obstacle_avoidance(void)
{
	left_speed_duty=100;
	right_speed_duty=-100;

}

//红外避障向左
void CarLeft_obstacle_avoidance(void)
{
	left_speed_duty=100;
	right_speed_duty=80;

}

//红外避障向右
void CarRight_obstacle_avoidance(void)
{
	left_speed_duty=-80;
	right_speed_duty=-100;

}

//电机驱动初始化
void motor_init(void)
{
	MotorGPIO_Configuration(); //GPIO配置函数
	
	CarStop(); //小车暂停 

	search_flag=0;
	obstacle_avoidance_flag=0;
	speed_measurement_flag=0; 
	ultrasonic_flag=0;  
	
}

		
 void stop_display_execute()//电机暂停显示执行函数
 {
	 
				if(stop_display_flag==1)//屏幕显示 暂停 标志位 ok键
		{
			          Car_Brake();//小车刹车
				      OLED_Clear();	//OLED屏幕清屏
			          sudu=90;//速度设置为90
					  OLED_ShowCH(48,1,"暂停");		    //显示 暂停 文字
			          OLED_ShowCH(3,6,"设置速度:");	   //显示 设置速度 文字
                      OLED_ShowCH(110,6,"%");		      //显示 % 符号
					  OLED_ShowNum(80,6,sudu,3,0);    //显示 当前设置速度 数值
			          stop_display_flag=0;//屏幕显示 暂停 标志位为零
			          speed_stop=0;//速度测量和总里程测量变量为零
			   	      IrDA=0;		        	         
			          CarStop(); //小车暂停 
		}

 }	


