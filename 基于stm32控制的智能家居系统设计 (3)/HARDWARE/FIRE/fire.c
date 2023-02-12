#include "fire.h"
#include "delay.h"
#include "sys.h"
#include "beep.h"

#define ON 1
#define OFF 0

extern _Bool Fire_Flage;								//�����־
extern _Bool Beep_Flage;								//��������־
extern _Bool SOCKET_FLAG;
void fire_init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //PA8 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC12
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);							//��PA8 ����
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