#include "pwm.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "syn6288.h"
#include "uln2003.h"

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

#define ON 1
#define OFF 0
extern _Bool DOOR_FLAG; //��
extern _Bool WIN_FLAG;		//��



void WINDOW_TIM3_PWM_INIT(u16 arr,u16 psc)
{	
/*�����������*/
	GPIO_InitTypeDef GPIO_InitStrucsture;	//����ṹ��.
		/*��ʼ����ʱ��*/
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;		//����ṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);		//ʹ��GPIO�����AFIO���ù���
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		//ѡ��Timer3������ӳ��
	
	/*����GPIOģʽ*/
	GPIO_InitStrucsture.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_InitStrucsture.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStrucsture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrucsture);
	
	  //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStrucsture.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStrucsture.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStrucsture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStrucsture);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//ʹ�ܶ�ʱ��3ʱ��
	
	/*��ʼ��TIM3*/
	TIM_TimBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimBaseStructure.TIM_Period = arr;	//�Զ���װ�ؼĴ�����ֵ
	TIM_TimBaseStructure.TIM_Prescaler = psc; //Ԥ��Ƶֵ
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseStructure);
	
	/*tim3 ͨ����ʼ��*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	//ѡ��ʱ��ģʽ tim�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//ʹ��TIM1��OCR1�ϵ�Ԥװ��ֵ
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//ʹ��TIM1��OCR2�ϵ�Ԥװ��ֵ
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//ʹ��TIM1��OCR1�ϵ�Ԥװ��ֵ
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//ʹ��TIM1��OCR2�ϵ�Ԥװ��ֵ
	
	
	TIM_Cmd(TIM3, ENABLE);	//ʹ�ܶ�ʱ��3
}


void door_control(void)
{
		if(DOOR_FLAG)
		{
			TIM_SetCompare3(TIM3, 400);
		}
	else 
		{ 
				TIM_SetCompare3(TIM3, 125);
		}
}

void WIN_contorl()
{
	if(WIN_FLAG)
		{
			  TIM_SetCompare4(TIM3, 125);       //0��λ�ã�����
		}
		else
		{ 
		    TIM_SetCompare4(TIM3, 500);      //90��λ�ã�����
		}
}

extern _Bool WINDOW_FLAG;//����
extern _Bool WINDOW_STATE;//ON����������off����������

void WINDOW_control(void)
{
		if(WINDOW_FLAG==ON&&WINDOW_STATE==ON)
		 {
			    WINDOW_STATE=OFF;
			 /*
  		    TIM_SetCompare3(TIM3,200);//��������
			    TIM_SetCompare4(TIM3,800);
			    delay_ms(1000);
			    delay_ms(1000);
			    delay_ms(1000);
			    TIM_SetCompare3(TIM3,800);//��������
				  TIM_SetCompare4(TIM3,800);
			 */			 
			 motor_forward(5);
		 }
		 else if(WINDOW_FLAG==OFF&&WINDOW_STATE==ON)
		 {
			    WINDOW_STATE=OFF;
			 /*
				  TIM_SetCompare4(TIM3,200);//���ϴ���
					TIM_SetCompare3(TIM3,800);
					delay_ms(1000);
			    delay_ms(1000);
			    delay_ms(1000);
			    TIM_SetCompare3(TIM3,800);//��������
				  TIM_SetCompare4(TIM3,800);
			 */
			 motor_backward(5);
		 }
}
