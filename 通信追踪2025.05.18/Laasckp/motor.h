

#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "interface.h"//IO口定义头文件 很重要

extern int left_speed_duty;     //左边电机PWM
extern int right_speed_duty;   //右边电机PWM
extern unsigned char move_rec_flag;  //电机驱动标志位
extern unsigned char stop_display_flag;  //暂停显示标志位
extern unsigned char Time_10us_motor; //10us计数器，作为电机驱动的基本计数器
extern unsigned int speed_count;        //占空比计数器 100次一周期
extern float sound_Kp,sound_Ki,sound_Kd;
extern int sound_pwm;
void CarMove(void);               //电机驱动函数
void CarGo(void);                //车前进
void CarBack(void);             //车后退
void CarLeft(void);            //车左转
void CarRight(void);          //车右转
void CarStop(void);          //车滑行暂停 电机驱动芯片为休眠状态。
void Car_Brake(void);       //车刹车暂停
void motor_init(void);     //电机驱动初始化
void CarBack_Trailing(void);                //电机循迹后退
void CarLeft_Trailing(void);                //电机循迹左转
void CarRight_Trailing(void);               //电机循迹后退
void CarBack_obstacle_avoidance(void);      //电机避障后退
void CarLeft_obstacle_avoidance(void);      //电机避障左转
void CarRight_obstacle_avoidance(void);    //电机避障右转
void stop_display_execute(void);           //电机暂停显示执行函数
int sound_PID(int angle,int TargetVelocity);
#endif

