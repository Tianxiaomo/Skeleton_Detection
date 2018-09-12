#include "oled.h"
#include "ui.h"
#include "piclib.h"
#include "text.h"
#include "rtc.h"
#include "delay.h"
#include "log.h"
#include "key.h"
#include "led.h"

void Ui_Welcome(void)
{
	ai_load_picfile(pic_wel,0,0,oleddev.width,oleddev.height,1);
}

void Ui_Menu(void){
	u8 key;
	RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    u8 tbuf[40];
	static u8 Date = 33;
	static u8 sec = 100;
	
//	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //����WAKE UP�ж�,1�����ж�һ��  
	
	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,Menu_BC);		//ˢ����ɫ
	ai_load_picfile(pic_menu,0,0,oleddev.width,50,1);				//��ʾlogo
	
	Show_Str(4,110,35,16,"����",16,1,Menu_PC,Menu_BC,draw_point);
	Show_Str(47,110,35,16,"�˵�",16,1,Menu_PC,Menu_BC,draw_point);
	Show_Str(90,110,35,16,"����",16,1,Menu_PC,Menu_BC,draw_point);
	
//	while(1)
//    {
//		HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
//		if(sec != RTC_TimeStruct.Seconds){
//			sec = RTC_TimeStruct.Seconds;
//			sprintf((char*)tbuf,"%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
//			Show_Str(12,52,100,50,tbuf,16,1,Menu_PC,Menu_BC,draw_point);
//			debug(DEBUG,"%s",tbuf);
//			HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
//			if(Date != RTC_DateStruct.Date){
//				sprintf((char*)tbuf,"20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date);
//				Show_Str(12,72,100,50,tbuf,12,1,Menu_PC,Menu_BC,draw_point);
//				debug(DEBUG,"%s",tbuf);			
//				sprintf((char*)tbuf,"%d",RTC_DateStruct.WeekDay); 
//				Show_Str(12,92,100,50,tbuf,16,1,Menu_PC,Menu_BC,draw_point);
//				debug(DEBUG,"%s",tbuf);
//			}
//		}
//		
////      delay_ms(900);
//			
//        key=KEY_Scan(0);            //����ɨ��
//		switch(key)
//		{				 
//			case  KEY2_PRES:	//����LED0��ת
//				LED=!LED;
//				break;
//			case  KEY1_PRES:	//����LED1��ת	 
//				LED=!LED;
//				break;
//			case  KEY0_PRES:	//ͬʱ����LED0,LED1��ת 
//				LED=!LED;
//				break;
//		}
//	}  
}



