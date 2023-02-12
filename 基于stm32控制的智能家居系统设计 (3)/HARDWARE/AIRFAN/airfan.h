#ifndef __AIRFAN_H
#define __AIRFAN_H	 
#include "sys.h"

#define AIRFAN PCout(9)	// PA8
	

void AIRFAN_Init(void);//≥ı ºªØ

void AIRFAN_control(void);			    
#endif
