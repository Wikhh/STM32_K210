

#ifndef __SPEED_MEASUREMENT_H_
#define __SPEED_MEASUREMENT_H_

#include "interface.h"//IO口定义头文件 很重要

#define FOSC 48000000L //晶振设置，使用24M Hz 6T双速倍模式


extern unsigned char speed_measurement_flag;     //速度测量标志位
extern unsigned char speed_measurement_display_flag; //速度测量显示标志位
extern unsigned char total_distance_flag;//总里程测量标志位
extern unsigned char total_distance_display_flag;//总里程测量显示标志位
extern unsigned char speed_stop;//测速，总里程测量开始暂停变量

extern unsigned int  count_1ms;    //测量时间
extern unsigned int  speed_variate;//小车速度计算变量
extern unsigned int  speed_io;  //IO变化次数
extern unsigned int  speed_10us;        //10us计算，作为设置速度的基本计算器
extern unsigned int  speed_io_contrast;//IO电平变化比较
extern unsigned int  total_distance_variate;//总里程测量变量

void measure_init(void);//速度引脚初始化
void measure_speed(void);//函数功能 ：速度测量，计算IO变化次数 速度测量
void speed_measurement_display_execute(void);//速度测量显示和执行函数
void total_distance(void);//总里程测量函数
void total_distance_display_execute(void);//总里程测量显示和执行函数

#endif

