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
#include "battery.h"
#include "app_handler.h"
#include "app_task.h"
#include "app_detect.h"
#include "log.h"
#include "piclib.h"
#include "semphr.h"

/***********************************************************************************************************************
* CONSTANTS
*/


/***********************************************************************************************************************
* TYPEDEFS
*/


/***********************************************************************************************************************
* LOCAL VARIABLES
*/

//EventGroupHandle_t ui_event;
SemaphoreHandle_t ui_Sem;	//��ֵ�ź������
long long ui_Event;
TimerHandle_t 	   ui_timer;	//ui�����ʱ��
ui_state_t ui_state ={0};
/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
void Ui_clearScreen(void);
void Ui_timerHandler(TimerHandle_t xTimer);
static void Ui_pulseDynamicUi(void);
static void Ui_ShowInfoPage(u32 ui_page,u32 next_page);
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
//	ui_event=xEventGroupCreate();	 		//����UI�¼���־��
	ui_Sem = xSemaphoreCreateBinary();	
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
//void Ui_postPage(u32 page)	 
//{
//	xEventGroupSetBits(ui_event,page);
//}

void Ui_postPage(long long page){
	xSemaphoreGive(ui_Sem);	//�ͷŶ�ֵ�ź���
	ui_Event |= page;
}

/********************************************************************************
* ��������  Ui_delPage
* ������������ҳ,
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
***********************************************************************************/
void Ui_delPage(long long page){
	ui_Event &= ~page;
}
/**********************************************************************************
* ��������  Ui_clearScreen
* ��������������
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************/
void Ui_clearScreen(void){
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
dateAndTime_t* Ui_getRTC(void)	 { 
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
/*******************************************************************************
* ��������  Ui_showString
* ������������ʾ�ַ���
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*********************************************************************************/
void Ui_showString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 Char_Num){
	Show_Str(x,y,Char_Size*Char_Num,Char_Size,chr,Char_Size,1,TIME_COLOR,BACKGROUD_COLOR,draw_point);
}
/********************************************************************************
* ��������  Ui_showPicture
* ������������ʾͼƬ
* ���ߣ�    Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*********************************************************************************/
void Ui_showPicture(const u8 *filename,u8 x,u8 y,u8 w,u8 h){
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
static void Ui_ShowMainPage(void){
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//ʱ��&����
	batteryState_t *batteryState;     //��ȡ���״̬
	
	Ui_ShowInfoPage(ui_state.ui_page,UI_HOME_PAGE);
	Ui_stateSet(UI_HOME_PAGE,UI_POWER_ON);

	debug(WARN,"��ʾ��ҳ");
	Ui_showPicture(LOGO_ICO,13,18,100,38);						//logo��ʾ
	Ui_showPicture(LEFT_ICO,3,51,12,16);
	Ui_showPicture(RIGHT_ICO,111,51,12,16);	
	Ui_showPicture(MORE_ICO,51,116,24,8);
	
	/*******************<��ص�����ʾ�߼�>*************************/	
	batteryState = Battery_getStatus();
	Ui_showPicture(BATTERY_CHARGING_JPG_100,111,0,16,16);
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
	debug(WARN,"ʱ�䣺 %s	���ڣ�20%d-%d-%d %d",uiBuf,dateAndTime->year,dateAndTime->month,dateAndTime->date,dateAndTime->week);
	Ui_showString(23,56,uiBuf,32,5);
	
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	debug(WARN,"ʱ�䣺 %s",uiBuf);
	Ui_showString(1,2,uiBuf,12,5);
	
	sprintf((char*)uiBuf,"20%02d.%02d.%02d",dateAndTime->year,dateAndTime->month,dateAndTime->date); 
	Ui_showString(25,90,uiBuf,12,10);
	switch(dateAndTime->week)
	{
		case MONDAY:
			Ui_showString(90,90,"Mon",12,3);
			break;
		case TUESDAY:
			Ui_showString(90,90,"Tues",12,4);
			break;
		case WEDNESDAY:
			Ui_showString(90,90,"Wed",12,3);
			break;
		case THURSDAY:
			Ui_showString(90,90,"Thur",12,4);
			break;
		case FRIDAY:
			Ui_showString(90,90,"Fri",12,3);
			break;
		case SATURDAY:
			Ui_showString(90,90,"Sat",12,3);
			break;
		case SUNDAY:
			Ui_showString(90,90,"Sun",12,3);
			break;
		default:
			break;		
	}
}



/*******************************************************************************
* ��������ui_NoPage
* �����������ر�ui 
* ���ߣ�Momo  
* ����˵����  
* ����ֵ˵��:
* �޸ļ�¼��
*******************************************************************************/
static void Ui_NoPage(void){
//  OLED_clear();   
}
/*******************************************************************************
* ��������  Ui_ShowInfoPage
* ��������������ҳ������һ��ҳ���ʱ�Ӻ͵�����ʾ
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵��:
* �޸ļ�¼��
*******************************************************************************/
static void Ui_ShowInfoPage(u32 ui_page,u32 next_page){
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//ʱ��&����
	batteryState_t *batteryState;     //��ȡ���״̬
		
	debug(WARN,"��ʾ��Ϣҳ�� %d -> %d",ui_page,next_page);
//	debug(ERROR,"first %d ,second %d",UI_FIRST_PAGE,UI_SECOND_PAGE);
//	debug(WARN,"�� %d && %d = %d",(ui_page & UI_FIRST_PAGE),(next_page & UI_FIRST_PAGE),(ui_page & UI_FIRST_PAGE) && (next_page & UI_FIRST_PAGE));
	if(UI_HOME_PAGE == ui_page){												//��homeҳ����
		debug(WARN,"��homeҳ����");
		Fill_Block_A(13,18,100,38,WHITE);				//logo����	
		Fill_Block_A(23,56,80,32,WHITE);				//ʱ�����
		Fill_Block_A(25,90,90,12,WHITE);				//���ڲ���
	}else if((ui_page & UI_FIRST_PAGE) && (next_page & UI_FIRST_PAGE)){			//һ��ҳ�淭ҳ
		debug(WARN,"һ��ҳ�淭ҳ");
		Fill_Block_A(43,95,40,16,WHITE);
		return;
	}else if((ui_page & UI_FIRST_PAGE) && (next_page & UI_SECOND_PAGE)){		//�������ҳ��
		debug(WARN,"�Զ���ҳ��");
		Fill_Block_A(3,51,12,16,WHITE);					//���Һ�����ġ���������
		Fill_Block_A(111,51,12,16,WHITE);
		Fill_Block_A(51,116,24,8,WHITE);
		Fill_Block_A(111,0,16,16,WHITE);				//���
		Fill_Block_A(1,2,30,12,WHITE);					//ʱ��
		Fill_Block_A(43,95,40,16,WHITE);				//�����ã�ģʽ1...��
		Fill_Block_A(31,21,64,64,WHITE);				//ͼ��
		
		App_Time_Show_Stop();							//����ʱ��͵���ֹͣ��ʾ
	}else if((next_page & UI_FIRST_PAGE) && (ui_page & UI_SECOND_PAGE)){		//����һ��ҳ��
		debug(WARN,"����һ��ҳ��");
		Fill_Block_A(1,110,32,16,WHITE);								//�ײ��Ĺ�����ʾ����
		Fill_Block_A(48,110,32,16,WHITE);
		Fill_Block_A(95,110,32,16,WHITE);
		
		Ui_showPicture(LEFT_ICO,3,51,12,16);
		Ui_showPicture(RIGHT_ICO,111,51,12,16);	
		Ui_showPicture(BATTERY_CHARGING_JPG_100,111,0,16,16);
		
		dateAndTime = Ui_getRTC();
		sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
		debug(WARN,"ʱ�䣺 %s",uiBuf);
		Ui_showString(1,2,uiBuf,12,5);
		
		Ui_showPicture(MORE_ICO,51,116,24,8);

		App_Time_Show_Start();							//�����ʱ����¿�ʼ����
	}else if((ui_page & UI_FIRST_PAGE) && (next_page == UI_HOME_PAGE)){			//��ҳ��homeҳ
		debug(WARN,"��ҳ��homeҳ");
		Fill_Block_A(31,21,64,64,WHITE);
		Fill_Block_A(43,95,16,40,WHITE);
	}
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
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_1_PAGE);
	Ui_showPicture(PATTERY_1_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(43,95,"ģʽ1",16,3);
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
static void Ui_ShowPattern2Page(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_2_PAGE);
	Ui_showPicture(PATTERY_2_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(43,95,"ģʽ2",16,3);
	ui_state.ui_page = UI_PATTERN_2_PAGE;
}
/*******************************************************************************
* ��������  Ui_ShowPattern3Page
* ����������ģʽ2
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
static void Ui_ShowPattern3Page(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_3_PAGE);
	Ui_showPicture(PATTERY_3_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(43,95,"ģʽ3",16,3);
	ui_state.ui_page = UI_PATTERN_3_PAGE;
}
/*******************************************************************************
* ��������  Ui_ShowSetPage
* ��������������
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
static void Ui_ShowSetPage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_SET_PAGE);
	Ui_showPicture(SET_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(47,95,"����",16,2);
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
static void Ui_FilePage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_FILE_PAGE);
	Ui_showPicture(FILE_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(47,95,"�ļ�",16,2);
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
static void Ui_DetectionPage(u8 pattern){
	detect_t * DetectPattern;
	DetectPattern = get_DetectStatus();
	switch (pattern){
		case 1:
			Ui_ShowInfoPage(ui_state.ui_page,UI_DETECTION1_PAGE);
			ui_state.ui_page = UI_DETECTION1_PAGE;
			DetectPattern->detect_pattern = pattern_one;
			break;
		case 2:
			Ui_ShowInfoPage(ui_state.ui_page,UI_DETECTION2_PAGE);
			ui_state.ui_page = UI_DETECTION2_PAGE;
			DetectPattern->detect_pattern = pattern_two;
			break;
		case 3:
			Ui_ShowInfoPage(ui_state.ui_page,UI_DETECTION2_PAGE);
			ui_state.ui_page = UI_DETECTION2_PAGE;
			DetectPattern->detect_pattern = pattern_three;
			break;
	}
	DetectPattern->detect_status = next_start;
	Ui_showPicture(LOADING_ICO,31,21,64,64);//��ʾͼ��
	Ui_showString(1,110,"��ʼ",16,2);
	Ui_showString(48,110,"ֹͣ",16,2);
	Ui_showString(95,110,"����",16,2);
	App_detect_Init();
}
/*******************************************************************************
* ��������  Ui_SetSubPage
* ����������������ҳ
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
static void Ui_SetSubPage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_SET_SUB_PAGE);
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
	Ui_showString(1,110,"����",16,2);
	Ui_showString(95,110,"����",16,2);
	ui_state.ui_page = UI_SET_SUB_PAGE;
}



/*******************************************************************************
* ��������  Ui_Detect_1_Page
* �������������1
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
void Ui_Detecting_Page(u8 pattern){
	detect_t * DetectPattern = get_DetectStatus();
	switch (DetectPattern->detect_status){
		case next_start:
			debug(INFO,"���룬����ʼ");
			DetectPattern->detect_status = start;
			Ui_showString(1,110,"��ͣ",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
		case end:
			debug(INFO,"�����������¿�ʼ");
			DetectPattern->detect_status = next_start;
			Ui_showString(1,110,"��ʼ",16,2);
			break;
		case detecting:						//����У����������ͣ����ʾ��������
			debug(INFO,"��ͣ");
			DetectPattern->detect_status = suspend;
			Ui_showString(1,110,"����",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
		case stop:							//δ��ʼ�����������ʼ����ʾ����ͣ��
			debug(INFO,"��ʼ");
			DetectPattern->detect_status = start;
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			Ui_showString(1,110,"��ͣ",16,2);
			break;
		case suspend:						//��ͣ�����������������ʾ����ͣ��
			debug(INFO,"����");
			DetectPattern->detect_status = continu;
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			Ui_showString(1,110,"��ͣ",16,2);
			break;
		case continu:						//����У����������ͣ����ʾ��������
			debug(INFO,"��ͣ");
			DetectPattern->detect_status = suspend;
			Ui_showString(1,110,"����",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
	}
}

/*******************************************************************************
* ��������  Ui_Detect_1_Page
* �������������1
* ���ߣ�    Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
*******************************************************************************/
static void Ui_Detect_Stop(u8 pattern){
	detect_t * DetectPattern = get_DetectStatus();
	switch (pattern){
		case 1:
			switch (DetectPattern->detect_status){
				case detecting:						//����У����������ͣ����ʾ��������
					debug(INFO,"ֹͣ");
					DetectPattern->detect_status = stop;
					Ui_showString(1,110,"��ʼ",16,2);
					xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
					break;
				case suspend:						//��ͣ�����������������ʾ����ͣ��
					debug(INFO,"ֹͣ");
					DetectPattern->detect_status = stop;
					xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
					Ui_showString(1,110,"��ʼ",16,2);
					break;
				}
			break;
		case 2:

			break;
		case 3:

			break;
	}
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
//	EventBits_t EventValue;
//	
//	EventValue=xEventGroupWaitBits((EventGroupHandle_t	)ui_event,		//�ȴ��ź�������,�ȴ�page��λ 
//							   (EventBits_t			)UI_ALL_PAGE,
//							   (BaseType_t			)pdTRUE,				
//							   (BaseType_t			)pdFALSE,
//							   (TickType_t			)portMAX_DELAY);
	
//	debug(WARN,"UI�¼� %d",EventValue);
	u8 i;
	BaseType_t err=pdFALSE;
	err=xSemaphoreTake(ui_Sem,portMAX_DELAY);	//��ȡ�ź���
	if((err==pdTRUE) && (ui_Event!= 0))									//��ȡ�ź����ɹ�
	{
		for(i=0;i<UI_PAGE_SIZE;i++){
			if(ui_Event & (1<<i)){
					switch(1<<i){
						case UI_NO_PAGE:
							Ui_delPage(UI_NO_PAGE);
							break;
						case UI_HOME_PAGE:			//��ҳ
							Ui_delPage(UI_HOME_PAGE);
							Ui_ShowMainPage();
							break;
						case UI_PATTERN_1_PAGE:		//ģʽ1
							Ui_delPage(UI_PATTERN_1_PAGE);
							Ui_ShowPattern1Page();
							break;
						case UI_PATTERN_2_PAGE:		//ģʽ2
							Ui_delPage(UI_PATTERN_2_PAGE);
							Ui_ShowPattern2Page();
							break;
						case UI_PATTERN_3_PAGE:		//ģʽ3
							Ui_delPage(UI_PATTERN_3_PAGE);
							Ui_ShowPattern3Page();
							break;
						case UI_SET_PAGE:			//����
							Ui_delPage(UI_SET_PAGE);
							Ui_ShowSetPage();
							break;
						case UI_FILE_PAGE:			//�ļ�
							Ui_delPage(UI_FILE_PAGE);
							Ui_FilePage();
							break;
						case UI_DETECTION1_PAGE:	//���ģʽ1
							Ui_delPage(UI_DETECTION1_PAGE);
							Ui_DetectionPage(1);
							break;
						case UI_DETECTION2_PAGE:	//���ģʽ2
							Ui_delPage(UI_DETECTION2_PAGE);
							Ui_DetectionPage(2);
							break;
						case UI_DETECTION3_PAGE:	//���ģʽ3
							Ui_delPage(UI_DETECTION3_PAGE);
							Ui_DetectionPage(3);
							break;
						case UI_DETECT_1_PAGE:		//��ʼ���ģʽ1
							Ui_delPage(UI_DETECT_1_PAGE);
							Ui_Detecting_Page(1);
							break;
						case UI_DETECT_2_PAGE:		//��ʼ���ģʽ2
							Ui_delPage(UI_DETECT_2_PAGE);
							Ui_Detecting_Page(2);
							break;
						case UI_DETECT_3_PAGE:		//��ʼ���ģʽ3
							Ui_delPage(UI_DETECT_3_PAGE);
							Ui_Detecting_Page(3);
							break;
						case UI_PATTERN_1_PAGE_MENU:	//�ټ��ģʽ��menu��
							Ui_delPage(UI_PATTERN_1_PAGE_MENU);
							Ui_Detect_Stop(1);
							break;
						case UI_SET_SUB_PAGE:		//������Ҷ
							Ui_delPage(UI_SET_SUB_PAGE);
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
		}
	}
}



/***********************************************************************************************************************
***********************************************************************************************************************/


