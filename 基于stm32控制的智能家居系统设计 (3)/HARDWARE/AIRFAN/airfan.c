#include "airfan.h"



//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void AIRFAN_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PC9 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC9
 GPIO_ResetBits(GPIOC,GPIO_Pin_9);						 //PC9 输出高
	
}
extern _Bool AIRFAN_FLAG;  //风扇
#define ON 1
#define OFF 0
void AIRFAN_control(void)
{
		 if(AIRFAN_FLAG)
		 {
				AIRFAN=ON;		 
		 }
		 else
		 {
			 
				AIRFAN=OFF;	
		 }	
}

