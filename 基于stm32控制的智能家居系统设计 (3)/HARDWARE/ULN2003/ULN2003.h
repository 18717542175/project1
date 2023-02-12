#ifndef __ULN2003_H
#define __ULN2003_H

#include "sys.h"

/*
#define IN1 PBout(9);
#define IN2 PBout(10);
#define IN3 PBout(11);
#define IN4 PBout(12);.
*/

#define IN1(a) if(a) \
	GPIO_SetBits(GPIOC, GPIO_Pin_1); \
else \
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);

#define IN2(a) if(a) \
	GPIO_SetBits(GPIOC, GPIO_Pin_2); \
else \
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);

#define IN3(a) if(a) \
	GPIO_SetBits(GPIOC, GPIO_Pin_3); \
else \
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);

#define IN4(a) if(a) \
	GPIO_SetBits(GPIOC, GPIO_Pin_4); \
else \
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);

void motor_init(void);
//正转函数
void motor_forward(u16 circle);
//反转函数
void motor_backward(u16 circle);
void motor_stop();

#endif









