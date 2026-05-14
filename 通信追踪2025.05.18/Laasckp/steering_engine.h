
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __STEERING_ENGINE_H_
#define __STEERING_ENGINE_H_

#include "interface.h"//IO口定义头文件 很重要
void ServoInit(void);
void DuojiMid(void);
void DuojiRight(void);
void DuojiLeft(void);
extern unsigned int duoji_count;//速度加变量
extern unsigned int zhuanjiao;//速度减变量
#endif
