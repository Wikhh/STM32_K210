
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __ULTRASONIC_H_
#define __ULTRASONIC_H_
#include "interface.h"

extern unsigned char ultrasonic_flag;          //超声波舵机避障执行标志位
extern unsigned char ultrasonic_display_flag; //超声波舵机避障显示标志位

extern volatile unsigned int distance_cm;//当前距离

extern unsigned int dis_left;//左边距离
extern unsigned int dis_right;//右边距离
extern unsigned int dis_direct;//前方距离

void UltraSoundInit(void);
void GetDistanceDelay(void);
void Distance(void);
void BarrierProc(void);
void GetAllDistance(unsigned int *dis_left,unsigned int *dis_right,unsigned int *dis_direct);
void Time4Init(void);
void ultrasonic_init(void);                 //超声波初始化函数
void ultrasonic_execute(void);            //超声波舵机避障执行函数
void ultrasonic_display_execute(void);   //超声波舵机避障显示和执行函数

#endif
