#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(9)	// PA9
#define LED1 PCout(0)	// PC0	

void LED_Init(void);//≥ı ºªØ

void led_control(void);			

#endif
