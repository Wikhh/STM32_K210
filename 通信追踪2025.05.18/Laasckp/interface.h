
/******************************************************
  
	淘宝：Laasckp
	咸鱼：Laasckp
  说明：部份源码来源网络--请试用24小时后删除。
			
******************************************************/

#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "stm32f10x.h"                       //STM32F10x系列重要头文件
#include "sys.h"                            //STM32的IO口输入宏定义和输出宏定义
#include "IRCtrol.h"                       //红外遥控头文件
#include "motor.h"                        //电机驱动头文件
#include "oled_iic.h"                    //OLED显示屏驱动头文件
#include "speed_set.h"                  //速度设置头文件
#include "search.h"                    //红外循迹头文件
#include "Obstacle_Avoidance.h"       //红外避障头文件
#include "speed_measurement.h"       //速度测量头文件
#include "Delay.h"                  //延时函数头文件
#include "uart.h"                  //串口头文件
#include "led.h"                  //led头文件
#include "timer.h"               //定时器头文件
#include "steering_engine.h"    //舵机头文件
#include "ultrasonic.h"        //超声波头文件
#include "buzzer.h"           //蜂鸣器头文件
#include "adc.h"             //adc头文件
#include "battery_voltage.h"//电池电压测量头文件


#include "uart2.h"     //接收K210数据所用到的串口
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void GPIO_clock_init(void);//GPIO时钟初始化
void all_init(void);     //所有想要初始化的函数	
void decode_choice(void);//红外，蓝牙，解码选择函数
void stop_choice(void); ///电机停止，led闪烁函数

//定义OLED显示屏
//定义IIC总线连接的GPIO端口, 用户只需要修改下面代码即可任意改变SCL和SDA的引脚
#define OLED_SCL PBout(7)  //SCL引脚
#define OLED_SDA PBout(6)  //SDA引脚

#define OLED_SCL_GPIO_PORT	  GPIOB			/* GPIO端口 */
#define OLED_SCL_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define OLED_SCL_PIN		  GPIO_Pin_7			/* 连接到SCL时钟线的GPIO */

#define OLED_SDA_GPIO_PORT	  GPIOB			/* GPIO端口 */
#define OLED_SDA_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define OLED_SDA_PIN		  GPIO_Pin_6			/* 连接到SDA数据线的GPIO */

// LED PA11 红色
#define LED_PIN_0         GPIO_Pin_5
#define LED_GPIO_0        GPIOA
#define LED_SET_0         GPIO_SetBits(LED_GPIO_0 , LED_PIN_0)
#define LED_RESET_0       GPIO_ResetBits(LED_GPIO_0 , LED_PIN_0)

// LED PA8  绿色
#define LED_PIN_1         GPIO_Pin_8
#define LED_GPIO_1        GPIOA
#define LED_SET_1         GPIO_SetBits(LED_GPIO_1 , LED_PIN_1)
#define LED_RESET_1       GPIO_ResetBits(LED_GPIO_1 , LED_PIN_1)

// LED PB13  蓝色
#define LED_PIN_2         GPIO_Pin_13
#define LED_GPIO_2        GPIOB
#define LED_SET_2         GPIO_SetBits(LED_GPIO_2 , LED_PIN_2)
#define LED_RESET_2       GPIO_ResetBits(LED_GPIO_2 , LED_PIN_2)


//红外遥控 红外接收器数据线,外部中断 PB12
#define IRIN_PIN         GPIO_Pin_12
#define IRIN_GPIO        GPIOB
#define IRIN_PORTSOURCE  GPIO_PortSourceGPIOB
#define IRIN_PINSOURCE   GPIO_PinSource12
#define IRIN_EXITLINE    EXTI_Line12
#define IRIN_IRQCH       EXTI15_10_IRQn
#define IRIN             GPIO_ReadInputDataBit(IRIN_GPIO, IRIN_PIN)


 
//LEFT_F_PIN	PA6	 
//LEFT_Z_PIN	PA7	 

//FRONT_F_PIN	PA1	 
//FRONT_Z_PIN	PA0  
 
#define LEFT_F_PIN           GPIO_Pin_7
#define LEFT_F_GPIO          GPIOA
#define LEFT_F_SET           GPIO_SetBits  (LEFT_F_GPIO , LEFT_F_PIN )
#define LEFT_F_RESET         GPIO_ResetBits(LEFT_F_GPIO , LEFT_F_PIN )

#define LEFT_Z_PIN           GPIO_Pin_6
#define LEFT_Z_GPIO          GPIOA
#define LEFT_Z_SET           GPIO_SetBits  ( LEFT_Z_GPIO , LEFT_Z_PIN)
#define LEFT_Z_RESET         GPIO_ResetBits( LEFT_Z_GPIO , LEFT_Z_PIN)

#define RIGHT_F_PIN          GPIO_Pin_1
#define RIGHT_F_GPIO         GPIOB
#define RIGHT_F_SET          GPIO_SetBits  (RIGHT_F_GPIO , RIGHT_F_PIN)
#define RIGHT_F_RESET        GPIO_ResetBits(RIGHT_F_GPIO , RIGHT_F_PIN)

#define RIGHT_Z_PIN          GPIO_Pin_0
#define RIGHT_Z_GPIO         GPIOB
#define RIGHT_Z_SET          GPIO_SetBits  ( RIGHT_Z_GPIO , RIGHT_Z_PIN)
#define RIGHT_Z_RESET        GPIO_ResetBits( RIGHT_Z_GPIO , RIGHT_Z_PIN)


//左边电机
#define LEFT_GO     LEFT_F_SET;   LEFT_Z_RESET  //前进
#define LEFT_BACK   LEFT_F_RESET; LEFT_Z_SET   //后退
#define LEFT_STOP   LEFT_F_RESET; LEFT_Z_RESET//停止
#define LEFT_BRAKE  LEFT_F_SET;   LEFT_Z_SET //刹车

//右边电机
#define RIGHT_GO    RIGHT_F_SET;  RIGHT_Z_RESET   //前进
#define RIGHT_BACK  RIGHT_F_RESET;RIGHT_Z_SET   //后退
#define RIGHT_STOP  RIGHT_F_RESET;RIGHT_Z_RESET//停止
#define RIGHT_BRAKE RIGHT_F_SET;  RIGHT_Z_SET //刹车


//红外循迹
/*

左循迹	SEARCH_L_PIN	PA1
右循迹	SEARCH_R_PIN	PA0

 */
 
#define SEARCH_L_PIN         GPIO_Pin_1
#define SEARCH_L_GPIO        GPIOA
#define SEARCH_L_IO          GPIO_ReadInputDataBit(SEARCH_L_GPIO, SEARCH_L_PIN)

#define SEARCH_R_PIN         GPIO_Pin_0
#define SEARCH_R_GPIO        GPIOA
#define SEARCH_R_IO          GPIO_ReadInputDataBit(SEARCH_R_GPIO, SEARCH_R_PIN)

#define BLACK_AREA 1        //检测到黑线
#define WHITE_AREA 0        //没有检测到黑线


//红外避障
/* 
左避障	VOID_L_PIN	PB14
右避障	VOID_R_PIN	PB15
 */
 
#define VOID_L_PIN         GPIO_Pin_14
#define VOID_L_GPIO        GPIOB
#define VOID_L_IO          GPIO_ReadInputDataBit(VOID_L_GPIO, VOID_L_PIN)

#define VOID_R_PIN         GPIO_Pin_15
#define VOID_R_GPIO        GPIOB
#define VOID_R_IO          GPIO_ReadInputDataBit(VOID_R_GPIO, VOID_R_PIN)


#define BARRIER_Y 0  //有障碍物
#define BARRIER_N 1  //无障碍物


//测速
/* 
速度码盘	coded_disc_PIN	PC13
 */
#define coded_disc_PIN         GPIO_Pin_13
#define coded_disc_GPIO        GPIOC
#define coded_disc_IO          GPIO_ReadInputDataBit(coded_disc_GPIO, coded_disc_PIN)

//超声波控制 外部中断返回 PB9
#define Echo_PIN         GPIO_Pin_9
#define Echo_GPIO        GPIOB
#define Echo_PORTSOURCE  GPIO_PortSourceGPIOB
#define Echo_PINSOURCE   GPIO_PinSource9
#define Echo_EXITLINE    EXTI_Line9
#define Echo_IRQCH       EXTI9_5_IRQn
#define Echo             GPIO_ReadInputDataBit(Echo_GPIO, Echo_PIN)

//触发IO PB8
#define Trig_PIN         GPIO_Pin_8
#define Trig_GPIO        GPIOB
#define Trig_RESET       GPIO_SetBits(Trig_GPIO , Trig_PIN)
#define Trig_SET         GPIO_ResetBits(Trig_GPIO , Trig_PIN)//做了一次反相 

//舵机控制IO PB5
#define Servo_PIN         GPIO_Pin_5
#define Servo_GPIO        GPIOB
#define Servo_SET         GPIO_SetBits(Servo_GPIO , Servo_PIN)
#define Servo_RESET       GPIO_ResetBits(Servo_GPIO , Servo_PIN)

//蜂鸣器 PA12
#define BUZZER_PIN        GPIO_Pin_12
#define BUZZER_GPIO       GPIOA
#define BUZZER_SET        GPIO_SetBits(BUZZER_GPIO , BUZZER_PIN)
#define BUZZER_RESET      GPIO_ResetBits(BUZZER_GPIO , BUZZER_PIN)

#define VOLTAGE_ADC1_CHx	ADC_Channel_4	//定义电压采集所在的ADC1通道编号

//运动指令定义
#define Car_Go    0x18//前进 上箭头键
#define Car_Back  0x52//后退 下箭头键
#define Car_Left  0x08//左转 左箭头键
#define Car_Right 0x5A//后退 右箭头键
#define Car_Stop  0x1C//暂停 ok键	

#define Car_Obstacle_Avoidance  0x45 //两路红外避障        1键
#define Car_Search              0x46 //两路红外循迹        2键
#define Car_Speed_Measurement   0x47 //小车测速            3键
#define Car_Ultrasonic          0x44 //超声波舵机旋转避障  4键
#define Car_Buzzer              0x40 //蜂鸣器              5键	
#define Car_Battery_Voltage     0x43 //电池电压测量        6键
#define Car_Speed_And           0x16 //速度+               *键	
#define Car_Speed_Subtract      0x0D //速度-               #键
				
#endif

