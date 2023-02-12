#ifndef __RC522_FUNCTION_H
#define	__RC522_FUNCTION_H


#include "rc522_config.h"
#include "delay.h"

#define   macRC522_DELAY()  delay_us (200)



#define          macDummy_Data              0x00


void             PcdReset                   ( void );                       //复位
void             M500PcdConfigISOType       ( u8 type );                    //工作方式
char             PcdRequest                 ( u8 req_code, u8 * pTagType ); //寻卡
char             PcdAnticoll                ( u8 * pSnr);                   //读卡号
void                    SysTick_Init                            ( void );
void                    TimingDelay_Decrement                   ( void );


#endif /* __RC522_FUNCTION_H */

