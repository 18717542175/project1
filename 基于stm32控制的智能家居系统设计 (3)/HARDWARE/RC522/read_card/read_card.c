/*******************************************
*本程序为IC卡读卡程序
*作者：电创陆小果
*QQ：3045136580
*********************************************/
#include "read_card.h"
#include "syn6288.h"

#define ON 1
#define OFF 0
extern _Bool Beep_Flage;

char table[]="2411101442";
void IC_test ( void )
{
	char cStr [ 30 ];
	u8 i,flag;
  u8 ucArray_ID [ 4 ];                                                                                             //先后存放IC卡的类型和UID(IC卡序列号)
	u8 ucStatusReturn;	
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //寻卡
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //若失败再次寻卡

		if ( ucStatusReturn == MI_OK  )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
			{
				sprintf ( cStr, "%d%d%d%d", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );
        	printf ( "%s\n",cStr );   //在这里打印出来卡号
				
				for(i=0;i<8;i++)   //密码比较
				{
             if(table[i]==cStr[i])
                 flag=1;
               else
                {
                    flag=0;
                    i=8;   //只要有一个错误就返回0
                }
				}				
				if(flag==1)
				{
					SYN_FrameInfo(2, "[v10][m1][t5]卡号正确");
					delay_ms(1500);
          printf("t0.txt=\"读取成功\"\xff\xff\xff"); 
					Beep_Flage=ON;
          flag=0;  
				}
        else
				{
					SYN_FrameInfo(2, "[v10][m1][t5]卡号错误");
					delay_ms(1500);
				  printf("t0.txt=\"错误卡号\"\xff\xff\xff"); 
					Beep_Flage=OFF;
          flag=0;
				}					
         while(PcdAnticoll ( ucArray_ID ) == MI_OK);
         printf("t0.txt=\"请插入IC卡\"\xff\xff\xff");   					 
			}			
		}
}


