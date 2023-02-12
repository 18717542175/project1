#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__
#include "stdint.h"
#include "stm32f10x_iwdg.h"

void IWDG_Init(uint8_t prer,uint16_t rlr);
void IWDG_Feed(void);


#endif
