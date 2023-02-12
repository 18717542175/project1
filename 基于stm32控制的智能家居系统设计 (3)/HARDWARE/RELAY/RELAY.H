#ifndef __RELAY_H
#define __RELAY_H	
#include "sys.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define RELAY_ON PBout(13)=0;	// PA8
#define RELAY_OFF PBout(13)=1;	// PD2

void RELAY_Init(void);

 void realy_control(void);
#endif 
