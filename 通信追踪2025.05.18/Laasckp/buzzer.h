
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __BUZZER_H_
#define __BUZZER_H_


#include "interface.h"//模块接口定义头文件

extern unsigned char buzzer_flag;             //蜂鸣器执行标志位
extern unsigned char buzzer_display_flag;    //蜂鸣器显示标志位

void buzzer_init(void);                   //蜂鸣器初始化
void buzzer_display_execute(void);       //蜂鸣器显示执行函数
void buzzer_execute(uint16_t Buzzer);   //蜂鸣器执行函数

#endif



