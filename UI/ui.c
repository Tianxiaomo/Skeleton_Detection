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
SemaphoreHandle_t ui_Sem;	//二值信号量句柄
long long ui_Event;
TimerHandle_t 	   ui_timer;	//ui软件定时器
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
//	ui_event=xEventGroupCreate();	 		//创建UI事件标志组
	ui_Sem = xSemaphoreCreateBinary();	
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
//void Ui_postPage(u32 page)	 
//{
//	xEventGroupSetBits(ui_event,page);
//}

void Ui_postPage(long long page){
	xSemaphoreGive(ui_Sem);	//释放二值信号量
	ui_Event |= page;
}

/********************************************************************************
* 函数名：  Ui_delPage
* 功能描述：置页,
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
***********************************************************************************/
void Ui_delPage(long long page){
	ui_Event &= ~page;
}
/**********************************************************************************
* 函数名：  Ui_clearScreen
* 功能描述：清屏
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************/
void Ui_clearScreen(void){
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
dateAndTime_t* Ui_getRTC(void)	 { 
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
/*******************************************************************************
* 函数名：  Ui_showString
* 功能描述：显示字符串
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*********************************************************************************/
void Ui_showString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 Char_Num){
	Show_Str(x,y,Char_Size*Char_Num,Char_Size,chr,Char_Size,1,TIME_COLOR,BACKGROUD_COLOR,draw_point);
}
/********************************************************************************
* 函数名：  Ui_showPicture
* 功能描述：显示图片
* 作者：    Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*********************************************************************************/
void Ui_showPicture(const u8 *filename,u8 x,u8 y,u8 w,u8 h){
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
static void Ui_ShowMainPage(void){
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//时间&日期
	batteryState_t *batteryState;     //获取电池状态
	
	Ui_ShowInfoPage(ui_state.ui_page,UI_HOME_PAGE);
	Ui_stateSet(UI_HOME_PAGE,UI_POWER_ON);

	debug(WARN,"显示主页");
	Ui_showPicture(LOGO_ICO,13,18,100,38);						//logo显示
	Ui_showPicture(LEFT_ICO,3,51,12,16);
	Ui_showPicture(RIGHT_ICO,111,51,12,16);	
	Ui_showPicture(MORE_ICO,51,116,24,8);
	
	/*******************<电池电量显示逻辑>*************************/	
	batteryState = Battery_getStatus();
	Ui_showPicture(BATTERY_CHARGING_JPG_100,111,0,16,16);
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
	debug(WARN,"时间： %s	日期：20%d-%d-%d %d",uiBuf,dateAndTime->year,dateAndTime->month,dateAndTime->date,dateAndTime->week);
	Ui_showString(23,56,uiBuf,32,5);
	
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	debug(WARN,"时间： %s",uiBuf);
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
* 函数名：ui_NoPage
* 功能描述：关闭ui 
* 作者：Momo  
* 参数说明：  
* 返回值说明:
* 修改记录：
*******************************************************************************/
static void Ui_NoPage(void){
//  OLED_clear();   
}
/*******************************************************************************
* 函数名：  Ui_ShowInfoPage
* 功能描述：非主页的其他一级页面的时钟和电量显示
* 作者：    Momo  
* 参数说明：  
* 返回值说明:
* 修改记录：
*******************************************************************************/
static void Ui_ShowInfoPage(u32 ui_page,u32 next_page){
	u8 uiBuf[40];
	dateAndTime_t *dateAndTime; 		//时间&日期
	batteryState_t *batteryState;     //获取电池状态
		
	debug(WARN,"显示信息页面 %d -> %d",ui_page,next_page);
//	debug(ERROR,"first %d ,second %d",UI_FIRST_PAGE,UI_SECOND_PAGE);
//	debug(WARN,"显 %d && %d = %d",(ui_page & UI_FIRST_PAGE),(next_page & UI_FIRST_PAGE),(ui_page & UI_FIRST_PAGE) && (next_page & UI_FIRST_PAGE));
	if(UI_HOME_PAGE == ui_page){												//从home页翻出
		debug(WARN,"从home页翻出");
		Fill_Block_A(13,18,100,38,WHITE);				//logo擦出	
		Fill_Block_A(23,56,80,32,WHITE);				//时间擦出
		Fill_Block_A(25,90,90,12,WHITE);				//日期擦除
	}else if((ui_page & UI_FIRST_PAGE) && (next_page & UI_FIRST_PAGE)){			//一级页面翻页
		debug(WARN,"一级页面翻页");
		Fill_Block_A(43,95,40,16,WHITE);
		return;
	}else if((ui_page & UI_FIRST_PAGE) && (next_page & UI_SECOND_PAGE)){		//进入二级页面
		debug(WARN,"显二级页面");
		Fill_Block_A(3,51,12,16,WHITE);					//左右和下面的。。。擦除
		Fill_Block_A(111,51,12,16,WHITE);
		Fill_Block_A(51,116,24,8,WHITE);
		Fill_Block_A(111,0,16,16,WHITE);				//电池
		Fill_Block_A(1,2,30,12,WHITE);					//时间
		Fill_Block_A(43,95,40,16,WHITE);				//“设置，模式1...”
		Fill_Block_A(31,21,64,64,WHITE);				//图标
		
		App_Time_Show_Stop();							//界面时间和电量停止显示
	}else if((next_page & UI_FIRST_PAGE) && (ui_page & UI_SECOND_PAGE)){		//返回一级页面
		debug(WARN,"返回一级页面");
		Fill_Block_A(1,110,32,16,WHITE);								//底部的功能提示擦除
		Fill_Block_A(48,110,32,16,WHITE);
		Fill_Block_A(95,110,32,16,WHITE);
		
		Ui_showPicture(LEFT_ICO,3,51,12,16);
		Ui_showPicture(RIGHT_ICO,111,51,12,16);	
		Ui_showPicture(BATTERY_CHARGING_JPG_100,111,0,16,16);
		
		dateAndTime = Ui_getRTC();
		sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
		debug(WARN,"时间： %s",uiBuf);
		Ui_showString(1,2,uiBuf,12,5);
		
		Ui_showPicture(MORE_ICO,51,116,24,8);

		App_Time_Show_Start();							//界面的时间从新开始更新
	}else if((ui_page & UI_FIRST_PAGE) && (next_page == UI_HOME_PAGE)){			//翻页到home页
		debug(WARN,"翻页到home页");
		Fill_Block_A(31,21,64,64,WHITE);
		Fill_Block_A(43,95,16,40,WHITE);
	}
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
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_1_PAGE);
	Ui_showPicture(PATTERY_1_ICO,31,21,64,64);//显示图标
	Ui_showString(43,95,"模式1",16,3);
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
static void Ui_ShowPattern2Page(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_2_PAGE);
	Ui_showPicture(PATTERY_2_ICO,31,21,64,64);//显示图标
	Ui_showString(43,95,"模式2",16,3);
	ui_state.ui_page = UI_PATTERN_2_PAGE;
}
/*******************************************************************************
* 函数名：  Ui_ShowPattern3Page
* 功能描述：模式2
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
static void Ui_ShowPattern3Page(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_PATTERN_3_PAGE);
	Ui_showPicture(PATTERY_3_ICO,31,21,64,64);//显示图标
	Ui_showString(43,95,"模式3",16,3);
	ui_state.ui_page = UI_PATTERN_3_PAGE;
}
/*******************************************************************************
* 函数名：  Ui_ShowSetPage
* 功能描述：设置
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
static void Ui_ShowSetPage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_SET_PAGE);
	Ui_showPicture(SET_ICO,31,21,64,64);//显示图标
	Ui_showString(47,95,"设置",16,2);
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
static void Ui_FilePage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_FILE_PAGE);
	Ui_showPicture(FILE_ICO,31,21,64,64);//显示图标
	Ui_showString(47,95,"文件",16,2);
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
	Ui_showPicture(LOADING_ICO,31,21,64,64);//显示图标
	Ui_showString(1,110,"开始",16,2);
	Ui_showString(48,110,"停止",16,2);
	Ui_showString(95,110,"返回",16,2);
	App_detect_Init();
}
/*******************************************************************************
* 函数名：  Ui_SetSubPage
* 功能描述：设置子页
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
static void Ui_SetSubPage(void){
	Ui_ShowInfoPage(ui_state.ui_page,UI_SET_SUB_PAGE);
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
	Ui_showString(1,110,"设置",16,2);
	Ui_showString(95,110,"返回",16,2);
	ui_state.ui_page = UI_SET_SUB_PAGE;
}



/*******************************************************************************
* 函数名：  Ui_Detect_1_Page
* 功能描述：检测1
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
void Ui_Detecting_Page(u8 pattern){
	detect_t * DetectPattern = get_DetectStatus();
	switch (DetectPattern->detect_status){
		case next_start:
			debug(INFO,"进入，将开始");
			DetectPattern->detect_status = start;
			Ui_showString(1,110,"暂停",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
		case end:
			debug(INFO,"结束，将重新开始");
			DetectPattern->detect_status = next_start;
			Ui_showString(1,110,"开始",16,2);
			break;
		case detecting:						//检测中，按左键，暂停，显示“继续”
			debug(INFO,"暂停");
			DetectPattern->detect_status = suspend;
			Ui_showString(1,110,"继续",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
		case stop:							//未开始，按左键，开始，显示“暂停”
			debug(INFO,"开始");
			DetectPattern->detect_status = start;
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			Ui_showString(1,110,"暂停",16,2);
			break;
		case suspend:						//暂停，按左键，继续，显示“暂停”
			debug(INFO,"继续");
			DetectPattern->detect_status = continu;
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			Ui_showString(1,110,"暂停",16,2);
			break;
		case continu:						//检测中，按左键，暂停，显示“继续”
			debug(INFO,"暂停");
			DetectPattern->detect_status = suspend;
			Ui_showString(1,110,"继续",16,2);
			xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
			break;
	}
}

/*******************************************************************************
* 函数名：  Ui_Detect_1_Page
* 功能描述：检测1
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/
static void Ui_Detect_Stop(u8 pattern){
	detect_t * DetectPattern = get_DetectStatus();
	switch (pattern){
		case 1:
			switch (DetectPattern->detect_status){
				case detecting:						//检测中，按左键，暂停，显示“继续”
					debug(INFO,"停止");
					DetectPattern->detect_status = stop;
					Ui_showString(1,110,"开始",16,2);
					xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
					break;
				case suspend:						//暂停，按左键，继续，显示“暂停”
					debug(INFO,"停止");
					DetectPattern->detect_status = stop;
					xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
					Ui_showString(1,110,"开始",16,2);
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
* 函数名：  ui_poll
* 功能描述：ui轮循 
* 作者：    Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
*******************************************************************************/

void Ui_poll(void)
{
//	EventBits_t EventValue;
//	
//	EventValue=xEventGroupWaitBits((EventGroupHandle_t	)ui_event,		//等待信号量触发,等待page置位 
//							   (EventBits_t			)UI_ALL_PAGE,
//							   (BaseType_t			)pdTRUE,				
//							   (BaseType_t			)pdFALSE,
//							   (TickType_t			)portMAX_DELAY);
	
//	debug(WARN,"UI事件 %d",EventValue);
	u8 i;
	BaseType_t err=pdFALSE;
	err=xSemaphoreTake(ui_Sem,portMAX_DELAY);	//获取信号量
	if((err==pdTRUE) && (ui_Event!= 0))									//获取信号量成功
	{
		for(i=0;i<UI_PAGE_SIZE;i++){
			if(ui_Event & (1<<i)){
					switch(1<<i){
						case UI_NO_PAGE:
							Ui_delPage(UI_NO_PAGE);
							break;
						case UI_HOME_PAGE:			//主页
							Ui_delPage(UI_HOME_PAGE);
							Ui_ShowMainPage();
							break;
						case UI_PATTERN_1_PAGE:		//模式1
							Ui_delPage(UI_PATTERN_1_PAGE);
							Ui_ShowPattern1Page();
							break;
						case UI_PATTERN_2_PAGE:		//模式2
							Ui_delPage(UI_PATTERN_2_PAGE);
							Ui_ShowPattern2Page();
							break;
						case UI_PATTERN_3_PAGE:		//模式3
							Ui_delPage(UI_PATTERN_3_PAGE);
							Ui_ShowPattern3Page();
							break;
						case UI_SET_PAGE:			//设置
							Ui_delPage(UI_SET_PAGE);
							Ui_ShowSetPage();
							break;
						case UI_FILE_PAGE:			//文件
							Ui_delPage(UI_FILE_PAGE);
							Ui_FilePage();
							break;
						case UI_DETECTION1_PAGE:	//检测模式1
							Ui_delPage(UI_DETECTION1_PAGE);
							Ui_DetectionPage(1);
							break;
						case UI_DETECTION2_PAGE:	//检测模式2
							Ui_delPage(UI_DETECTION2_PAGE);
							Ui_DetectionPage(2);
							break;
						case UI_DETECTION3_PAGE:	//检测模式3
							Ui_delPage(UI_DETECTION3_PAGE);
							Ui_DetectionPage(3);
							break;
						case UI_DETECT_1_PAGE:		//开始检测模式1
							Ui_delPage(UI_DETECT_1_PAGE);
							Ui_Detecting_Page(1);
							break;
						case UI_DETECT_2_PAGE:		//开始检测模式2
							Ui_delPage(UI_DETECT_2_PAGE);
							Ui_Detecting_Page(2);
							break;
						case UI_DETECT_3_PAGE:		//开始检测模式3
							Ui_delPage(UI_DETECT_3_PAGE);
							Ui_Detecting_Page(3);
							break;
						case UI_PATTERN_1_PAGE_MENU:	//再检测模式按menu键
							Ui_delPage(UI_PATTERN_1_PAGE_MENU);
							Ui_Detect_Stop(1);
							break;
						case UI_SET_SUB_PAGE:		//设置子叶
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


