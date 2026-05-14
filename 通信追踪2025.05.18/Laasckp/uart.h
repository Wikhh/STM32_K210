
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __UART_H_
#define __UART_H_


#include "interface.h"//IO口定义头文件 很重要

extern char BT ;//蓝牙控制指令
extern unsigned char BT_flag;//蓝牙控制标志位

void USART3Conf(u32 baudRate);
void PutChar(u8 Data);
void PutNChar(u8 *buf , u16 size);
void PutStr(char *str);

#endif
