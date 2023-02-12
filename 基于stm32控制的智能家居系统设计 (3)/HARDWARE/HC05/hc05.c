#include "delay.h" 			 
#include "usart2.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"
#include "OLED_I2C.h"
#include "syn6288.h"
#include "timer.h"

//初始化ATK-HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	/*
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	//使能PORTA C时钟	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化A4
	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC4

	GPIO_SetBits(GPIOC,GPIO_Pin_4);
 	*/
	USART2_Init(9600);	//初始化串口2为:9600,波特率.
	
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u2_printf("AT\r\n");		//发送AT测试指令
		HC05_KEY=0;					//KEY拉低,退出AT模式
		for(t=0;t<10;t++) 			//最长等待50ms,来接收HC05模块的回应
		{
			if(USART2_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART2_RX_STA&0X8000)	//接收到一次数据了
		{
			temp=USART2_RX_STA&0X7FFF;	//得到数据长度
			USART2_RX_STA=0;			 
			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
			{
				temp=0;//接收到OK响应
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//检测失败
	return temp;	 
}	 
//获取ATK-HC05模块的角色
//返回值:0,从机;1,主机;0XFF,获取失败.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u2_printf("AT+ROLE?\r\n");	//查询角色
		for(t=0;t<20;t++) 			//最长等待200ms,来接收HC05模块的回应
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEY拉低,退出AT模式
		if(USART2_RX_STA&0X8000)	//接收到一次数据了
		{
			temp=USART2_RX_STA&0X7FFF;	//得到数据长度
			USART2_RX_STA=0;			 
			if(temp==13&&USART2_RX_BUF[0]=='+')//接收到正确的应答了
			{
				temp=USART2_RX_BUF[6]-'0';//得到主从模式值
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//查询失败.
	return temp;
} 							   
//ATK-HC05设置命令
//此函数用于设置ATK-HC05,适用于仅返回OK应答的AT指令
//atstr:AT指令串.比如:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"等字符串
//返回值:0,设置成功;其他,设置失败.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u2_printf("%s\r\n",atstr);	//发送AT字符串
		HC05_KEY=0;					//KEY拉低,退出AT模式
		for(t=0;t<20;t++) 			//最长等待100ms,来接收HC05模块的回应
		{
			if(USART2_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART2_RX_STA&0X8000)	//接收到一次数据了
		{
			temp=USART2_RX_STA&0X7FFF;	//得到数据长度
			USART2_RX_STA=0;			 
			if(temp==4&&USART2_RX_BUF[0]=='O')//接收到正确的应答了
			{			
				temp=0;
				break;			 
			}
		}		
	}
	if(retry==0)temp=0XFF;//设置失败.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//通过该函数,可以利用USMART,调试接在串口2上的ATK-HC05模块
//str:命令串.(这里注意不再需要再输入回车符)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEY置高,进入AT模式
	delay_ms(10);
	u2_printf("%s\r\n",(char*)str); //发送指令
	for(t=0;t<50;t++) 				//最长等待500ms,来接收HC05模块的回应
	{
		if(USART2_RX_STA&0X8000)break;
		delay_ms(10);
	}			
	
	HC05_KEY=0;						//KEY拉低,退出AT模式
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{
		temp=USART2_RX_STA&0X7FFF;	//得到数据长度
		USART2_RX_STA=0;
		USART2_RX_BUF[temp]=0;		//加结束符		 
		printf("\r\n%s",USART2_RX_BUF);//发送回应数据到串口1
	} 				 
}

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

void HC05_control(void)
{
		if(USART2_RX_STA&0X8000)			//接收到一次数据了
		{
				reclen=USART2_RX_STA&0X7FFF;	//得到数据长度
		  	USART2_RX_BUF[reclen]=0;	 	//加入结束符

				if(strcmp((const char*)USART2_RX_BUF,"+LED ON")==0)
				{				
					LED_FLAG=ON;	//打开LED1
						SYN_FrameInfo(2, "[v10][m1][t5]灯已开");
					delay_ms(1500);
				}
				if(strcmp((const char*)USART2_RX_BUF,"+LED OFF")==0)
				{	
					LED_FLAG=OFF;//关闭LED1
						SYN_FrameInfo(2, "[v10][m1][t5]灯已关");
					delay_ms(1500);
				}
				
			  if(strcmp((const char*)USART2_RX_BUF,"+WINDOW ON")==0)
				{
					WINDOW_FLAG=ON;
						SYN_FrameInfo(2, "[v10][m1][t5]窗帘已开");
					delay_ms(1500);
				}
					WINDOW_STATE=ON;	
				
				if(strcmp((const char*)USART2_RX_BUF,"+WINDOW OFF")==0)
				{
					WINDOW_FLAG=OFF;
					SYN_FrameInfo(2, "[v10][m1][t5]窗帘已关");
					delay_ms(1500);
				}
					WINDOW_STATE=ON;
				
			  if(strcmp((const char*)USART2_RX_BUF,"+AIRFAN ON")==0)
				{
					AIRFAN_FLAG=ON;
						SYN_FrameInfo(2, "[v10][m1][t5]风扇已开");
					delay_ms(1500);
				}
				
				if(strcmp((const char*)USART2_RX_BUF,"+AIRFAN OFF")==0)
				{
					AIRFAN_FLAG=OFF;
					SYN_FrameInfo(2, "[v10][m1][t5]风扇已关");
					delay_ms(1500);
				}
				
			  if(strcmp((const char*)USART2_RX_BUF,"+DOOR ON")==0)
				{
					DOOR_FLAG=ON;	
					SYN_FrameInfo(2, "[v10][m1][t5]门已开");
					delay_ms(1500);
				}
				
				if(strcmp((const char*)USART2_RX_BUF,"+DOOR OFF")==0)
				{
					DOOR_FLAG=OFF;
					SYN_FrameInfo(2, "[v10][m1][t5]门已关");
					delay_ms(1500);
				}
				
			  if(strcmp((const char*)USART2_RX_BUF,"+SOCKET ON")==0)
				{
					WIN_FLAG=ON;	
						SYN_FrameInfo(2, "[v10][m1][t5]窗已开");
					delay_ms(1500);
				}
				
				if(strcmp((const char*)USART2_RX_BUF,"+SOCKET OFF")==0)
				{					
					WIN_FLAG=OFF;
						SYN_FrameInfo(2, "[v10][m1][t5]窗已关");
					delay_ms(1500);
				}
				
 			USART2_RX_STA=0;	 
		}

}









									  


