/*******************************************
*������ΪIC����������
*���ߣ��紴½С��
*QQ��3045136580
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
  u8 ucArray_ID [ 4 ];                                                                                             //�Ⱥ���IC�������ͺ�UID(IC�����к�)
	u8 ucStatusReturn;	
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //Ѱ��
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //��ʧ���ٴ�Ѱ��

		if ( ucStatusReturn == MI_OK  )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����
			{
				sprintf ( cStr, "%d%d%d%d", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );
        	printf ( "%s\n",cStr );   //�������ӡ��������
				
				for(i=0;i<8;i++)   //����Ƚ�
				{
             if(table[i]==cStr[i])
                 flag=1;
               else
                {
                    flag=0;
                    i=8;   //ֻҪ��һ������ͷ���0
                }
				}				
				if(flag==1)
				{
					SYN_FrameInfo(2, "[v10][m1][t5]������ȷ");
					delay_ms(1500);
          printf("t0.txt=\"��ȡ�ɹ�\"\xff\xff\xff"); 
					Beep_Flage=ON;
          flag=0;  
				}
        else
				{
					SYN_FrameInfo(2, "[v10][m1][t5]���Ŵ���");
					delay_ms(1500);
				  printf("t0.txt=\"���󿨺�\"\xff\xff\xff"); 
					Beep_Flage=OFF;
          flag=0;
				}					
         while(PcdAnticoll ( ucArray_ID ) == MI_OK);
         printf("t0.txt=\"�����IC��\"\xff\xff\xff");   					 
			}			
		}
}


