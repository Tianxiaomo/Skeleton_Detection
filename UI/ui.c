
/***********************************************************************************************************************
Skeleton_Detection
文件名:        ui.c
描述   :       管理ui显示

版本   :       V1.0
修改   :   
完成日期：     2016.10.14



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
TimerHandle_t 	   ui_timer;	//ui软件定时器
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
* 函数名：   Ui_timerHandler,软定时器中断函数
* 功能描述： 软定时中断，关闭软定时器，power_off UI,设置ui_page为UI_NO_PAGE
* 作者：     Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
void Ui_timerHandler(TimerHandle_t xTimer) 
{
  ui_state.ui_power = 0;        //关掉ui
  ui_state.ui_page = UI_NO_PAGE;//设置ui_page为UI_NO_PAGE
  //设置屏为低功耗,关掉屏
  // OLED_displayOff();
  Ui_clearScreen();	
}
/*******************************************************************************
* 函数名：  Ui_init
* 功能描述：ui初始化
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_init(void)
{
	ui_event=xEventGroupCreate();	 		//创建UI事件标志组
	ui_timer=xTimerCreate((const char*		)"AutoReloadTimer",
						(TickType_t			)6000,
						(UBaseType_t		)pdFALSE,
						(void*				)1,
						(TimerCallbackFunction_t)Ui_timerHandler); //周期定时器，周期6s(6000个时钟节拍)，单次模式
  //UI线程软定时器,定时周期为6s，定时时间到就关闭屏,注意该定时时间需要大于等于5s,目的是放置测量心率、血压时会暗屏
}
/*******************************************************************************
* 函数名：   Ui_startSoftTimer
* 功能描述： 打开软定时器，定时时间到，进入软定时中断
* 作者：     Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
void Ui_startSoftTimer(void)
{
	xTimerStart(ui_timer,0);			//启动定时器
}

/*******************************************************************************
* 函数名：  ui_stateSet
* 功能描述：设置ui的状态
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_stateSet(u32 ui_page,u8 ui_power)
{
	ui_state.ui_page = ui_page;
	ui_state.ui_power = ui_power;
}

/*******************************************************************************
* 函数名：  ui_stateGet
* 功能描述：获取ui的状态
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
ui_state_t *Ui_stateGet(void)
{
  return &ui_state;
}
/********************************************************************************
* 函数名：  Ui_postPage
* 功能描述：置页,
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
***********************************************************************************/
void Ui_postPage(u32 page)	 
{
	xEventGroupSetBits(ui_event,page);
}
/**********************************************************************************
* 函数名：  Ui_clearScreen
* 功能描述：清屏
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************/
void Ui_clearScreen(void)
{
//  OLED_clear();
}
/*************************************************************************************************************
* 函数名：  Ui_getRTC
* 功能描述：获取RTC实时时间
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
dateAndTime_t* Ui_getRTC(void)	 
{ 
  return RTC_getDateAndTime();
}
/*************************************************************************************************************
* 函数名：  Ui_getTempHumi
* 功能描述：获取温湿度
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
//TempHumiValue_t* Ui_getTempAndHumi(void)	 
//{ 
//  return SHT20_readTemAndHum();
//}
/*************************************************************************************************************
* 函数名：  Ui_showString
* 功能描述：显示字符串
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
void Ui_showString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 Char_Num)
{
	Show_Str(x,y,Char_Size*Char_Num,Char_Size,chr,Char_Size,1,TIME_COLOR,BACKGROUD_COLOR,draw_point);
}
/*************************************************************************************************************
* 函数名：  Ui_showPicture
* 功能描述：显示图片
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/
void Ui_showPicture(const u8 *filename,u8 x,u8 y,u8 w,u8 h)
{
	ai_load_picfile(filename,x,y,w,h,1);
}
/*******************************************************************************
* 函数名：  ui_MainPage
* 功能描述：显示时间界面
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
static void Ui_ShowMainPage(void)
{
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//时间&日期
	batteryState_t *batteryState;     //获取电池状态
	
	Ui_clearScreen(); 
	Ui_stateSet(UI_HOME_PAGE,UI_POWER_ON);
	
	debug(WARN,"显示主页");
	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);
	Ui_showPicture(LOGO_ICO,13,18,100,38);						//logo显示
	Ui_showPicture(LEFT_ICO,3,51,12,16);
	Ui_showPicture(RIGHT_ICO,111,51,12,16);	
	
	/*******************<电池电量显示逻辑>*************************/	
	batteryState = Battery_getStatus();
	if( BATTERY_CHARGE_ING == batteryState->chargeState ) //电池充电中
	{
		switch(batteryState->batteryRemain){
			case BATTERY_REMAIN_100:
				ai_load_picfile(BATTERY_CHARGING_JPG_100,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_90:
				ai_load_picfile(BATTERY_CHARGING_JPG_90,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_80:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_70:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_60:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_50:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_40:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_30:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_20:
				ai_load_picfile(BATTERY_CHARGING_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_10:
				ai_load_picfile(BATTERY_CHARGING_JPG_80,111,0,16,16,1);//显示图标
				break;
			default:
				break; 
		}
	}
	else if( BATTERY_CHARGE_FINISH == batteryState->chargeState ) //电池完成
	{
		ai_load_picfile(BATTERY_CHARGING_JPG_100,0,0,100,100,1);//显示图标  //充满电，显示100%
	}
	else   //BATTERY_DIS_CHARGE == batteryState->chargeState  //电池放电中
	{
		switch(batteryState->batteryRemain){
			case BATTERY_REMAIN_100:
				ai_load_picfile(BATTERY_JPG_100,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_90:
				ai_load_picfile(BATTERY_JPG_90,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_80:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_70:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_60:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_50:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_40:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_30:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_20:
				ai_load_picfile(BATTERY_JPG_10,111,0,16,16,1);//显示图标
				break;
			case BATTERY_REMAIN_10:
				ai_load_picfile(BATTERY_JPG_80,111,0,16,16,1);//显示图标
				break;
			default:
				break; 
		}
	}	
	/*******************<获取RTC时间日期，并显示>*************************/	
	dateAndTime = Ui_getRTC();
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	
	debug(WARN,"时间： %s",uiBuf);
	
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
* 函数名：ui_NoPage
* 功能描述：关闭ui 
* 作者：Momo  
* 参数说明：  
* 返回值说明:
* 修改记录：
*******************************************************************************/
static void Ui_NoPage(void)
{
//  OLED_clear();   
}

/*******************************************************************************
* 函数名：  Ui_ShowPattern1Page
* 功能描述：模式1
* 作者：    Momo  
* 参数说明：  
* 返回值说明:
* 修改记录：
*******************************************************************************/
static void Ui_ShowPattern1Page(void){
	Ui_showPicture(PATTERY_1_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_PATTERN_1_PAGE;
}

/*******************************************************************************
* 函数名：  Ui_ShowPattern2Page
* 功能描述：模式2
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_ShowPattern2Page(void){
	Ui_showPicture(PATTERY_2_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_PATTERN_2_PAGE;
}

/*******************************************************************************
* 函数名：  Ui_ShowSetPage
* 功能描述：设置
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_ShowSetPage(void){
	Ui_showPicture(SET_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_SET_PAGE;
}

/*******************************************************************************
* 函数名：  Ui_FilePage
* 功能描述：文件
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_FilePage(void){
	Ui_showPicture(FILE_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_FILE_PAGE;
}

/*******************************************************************************
* 函数名：  Ui_Detection1Page
* 功能描述：检测
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_Detection1Page(void){
	Ui_showPicture(LOADING_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_DETECTION1_PAGE;
}
/*******************************************************************************
* 函数名：  Ui_Detection2Page
* 功能描述：检测
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_Detection2Page(void){
	Ui_showPicture(LOADING_ICO,31,21,64,64);//显示图标
	ui_state.ui_page = UI_DETECTION2_PAGE;
}
/*******************************************************************************
* 函数名：  Ui_SetSubPage
* 功能描述：设置子页
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_SetSubPage(void){
	Ui_showPicture(TIME_ICO,5,5,16,16);//显示图标
	Ui_showString(31,5,"检测时间",16,4);
	Ui_showPicture(CLOCK_ICO,5,25,16,16);//显示图标
	Ui_showString(31,25,"时间日期",16,4);
	Ui_showPicture(LANG_ICO,5,45,16,16);//显示图标
	Ui_showString(31,45,"系统语言",16,4);
	Ui_showPicture(CONTENT_ICO,5,65,16,16);//显示图标
	Ui_showString(31,65,"存储容量",16,4);
	Ui_showPicture(ABOUT_ICO,5,85,16,16);//显示图标
	Ui_showString(31,85,"关于",16,2);
	ui_state.ui_page = UI_SET_SUB_PAGE;
}

/*******************************************************************************
* 函数名：  ui_poll
* 功能描述：ui轮循 
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/

void Ui_poll(void)
{
	EventBits_t EventValue;
	
	EventValue=xEventGroupWaitBits((EventGroupHandle_t	)ui_event,		//等待信号量触发,等待page置位 
							   (EventBits_t			)UI_ALL_PAGE,
							   (BaseType_t			)pdTRUE,				
							   (BaseType_t			)pdFALSE,
							   (TickType_t			)portMAX_DELAY);
	
	debug(WARN,"UI事件 %d",EventValue);
	
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


