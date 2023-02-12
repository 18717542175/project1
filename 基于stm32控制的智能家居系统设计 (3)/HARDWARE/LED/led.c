#include "led.h"

    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PD端口时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PD端口时钟 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PA.9端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.9
 GPIO_SetBits(GPIOA,GPIO_Pin_9);						 //PA.9输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PC0端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC0
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);						 //PC0 输出高
}
extern _Bool LED_FLAG;  //灯
#define ON 1
#define OFF 0
void led_control(void)
{
		 if(LED_FLAG)
		 {
				LED1=ON;		 
		 }
		 else
		 {
				LED1=OFF;	
		 }	
}

