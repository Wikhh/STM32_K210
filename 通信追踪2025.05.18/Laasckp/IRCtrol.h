
#ifndef __IRCTROL_H_
#define __IRCTROL_H_

#include "interface.h" //IO口定义头文件

extern unsigned char ir_rec_flag;   //接收数据标志位 1 有新数据 0 没有
extern unsigned char continue_time;//停车时间
extern unsigned char IRCOM[4];    //储存红外遥控发射数据

extern char IrDA;//红外值

void IRCtrolInit(void);          //外部中断配置 红外遥控配置
void IRIntIsr(void);            //红外遥控发射数据解码

#endif
