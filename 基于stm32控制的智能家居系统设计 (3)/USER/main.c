/***************************************************
	 @text ���ܼҾ���Ŀ������
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

/*RC522ͷ�ļ�*/
#include "rc522_function.h"
#include "rc522_config.h"
#include "read_card.h"

#define ON 1
#define OFF 0

u8 oled_buffer[50];								//oled����
u8 temperature; 								 //�¶�	    
u8 humidity; 										//ʪ��
u16 adcx;      									 //����adc
u8 success_flage;

float temp;     								//��ѹС����
u8 reclen=0;   									 //���������ַ����ĳ���

_Bool FLAG;     								//�жϱ���flag
_Bool LED_FLAG=OFF; 						//��
_Bool DOOR_FLAG=OFF;						 //��
_Bool WIN_FLAG=0;									//��
_Bool AIRFAN_FLAG=0;						//����
_Bool WINDOW_FLAG=OFF;					//����
_Bool WINDOW_STATE=OFF;						//ON����������off����������
_Bool SOCKET_FLAG=OFF;						//����
_Bool people_mode;								//0û�� 1 ����
_Bool warning_mode;							//0������ 1 ����
_Bool Beep_Flage;								//��������
_Bool Fire_Flage;								//�����־


int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 

	/*syn6288*/
	USART3_Init(9600);
	uart1_init(9600);
	
	/*OLED*/	
	OLED_Init();
	OLED_CLS();	
	
	/*��ʪ�ȴ�����*/
	DHT11_Init();
				
	/*������*/
	Beep_Init();
	
	/*���洫����*/
	fire_init();
	
	/*����*/
	KEY_Init();
	
	HCSR501_Init();
	RELAY_Init();
	
	/*RC522��ʼ��*/
	RC522_Init();
	PcdReset();
	M500PcdConfigISOType('A');
	
	
	/*�������*/
	motor_init();
	
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	/*����*/
	AIRFAN_Init();
	
	/*ADC ����*/
	Adc_Init();		

	/*����*/
	WINDOW_TIM3_PWM_INIT(7199,199);
	
	/*����*/
	HC05_Init();	
	printf("���ڲ���\r\n"); 
 	
	SYN_FrameInfo(2, "[v10][m1][t5]��ӭʹ���ǻۼҾӿ���ϵͳ");
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);



	while(1)
	{   
		/* //////////////////////////////����//////////////////////////////*/
		//��ʱ����������
		if(warning_mode==0)
		{
			if(success_flage==0)
			{
					Beep_Flage=OFF;
					SYN_FrameInfo(2, "[v10][m1][t5]����Ӽ�ģʽ");
					delay_ms(1500);
			}
				//չʾ��������
		  show_display();	
			success_flage=1;
		}
		else
		{
			if(success_flage==1)
			{
				  Beep_Flage=ON;
					SYN_FrameInfo(2, "[v10][m1][t5]���밲��ģʽ");
					delay_ms(1500);
			}
			success_flage=0;
			
			//�������� ���밲������
		  show_warning();
		}
		
		/*////////////////////////////ld3320����/////////////////////////*/
		// USART_NOW_RX_Buffer();
		
		/*/////////////////////////////�ж�/////////////////////////////*/
		 if(FLAG)
		 {
			 FLAG = 0;	
    /*//////////////////////////��������/////////////////////////////*/
		   HC05_control(); 
		 }	      			
		 
		 /*�ɼ�adc��ֵ*/
       adcx=Get_Adc_Average(ADC_Channel_1,10);  //��MQ135
			 temp=(float)adcx*(3.3/4096);
		 
		 
			 if(temp>1.2) 
					{BEEP_ON;} 
				else 
					{BEEP_OFF;}
					
					/*��ȡ��ʪ��ֵ*/
			 DHT11_Read_Data(&temperature,&humidity);	
			/**************************������**************************/
  			get_fire();
			/****************************RC522�Ž�*************************/
			IC_test();
			delay_ms(300);
			 /**************************���͸�APP***************************/
			 u2_printf("TEMP:%dC    HUMI:%d%%\r\n",temperature,humidity);//���͸�app
		   /*****************************����*****************************/
		   KEY_control();
	     /*****************************�ƿ�*****************************/
	  	 led_control();
			 /*****************************�̵���*****************************/
		   realy_control();		  
		   /****************************��*******************************/
		   door_control();		  
		   /****************************����*******************************/
		   AIRFAN_control();
		  /****************************���˱���*******************************/
		   HCSR501_control();
			/****************************����*******************************/
		   WINDOW_control();
			/****************************��*******************************/
		   WIN_contorl();
	}
}


