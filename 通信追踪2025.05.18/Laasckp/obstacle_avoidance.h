
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __OBSTACLE_AVOIDANCE_H_
#define __OBSTACLE_AVOIDANCE_H_

#include "interface.h"//IO口定义头文件 很重要

extern unsigned char obstacle_avoidance_flag;          //红外避障执行标志位
extern unsigned char obstacle_avoidance_display_flag; //红外避障显示标志位

void obstacle_avoidance_detector(void);             //红外避障检测函数
void obstacle_avoidance_init(void);                //红外避障初始化
void obstacle_avoidance_display_execute(void);   //红外避障显示和执行函数

#endif
