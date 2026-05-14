#include "steering_engine.h"

unsigned int duoji_count=0;
unsigned int zhuanjiao = 150;//默认舵机角度90度

void ServoInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = Servo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//配置GPIO端口速度
	GPIO_Init(Servo_GPIO , &GPIO_InitStructure);
	
	Servo_SET;//默认给高电位			 
}

//定时器2周期是0.01ms，舵机PWM周期是20ms
//舵机角度 0-180度 分别对应 0.5-2.5ms 的脉宽
//变量zhuanjiao可调范围为 50-250 对应舵机 0-180度
//右转时zhuanjiao=50， 对应舵机角度0°
//中间时zhuanjiao=150，对应舵机角度90°
//左转时zhuanjiao=250，对应舵机角度180°
void DuojiMid()//舵机居中	
{
	zhuanjiao = 150;
	delay_ms(300);//延时300毫秒
}

void DuojiRight()//舵机右转	
{
	zhuanjiao = 50;
	delay_ms(300);//延时300毫秒
}

void DuojiLeft()//舵机左转	
{	
	zhuanjiao = 250;
	delay_ms(300);//延时300毫秒	
}
