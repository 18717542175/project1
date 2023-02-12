#include "pwm.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "syn6288.h"
#include "uln2003.h"

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

#define ON 1
#define OFF 0
extern _Bool DOOR_FLAG; //门
extern _Bool WIN_FLAG;		//窗



void WINDOW_TIM3_PWM_INIT(u16 arr,u16 psc)
{	
/*引脚输出配置*/
	GPIO_InitTypeDef GPIO_InitStrucsture;	//定义结构体.
		/*初始化定时器*/
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;		//定义结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);		//使能GPIO外设和AFIO复用功能
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		//选择Timer3部分重映射
	
	/*配置GPIO模式*/
	GPIO_InitStrucsture.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_InitStrucsture.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStrucsture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrucsture);
	
	  //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStrucsture.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStrucsture.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStrucsture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStrucsture);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//使能定时器3时钟
	
	/*初始化TIM3*/
	TIM_TimBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimBaseStructure.TIM_Period = arr;	//自动重装载寄存器的值
	TIM_TimBaseStructure.TIM_Prescaler = psc; //预分频值
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseStructure);
	
	/*tim3 通道初始化*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	//选择定时器模式 tim脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//输出比较极性高
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIM1在OCR1上的预装载值
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIM1在OCR2上的预装载值
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIM1在OCR1上的预装载值
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//使能TIM1在OCR2上的预装载值
	
	
	TIM_Cmd(TIM3, ENABLE);	//使能定时器3
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
			  TIM_SetCompare4(TIM3, 125);       //0度位置，开门
		}
		else
		{ 
		    TIM_SetCompare4(TIM3, 500);      //90度位置，关门
		}
}

extern _Bool WINDOW_FLAG;//窗帘
extern _Bool WINDOW_STATE;//ON窗帘工作，off窗帘不工作

void WINDOW_control(void)
{
		if(WINDOW_FLAG==ON&&WINDOW_STATE==ON)
		 {
			    WINDOW_STATE=OFF;
			 /*
  		    TIM_SetCompare3(TIM3,200);//拉开窗帘
			    TIM_SetCompare4(TIM3,800);
			    delay_ms(1000);
			    delay_ms(1000);
			    delay_ms(1000);
			    TIM_SetCompare3(TIM3,800);//窗帘不动
				  TIM_SetCompare4(TIM3,800);
			 */			 
			 motor_forward(5);
		 }
		 else if(WINDOW_FLAG==OFF&&WINDOW_STATE==ON)
		 {
			    WINDOW_STATE=OFF;
			 /*
				  TIM_SetCompare4(TIM3,200);//拉上窗帘
					TIM_SetCompare3(TIM3,800);
					delay_ms(1000);
			    delay_ms(1000);
			    delay_ms(1000);
			    TIM_SetCompare3(TIM3,800);//窗帘不动
				  TIM_SetCompare4(TIM3,800);
			 */
			 motor_backward(5);
		 }
}
