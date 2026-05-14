
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __LED_H_
#define __LED_H_

#include "interface.h"//IO口定义头文件 很重要

extern unsigned char tick_200ms ;        //LED闪烁时间

void led_init(void);//led灯的初始化
void led_colour_red(uint16_t led_red);     //LED红色驱动函数
void led_colour_green(uint16_t led_green);//LED绿色驱动函数
void led_colour_blue(uint16_t led_blue);//LED蓝色驱动函数



#endif
