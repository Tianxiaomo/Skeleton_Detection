
/***********************************************************************************************************************
Skeleton_Detection
�ļ���:        ui.c
����   :       ����ui��ʾ

�汾   :       V1.0
�޸�   :   
������ڣ�     2016.10.14



************************************************************************************************************************/

/***********************************************************************************************************************
* INCLUDES
*/
#include "oled.h"
#include "ui.h"
#include "FreeRTOS.h"
#include "bmp.h"
#include "rtc.h"
//#include "sht20.h"
//#include "pedometer.h"
//#include "pulse.h"
//#include "bp.h"
//#include "timer.h"
//#include "protocol.h"
#include "battery.h"
#include "app_handler.h"
#include "app_task.h"
//#include "bluetooth.h"
//#include "pedometer.h"
#include "log.h"
/***********************************************************************************************************************
* CONSTANTS
*/


/***********************************************************************************************************************
* TYPEDEFS
*/


/***********************************************************************************************************************
* LOCAL VARIABLES
*/

EventGroupHandle_t ui_event;
TimerHandle_t 	   ui_timer;	//ui�����ʱ��
u32 ui_page;   
ui_state_t ui_state ={0};
/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
void Ui_clearScreen(void);
void Ui_timerHandler(TimerHandle_t xTimer);
static void Ui_pulseDynamicUi(void);

/***********************************************************************************************************************
* LOCAL FUNCTIONS  
*/



/***********************************************************************************************************************
* PUBLIC FUNCTIONS
*/

/***************************************************************************************************************
* PROFILE CALLBACKS
*/



/***************************************************************************************************************
* PUBLIC FUNCTIONS
*/
/*************************************************************************************************************
* ��������   Ui_timerHandler,��ʱ���жϺ���
* ���������� ��ʱ�жϣ��ر���ʱ����power_off UI,����ui_pageΪUI_NO_PAGE
* ���ߣ�     Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
void Ui_timerHandler(TimerHandle_t xTimer) 
{
  ui_state.ui_power = 0;        //�ص�ui
  ui_state.ui_page = UI_NO_PAGE;//����ui_pageΪUI_NO_PAGE
  //������Ϊ�͹���,�ص���
  // OLED_displayOff();
  Ui_clearScreen();	
}
/*******************************************************************************
* ��������  Ui_init
* ����������ui��ʼ��
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_init(void)
{
	ui_event=xEventGroupCreate();	 		//����UI�¼���־��
	ui_timer=xTimerCreate((const char*		)"AutoReloadTimer",
						(TickType_t			)6000,
						(UBaseType_t		)pdFALSE,
						(void*				)1,
						(TimerCallbackFunction_t)Ui_timerHandler); //���ڶ�ʱ��������6s(6000��ʱ�ӽ���)������ģʽ
  //UI�߳���ʱ��,��ʱ����Ϊ6s����ʱʱ�䵽�͹ر���,ע��ö�ʱʱ����Ҫ���ڵ���5s,Ŀ���Ƿ��ò������ʡ�Ѫѹʱ�ᰵ��
}
/*******************************************************************************
* ��������   Ui_startSoftTimer
* ���������� ����ʱ������ʱʱ�䵽��������ʱ�ж�
* ���ߣ�     Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
void Ui_startSoftTimer(void)
{
	xTimerStart(ui_timer,0);			//������ʱ��
}

/*******************************************************************************
* ��������  ui_stateSet
* ��������������ui��״̬
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_stateSet(u32 ui_page,u8 ui_power)
{
	ui_state.ui_page = ui_page;
	ui_state.ui_power = ui_power;
}

/*******************************************************************************
* ��������  ui_stateGet
* ������������ȡui��״̬
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
ui_state_t *Ui_stateGet(void)
{
  return &ui_state;
}
/********************************************************************************
* ��������  Ui_postPage
* ������������ҳ,
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
***********************************************************************************/
void Ui_postPage(u32 page)	 
{
	xEventGroupSetBits(ui_event,page);
}
/**********************************************************************************
* ��������  Ui_clearScreen
* ��������������
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************/
void Ui_clearScreen(void)
{
//  OLED_clear();
}
/*************************************************************************************************************
* ��������  Ui_getRTC
* ������������ȡRTCʵʱʱ��
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
dateAndTime_t* Ui_getRTC(void)	 
{ 
  return RTC_getDateAndTime();
}
/*************************************************************************************************************
* ��������  Ui_getTempHumi
* ������������ȡ��ʪ��
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
//TempHumiValue_t* Ui_getTempAndHumi(void)	 
//{ 
//  return SHT20_readTemAndHum();
//}
/*************************************************************************************************************
* ��������  Ui_showString
* ������������ʾ�ַ���
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
void Ui_showString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 Char_Num)
{
	Show_Str(x,y,Char_Size*Char_Num,Char_Size,chr,Char_Size,1,TIME_COLOR,BACKGROUD_COLOR,draw_point);
}
/*************************************************************************************************************
* ��������  Ui_showPicture
* ������������ʾͼƬ
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/
void Ui_showPicture(const u8 *filename,u8 x,u8 y,u8 w,u8 h)
{
	ai_load_picfile(filename,x,y,w,h,1);
}
/*******************************************************************************
* ��������  ui_MainPage
* ������������ʾʱ�����
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
static void Ui_ShowMainPage(void)
{
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//ʱ��&����
	batteryState_t *batteryState;     //��ȡ���״̬
	
	Ui_clearScreen(); 
	Ui_stateSet(UI_HOME_PAGE,UI_POWER_ON);
	
	debug(WARN,"��ʾ��ҳ");
	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);
	Ui_showPicture(LOGO_ICO,13,18,100,38);						//logo��ʾ
	Ui_showPicture(LEFT_ICO,3,51,12,16);
	Ui_showPicture(RIGHT_ICO,111,51,12,16);	
	
	/*******************<��ص�����ʾ�߼�>*************************/	
	batteryState = Battery_getStatus();
	if( BATTERY_CHARGE_ING == batteryState->chargeState ) //��س����
	{
		switch(batteryState->batteryRemain){
			case BATTERY_REMAIN_100:
				ai_load_picfile(BATTERY_CHARGING_JPG_100,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_90:
				ai_load_picfile(BATTERY_CHARGING_JPG_90,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_80:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_70:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_60:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_50:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_40:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_30:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_20:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_10:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			default:
				break; 
		}
	}
	else if( BATTERY_CHARGE_FINISH == batteryState->chargeState ) //������
	{
		ai_load_picfile(BATTERY_CHARGING_JPG_100,0,0,100,100,1);//��ʾͼ��  //�����磬��ʾ100%
	}
	else   //BATTERY_DIS_CHARGE == batteryState->chargeState  //��طŵ���
	{
		switch(batteryState->batteryRemain){
			case BATTERY_REMAIN_100:
				ai_load_picfile(BATTERY_JPG_100,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_90:
				ai_load_picfile(BATTERY_JPG_90,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_80:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_70:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_60:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_50:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_40:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_30:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_20:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//��ʾͼ��
				break;
			case BATTERY_REMAIN_10:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//��ʾͼ��
				break;
			default:
				break; 
		}
	}	
	/*******************<��ȡRTCʱ�����ڣ�����ʾ>*************************/	
	dateAndTime = Ui_getRTC();
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	
	debug(WARN,"ʱ�䣺 %s",uiBuf);
	
	Ui_showString(23,56,uiBuf,32,5);
	sprintf((char*)uiBuf,"20%02d.%02d.%02d",dateAndTime->year,dateAndTime->month,dateAndTime->date); 
	Ui_showString(33,90,uiBuf,12,10);
//	switch(dateAndTime->week)
//	{
//		case MONDAY:
//			Ui_showString(10+8*10,6,"Mon",16,3);
//			break;
//		case TUESDAY:
//			Ui_showString(10+8*10,6,"Tues",16,4);
//			break;
//		case WEDNESDAY:
//			Ui_showString(10+8*10,6,"Wed",16,3);
//			break;
//		case THURSDAY:
//			Ui_showString(10+8*10,6,"Thur",16,4);
//			break;
//		case FRIDAY:
//			Ui_showString(10+8*10,6,"Fri",16,3);
//			break;
//		case SATURDAY:
//			Ui_showString(10+8*10,6,"Sat",16,3);
//			break;
//		case SUNDAY:
//			Ui_showString(10+8*10,6,"Sun",16,3);
//			break;
//		default:
//			break;		
//	}
}



/*******************************************************************************
* ��������ui_NoPage
* �����������ر�ui 
* ���ߣ�Momo  
* ����˵����  
* ����ֵ˵��:
* �޸ļ�¼��
*******************************************************************************/
static void Ui_NoPage(void)
{
//  OLED_clear();   
}

/*******************************************************************************
* ��������  Ui_ShowPattern1Page
* ����������ģʽ1
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵��:
* �޸ļ�¼��
*******************************************************************************/
static void Ui_ShowPattern1Page(void){
	Ui_showPicture(PATTERY_1_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_PATTERN_1_PAGE;
}

/*******************************************************************************
* ��������  Ui_ShowPattern2Page
* ����������ģʽ2
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_ShowPattern2Page(void){
	Ui_showPicture(PATTERY_2_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_PATTERN_2_PAGE;
}

/*******************************************************************************
* ��������  Ui_ShowSetPage
* ��������������
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_ShowSetPage(void){
	Ui_showPicture(SET_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_SET_PAGE;
}

/*******************************************************************************
* ��������  Ui_FilePage
* �����������ļ�
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_FilePage(void){
	Ui_showPicture(FILE_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_FILE_PAGE;
}

/*******************************************************************************
* ��������  Ui_Detection1Page
* �������������
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_Detection1Page(void){
	Ui_showPicture(LOADING_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_DETECTION1_PAGE;
}
/*******************************************************************************
* ��������  Ui_Detection2Page
* �������������
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_Detection2Page(void){
	Ui_showPicture(LOADING_ICO,31,21,64,64);//��ʾͼ��
	ui_state.ui_page = UI_DETECTION2_PAGE;
}
/*******************************************************************************
* ��������  Ui_SetSubPage
* ����������������ҳ
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_SetSubPage(void){
	Ui_showPicture(TIME_ICO,5,5,16,16);//��ʾͼ��
	Ui_showString(31,5,"���ʱ��",16,4);
	Ui_showPicture(CLOCK_ICO,5,25,16,16);//��ʾͼ��
	Ui_showString(31,25,"ʱ������",16,4);
	Ui_showPicture(LANG_ICO,5,45,16,16);//��ʾͼ��
	Ui_showString(31,45,"ϵͳ����",16,4);
	Ui_showPicture(CONTENT_ICO,5,65,16,16);//��ʾͼ��
	Ui_showString(31,65,"�洢����",16,4);
	Ui_showPicture(ABOUT_ICO,5,85,16,16);//��ʾͼ��
	Ui_showString(31,85,"����",16,2);
	ui_state.ui_page = UI_SET_SUB_PAGE;
}

/*******************************************************************************
* ��������  ui_poll
* ����������ui��ѭ 
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/

void Ui_poll(void)
{
	EventBits_t EventValue;
	
	EventValue=xEventGroupWaitBits((EventGroupHandle_t	)ui_event,		//�ȴ��ź�������,�ȴ�page��λ 
							   (EventBits_t			)UI_ALL_PAGE,
							   (BaseType_t			)pdTRUE,				
							   (BaseType_t			)pdFALSE,
							   (TickType_t			)portMAX_DELAY);
	
	debug(WARN,"UI�¼� %d",EventValue);
	
	switch(EventValue){
		case UI_HOME_PAGE:
			Ui_ShowMainPage();
			break;
		case UI_PATTERN_1_PAGE:
			Ui_ShowPattern1Page();
			break;
		case UI_PATTERN_2_PAGE:
			Ui_ShowPattern2Page();
			break;
		case UI_SET_PAGE:
			Ui_ShowSetPage();
			break;
		case UI_FILE_PAGE:
			Ui_FilePage();
			break;
		case UI_DETECTION1_PAGE:
			Ui_Detection1Page();
			break;
		case UI_DETECTION2_PAGE:
			Ui_Detection2Page();
			break;
		case UI_SET_SUB_PAGE:
			Ui_SetSubPage();
			break;
//		case UI_FILE_SUB_PAGE:
//			Ui_FileSubPage();
//			break;
//		case UI_SET_TIME_PAGE:
//			Ui_SetTimePage();
//			break;
//		case UI_SET_LAG_PAGE:
//			Ui_SetLagPage();
//			break;
//		case UI_SET_SD_PAGE:
//			Ui_SetSdPage();
//			break;
		default:
			break;
	}
}



/***********************************************************************************************************************
***********************************************************************************************************************/


