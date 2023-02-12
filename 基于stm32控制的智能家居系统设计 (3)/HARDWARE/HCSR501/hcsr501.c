#include "HCSR501.h"
#include "BEEP.h"
#include "delay.h"

#define ON 1
#define OFF 0

void HCSR501_Init(void)
{
 
		 GPIO_InitTypeDef  GPIO_InitStructure;
		
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟 
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0--端口配置
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIO
	
	
}
extern _Bool warning_mode;//0不报警 1 报警
extern _Bool people_mode;////0没人 1 有人
extern _Bool Beep_Flage;	//

void HCSR501_control(void)
{
			u8 t;
		 t=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6); 	
		 if(t==0)
		 {
		    people_mode=1;	
				if(warning_mode)//有人闯入
				{
				  BEEP_ON; 
				}
				else
				{
				   BEEP_OFF;
				}			 
//			 (warning_mode==1)?(BEEP_ON):(BEEP_OFF);
		 }
		 if(t==1)
		 {
		    people_mode=0;
			  
		 } 
}
