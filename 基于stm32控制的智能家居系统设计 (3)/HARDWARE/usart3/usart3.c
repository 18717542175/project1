#include "sys.h"
#include "usart3.h"
#include "timer.h"
#include "delay.h"
#include "syn6288.h"

#define ON 1
#define OFF 0
extern _Bool LED_FLAG;  //��
extern _Bool DOOR_FLAG; //��
extern _Bool AIRFAN_FLAG;//����
extern _Bool WINDOW_FLAG;//����
extern _Bool WINDOW_STATE;//ON����������off����������
extern _Bool SOCKET_FLAG;//����
extern  u8 reclen;    //���������ַ����ĳ���
extern _Bool warning_mode;//0������ 1 ����
extern _Bool WIN_FLAG;									//��
extern u8 temperature; 								 //�¶�	    
extern u8 humidity; 										//ʪ��

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

//FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}


//�ض���fputc����

int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //ѭ������,ֱ���������
	USART3->DR = (u8) ch;
	return ch;
}

_ttywrch(int ch)
{
ch = ch;
}

#endif

void USART3_SendData(u8 data)
{
	while((USART3->SR & 0X40) == 0);
	USART3->DR = data;
}

void USART3_SendString(u8 *DAT, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART3_SendData(*DAT++);
	}
}

#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
u16 USART3_RX_STA = 0;     //����״̬���
u16 USART1_RX_STA=0;       //����״̬���	  


void uart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);	
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3_RX	  GPIOB.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1

	TIM3_Int_Init(99, 7199);		//10ms�ж�
	USART3_RX_STA = 0;		//����
	TIM_Cmd(TIM3, DISABLE);			//�رն�ʱ��7
}



//LD3320���������ж�
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
		{		
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		  Res=USART_ReceiveData(USART1);	//��ȡ���յ�������
			//USART_SendData(USART1,Res);//�ش����յ�������		
			
			Res = (char)Res;
			
			switch(Res)
			{
				case '1':
					SYN_FrameInfo(1, "[v3][m1][t5]����");
					delay_ms(500);
				break;
				
				case '2':
					LED_FLAG=ON;
					SYN_FrameInfo(1, "[v3][m1][t5]�ƿ���");
					delay_ms(500);
				break;
				
				case '3':
						LED_FLAG=OFF;//�ر�LED1
					SYN_FrameInfo(1, "[v3][m1][t5]���ѹ�");
					delay_ms(500);
				break;
				
				case '4':
					
					SYN_FrameInfo(1, "[v3][m1][t5]�����ѿ�");
					delay_ms(500);
				break;
				
				case '5':
					SYN_FrameInfo(1, "[v3][m1][t5]�����ѹ�");
					delay_ms(500);
				break;
				
				case '6':
						AIRFAN_FLAG=ON;
					SYN_FrameInfo(1, "[v3][m1][t5]�����ѿ�");
					delay_ms(500);
				break;
				
				case '7':
					AIRFAN_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]�����ѹ�");
					delay_ms(1500);
				break;
				
				case '8':
						DOOR_FLAG=ON;	
					SYN_FrameInfo(1, "[v3][m1][t5]�ſ���");
					delay_ms(500);
				break;
				
				case '9':
							DOOR_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]�ѹر�");
					delay_ms(500);
				break;
				
				case 'a':
						WIN_FLAG=ON;	
					SYN_FrameInfo(1, "[v3][m1][t5]�õ�");
					delay_ms(500);
				break;
				
				case 'b':
						WIN_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]������");
					delay_ms(500);
				break;
				
				case 'c':
					sprintf((char *)temperature,"ax:%.2d",temperature);//0300
					SYN_FrameInfo(1, "[v3][m1][t5]����temperature�� ");
					delay_ms(500);
				break; 
				
				case 'd':
					SYN_FrameInfo(1, "[v3][m1][t5]�����ܺ� ΢��");
					delay_ms(500);
				break;
	
				default:
					SYN_FrameInfo(1, "[v30[m1][t5]����˵һ��");
					delay_ms(500);
					break;
								
				
			}
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ��������
			
			
			USART1_RX_STA=0;
     }

} 


void USART3_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		if((USART3_RX_STA & 0x8000) == 0) //����δ���
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM3, 0); //���������          				//���������
				if(USART3_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
				{
					TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//��¼���յ���ֵ
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
			}
		}
	}
}
#endif

