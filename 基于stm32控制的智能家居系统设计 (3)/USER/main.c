/***************************************************
	 @text 智能家居项目主函数
	 @time 2021/4/14/18:51
	 @author LiuGang
****************************************************/

#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"
#include "led.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "dht11.h"
#include "adc.h"
#include "BEEP.h"
#include "RELAY.h"
#include "timer.h"
#include "dht11.h"
#include "hc05.h"
#include "usart2.h"
#include "usart3.h"
#include "string.h"	
#include "pwm.h"
#include "airfan.h"
#include "HCSR501.h"
#include "key.h"
#include "timer.h"
#include "syn6288.h"
#include "uln2003.h"
#include "FIRE.h"

/*RC522头文件*/
#include "rc522_function.h"
#include "rc522_config.h"
#include "read_card.h"

#define ON 1
#define OFF 0

u8 oled_buffer[50];								//oled送字
u8 temperature; 								 //温度	    
u8 humidity; 										//湿度
u16 adcx;      									 //烟雾adc
u8 success_flage;

float temp;     								//电压小数点
u8 reclen=0;   									 //蓝牙接受字符串的长度

_Bool FLAG;     								//中断必死flag
_Bool LED_FLAG=OFF; 						//灯
_Bool DOOR_FLAG=OFF;						 //门
_Bool WIN_FLAG=0;									//窗
_Bool AIRFAN_FLAG=0;						//风扇
_Bool WINDOW_FLAG=OFF;					//窗帘
_Bool WINDOW_STATE=OFF;						//ON窗帘工作，off窗帘不工作
_Bool SOCKET_FLAG=OFF;						//插座
_Bool people_mode;								//0没人 1 有人
_Bool warning_mode;							//0不报警 1 报警
_Bool Beep_Flage;								//安防警报
_Bool Fire_Flage;								//火焰标志


int main(void)
{
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms 

	/*syn6288*/
	USART3_Init(9600);
	uart1_init(9600);
	
	/*OLED*/	
	OLED_Init();
	OLED_CLS();	
	
	/*温湿度传感器*/
	DHT11_Init();
				
	/*蜂鸣器*/
	Beep_Init();
	
	/*火焰传感器*/
	fire_init();
	
	/*按键*/
	KEY_Init();
	
	HCSR501_Init();
	RELAY_Init();
	
	/*RC522初始化*/
	RC522_Init();
	PcdReset();
	M500PcdConfigISOType('A');
	
	
	/*步进电机*/
	motor_init();
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	
	/*风扇*/
	AIRFAN_Init();
	
	/*ADC 烟雾*/
	Adc_Init();		

	/*窗户*/
	WINDOW_TIM3_PWM_INIT(7199,199);
	
	/*蓝牙*/
	HC05_Init();	
	printf("串口测试\r\n"); 
 	
	SYN_FrameInfo(2, "[v10][m1][t5]欢迎使用智慧家居控制系统");
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);



	while(1)
	{   
		/* //////////////////////////////界面//////////////////////////////*/
		//此时不产生警报
		if(warning_mode==0)
		{
			if(success_flage==0)
			{
					Beep_Flage=OFF;
					SYN_FrameInfo(2, "[v10][m1][t5]进入居家模式");
					delay_ms(1500);
			}
				//展示正常界面
		  show_display();	
			success_flage=1;
		}
		else
		{
			if(success_flage==1)
			{
				  Beep_Flage=ON;
					SYN_FrameInfo(2, "[v10][m1][t5]进入安防模式");
					delay_ms(1500);
			}
			success_flage=0;
			
			//产生警报 进入安防界面
		  show_warning();
		}
		
		/*////////////////////////////ld3320交互/////////////////////////*/
		// USART_NOW_RX_Buffer();
		
		/*/////////////////////////////中断/////////////////////////////*/
		 if(FLAG)
		 {
			 FLAG = 0;	
    /*//////////////////////////蓝牙接收/////////////////////////////*/
		   HC05_control(); 
		 }	      			
		 
		 /*采集adc的值*/
       adcx=Get_Adc_Average(ADC_Channel_1,10);  //读MQ135
			 temp=(float)adcx*(3.3/4096);
		 
		 
			 if(temp>1.2) 
					{BEEP_ON;} 
				else 
					{BEEP_OFF;}
					
					/*读取温湿度值*/
			 DHT11_Read_Data(&temperature,&humidity);	
			/**************************火焰检测**************************/
  			get_fire();
			/****************************RC522门禁*************************/
			IC_test();
			delay_ms(300);
			 /**************************发送给APP***************************/
			 u2_printf("TEMP:%dC    HUMI:%d%%\r\n",temperature,humidity);//发送给app
		   /*****************************按键*****************************/
		   KEY_control();
	     /*****************************灯控*****************************/
	  	 led_control();
			 /*****************************继电器*****************************/
		   realy_control();		  
		   /****************************门*******************************/
		   door_control();		  
		   /****************************风扇*******************************/
		   AIRFAN_control();
		  /****************************有人报警*******************************/
		   HCSR501_control();
			/****************************窗帘*******************************/
		   WINDOW_control();
			/****************************窗*******************************/
		   WIN_contorl();
	}
}


