#include "fire.h"
#include "delay.h"
#include "sys.h"
#include "beep.h"

#define ON 1
#define OFF 0

extern _Bool Fire_Flage;								//火焰标志
extern _Bool Beep_Flage;								//蜂鸣器标志
extern _Bool SOCKET_FLAG;
void fire_init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //PA8 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC12
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);							//将PA8 拉低
}

void get_fire()
{
		 u8 t;
	
		 t=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8); 	
		 if(t==1)
		 {
		    SOCKET_FLAG=0;
		BEEP_OFF ;
		 }
		 if(t==0)
		 {
			 SOCKET_FLAG=1;
		BEEP_ON ;
			 
		 }			 
}