#include "led.h"

    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PD�˿�ʱ�� 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PA.9�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.9
 GPIO_SetBits(GPIOA,GPIO_Pin_9);						 //PA.9�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PC0�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC0
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);						 //PC0 �����
}
extern _Bool LED_FLAG;  //��
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

