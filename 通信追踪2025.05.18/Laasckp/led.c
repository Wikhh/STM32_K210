#include "led.h"//led头文件

unsigned char tick_200ms = 0;//LED闪烁时间

//led灯初始化
 void led_init(void)
 {
    GPIO_InitTypeDef  GPIO_InitStructure;
	 		 
	GPIO_InitStructure.GPIO_Pin = LED_PIN_0;          //配置使能GPIO管脚  PA11 红色
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(LED_GPIO_0 , &GPIO_InitStructure); 
	LED_SET_0;//默认给高电平 
	 
	GPIO_InitStructure.GPIO_Pin = LED_PIN_1;//配置使能GPIO管脚  PA8  绿色
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口输出速度
	GPIO_Init(LED_GPIO_1 , &GPIO_InitStructure); 
	LED_SET_1;//默认给高电平
	 
  GPIO_InitStructure.GPIO_Pin = LED_PIN_2;//配置使能GPIO管脚  PB13  蓝色
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(LED_GPIO_2 , &GPIO_InitStructure); 
	LED_SET_2;//默认给高电平
	 
	 

	   
 }


void led_colour_red(uint16_t led_red)//led红色驱动函数
{
	 
	
    /* 指定管脚输出异或 ，实现对应的LED指示灯状态取反目的 */
		  	  
	  LED_GPIO_0->ODR ^=led_red;	
	
}
void led_colour_green(uint16_t led_green)//LED绿色驱动函数
{
	 
	
    /* 指定管脚输出异或 ，实现对应的LED指示灯状态取反目的 */
		  	  	 
	   LED_GPIO_1->ODR ^=led_green;
	
	
}
void led_colour_blue(uint16_t led_blue)//LED蓝色驱动函数
{
	 
	
    /* 指定管脚输出异或 ，实现对应的LED指示灯状态取反目的 */
		  	  	 
	  LED_GPIO_2->ODR ^=led_blue;
	
}
