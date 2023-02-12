#include "RELAY.h"


void RELAY_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //PB13 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB13
 GPIO_SetBits(GPIOB,GPIO_Pin_13);						    
}
 
extern _Bool SOCKET_FLAG;//插座          

void realy_control(void)
{
		 if(SOCKET_FLAG)
		 {
				RELAY_ON;		 }
		 else
		 {
				RELAY_OFF;	
		 }
}
