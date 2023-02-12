#ifndef __PWM_H
#define __PWM_H
#include "sys.h"



void sg90_TIM1_PWM_Init(u16 arr,u16 psc);
void door_control(void);
void WIN_contorl(void);

void WINDOW_TIM3_PWM_INIT(u16 arr,u16 psc);
void WINDOW_control(void);
void DOOR_control(void);
#endif
