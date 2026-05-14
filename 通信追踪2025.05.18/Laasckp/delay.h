
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __Delay_H_
#define __Delay_H_

#include "interface.h"//IO口定义头文件 很重要

void delay_init(void);    // 延时函数初始化             
void delay_ms(u32 Nms);   //毫秒延时函数
void delay_us(u32 Nus); //微秒延时函数

#endif

