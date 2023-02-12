/*  
*		���ݣ�ULN2003�����������
*		��дʱ�䣺2021��3��5��
*/

#include "uln2003.h"
#include "sys.h"
#include "delay.h"


void motor_init()
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//PB9 10 11 12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	IN1(0);
	IN2(0);
	IN3(0);
	IN4(0);

 
}
void motor_forward(u16 circle)
{
	int n=0;
	//�������תһȦ��Ҫ32������
	//һ��ѭ����4��
	while(n<=(circle*32*1))
	{
		IN1(1);
		IN2(1);
		IN3(0);
		IN4(0);
		delay_ms(10);
		IN1(0);
		IN2(1);
		IN3(1);
		IN4(0);
		delay_ms(10);
		IN1(0);
		IN2(0);
		IN3(1);
		IN4(1);
		delay_ms(10);
		IN1(1);
		IN2(0);
		IN3(0);
		IN4(1);
		//
		delay_ms(10);
		n++;
	}
	
}
void motor_backward(u16 circle)
{
		int n=0;
	//�������תһȦ��Ҫ32������
	//һ��ѭ����4��
	while(n<=(circle*32*1))
	{
		IN1(1);
		IN2(1);
		IN3(0);
		IN4(0);
		delay_ms(10);
		IN1(1);
		IN2(0);
		IN3(0);
		IN4(1);
		delay_ms(10);
		IN1(0);
		IN2(0);
		IN3(1);
		IN4(1);
		delay_ms(10);
		IN1(0);
		IN2(1);
		IN3(1);
		IN4(0);
		delay_ms(10);
		n++;
	}

}

void motor_stop()
{
		IN1(1);
		IN2(1);
		IN3(1);
		IN4(1);
	delay_ms(100);
}







