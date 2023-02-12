#include "sys.h"
#include "usart3.h"
#include "timer.h"
#include "delay.h"
#include "syn6288.h"

#define ON 1
#define OFF 0
extern _Bool LED_FLAG;  //灯
extern _Bool DOOR_FLAG; //门
extern _Bool AIRFAN_FLAG;//风扇
extern _Bool WINDOW_FLAG;//窗帘
extern _Bool WINDOW_STATE;//ON窗帘工作，off窗帘不工作
extern _Bool SOCKET_FLAG;//插座
extern  u8 reclen;    //蓝牙接受字符串的长度
extern _Bool warning_mode;//0不报警 1 报警
extern _Bool WIN_FLAG;									//窗
extern u8 temperature; 								 //温度	    
extern u8 humidity; 										//湿度

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;

};

//FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}


//重定义fputc函数

int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //循环发送,直到发送完毕
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

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
u16 USART3_RX_STA = 0;     //接收状态标记
u16 USART1_RX_STA=0;       //接收状态标记	  


void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);	
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3_RX	  GPIOB.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口1

	TIM3_Int_Init(99, 7199);		//10ms中断
	USART3_RX_STA = 0;		//清零
	TIM_Cmd(TIM3, DISABLE);			//关闭定时器7
}



//LD3320语音交互中断
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
		{		
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		  Res=USART_ReceiveData(USART1);	//读取接收到的数据
			//USART_SendData(USART1,Res);//回传接收到的数据		
			
			Res = (char)Res;
			
			switch(Res)
			{
				case '1':
					SYN_FrameInfo(1, "[v3][m1][t5]我在");
					delay_ms(500);
				break;
				
				case '2':
					LED_FLAG=ON;
					SYN_FrameInfo(1, "[v3][m1][t5]灯开了");
					delay_ms(500);
				break;
				
				case '3':
						LED_FLAG=OFF;//关闭LED1
					SYN_FrameInfo(1, "[v3][m1][t5]灯已关");
					delay_ms(500);
				break;
				
				case '4':
					
					SYN_FrameInfo(1, "[v3][m1][t5]电视已开");
					delay_ms(500);
				break;
				
				case '5':
					SYN_FrameInfo(1, "[v3][m1][t5]电视已关");
					delay_ms(500);
				break;
				
				case '6':
						AIRFAN_FLAG=ON;
					SYN_FrameInfo(1, "[v3][m1][t5]风扇已开");
					delay_ms(500);
				break;
				
				case '7':
					AIRFAN_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]风扇已关");
					delay_ms(1500);
				break;
				
				case '8':
						DOOR_FLAG=ON;	
					SYN_FrameInfo(1, "[v3][m1][t5]门开了");
					delay_ms(500);
				break;
				
				case '9':
							DOOR_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]已关闭");
					delay_ms(500);
				break;
				
				case 'a':
						WIN_FLAG=ON;	
					SYN_FrameInfo(1, "[v3][m1][t5]好的");
					delay_ms(500);
				break;
				
				case 'b':
						WIN_FLAG=OFF;
					SYN_FrameInfo(1, "[v3][m1][t5]窗关了");
					delay_ms(500);
				break;
				
				case 'c':
					sprintf((char *)temperature,"ax:%.2d",temperature);//0300
					SYN_FrameInfo(1, "[v3][m1][t5]今天temperature度 ");
					delay_ms(500);
				break; 
				
				case 'd':
					SYN_FrameInfo(1, "[v3][m1][t5]天气很好 微风");
					delay_ms(500);
				break;
	
				default:
					SYN_FrameInfo(1, "[v30[m1][t5]请再说一遍");
					delay_ms(500);
					break;
								
				
			}
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送完毕
			
			
			USART1_RX_STA=0;
     }

} 


void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		if((USART3_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART3_RX_STA < USART3_REC_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM3, 0); //计数器清空          				//计数器清空
				if(USART3_RX_STA == 0) 				//使能定时器7的中断
				{
					TIM_Cmd(TIM3, ENABLE); //使能定时器7
				}
				USART3_RX_BUF[USART3_RX_STA++] = Res;	//记录接收到的值
			}
			else
			{
				USART3_RX_STA |= 1 << 15;				//强制标记接收完成
			}
		}
	}
}
#endif

