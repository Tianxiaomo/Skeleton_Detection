/***********************************************************************************************************************
Skeleton_Detection
文件名:        app_handler.c
描述   :       完成一些相关的处理

版本   :       V1.0
修改   :   
完成日期： 
************************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 */
#include "app_handler.h"
#include "key.h"
#include "led.h"
#include "ui.h"
#include "app_task.h"
#include "oled.h"
#include "rtc.h"
#include "battery.h"
#include "delay.h"
#include "log.h"
#include "ad7192.h"
#include "fattester.h"
#include "adc.h"
/***********************************************************************************************************************
 * CONSTANTS
 */

/***********************************************************************************************************************
 * TYPEDEFS
 */

/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
 
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */
//TimerHandle_t 	   detect_timer;			//
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */

/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */

/**********************************************************************************************************
* 函数名：        App_keyHandler
* 功能描述：      按键处理
* 参数说明：      none
* 返回值说明：    none
* 修改记录： 
**********************************************************************************************************/
void App_keyHandler(void)
{
	debug(WARN,"收到按键事件");
	key_t * key;  
	ui_state_t *ui_state; 
	ui_state = Ui_stateGet();                         	//获取ui的状态
	key = Key_getKey();                           		//获取按键状态
	debug(WARN,"ui page : %d,ui state : %d",ui_state->ui_page,ui_state->ui_power);
	
	if( UI_POWER_ON == ui_state->ui_power )           //屏的状态为亮
	{
		if(KEY_SHORT_PRESS == key->key_state)				//短按
		{	
			switch(key->key_id){
				case KEY_MENU_ID:						//按MENU
					switch(ui_state->ui_page){
		/*************************一级页面，进入二级************************************************/	
						case UI_HOME_PAGE:
							
							break;
						case UI_PATTERN_1_PAGE:
							Ui_postPage(UI_DETECTION1_PAGE);
							break;
						case UI_PATTERN_2_PAGE:
							Ui_postPage(UI_DETECTION2_PAGE);
							break;
						case UI_PATTERN_3_PAGE:
							Ui_postPage(UI_DETECTION3_PAGE);
							break;
						case UI_SET_PAGE:
							Ui_postPage(UI_SET_SUB_PAGE);
							break;
						case UI_FILE_PAGE:
							Ui_postPage(UI_FILE_SUB_PAGE);
							break;
		/*************************二级页面，功能************************************************/	
						case UI_DETECTION1_PAGE:
							Ui_postPage(UI_PATTERN_1_PAGE_MENU);
							break;
						case UI_DETECTION2_PAGE:
							
							break;
						case UI_SET_SUB_PAGE:
							
							break;
						case UI_FILE_SUB_PAGE:
							
							break;
						case UI_SET_TIME_PAGE:
							
							break;
						case UI_SET_LAG_PAGE:
							
							break;
						case UI_SET_SD_PAGE:
							
							break;
					}
					break;
				case KEY_LEFT_ID:						//按左键
					switch(ui_state->ui_page){
		/*************************一级页面，翻页************************************************/				
						case UI_HOME_PAGE:
							Ui_postPage(UI_FILE_PAGE);
							break;
						case UI_PATTERN_1_PAGE:
							Ui_postPage(UI_HOME_PAGE);
							break;
						case UI_PATTERN_2_PAGE:
							Ui_postPage(UI_PATTERN_1_PAGE);
							break;
						case UI_PATTERN_3_PAGE:
							Ui_postPage(UI_PATTERN_2_PAGE);
							break;
						case UI_SET_PAGE:
							Ui_postPage(UI_PATTERN_3_PAGE);
							break;
						case UI_FILE_PAGE:
							Ui_postPage(UI_SET_PAGE);
							break;
		/*************************二级页面，功能************************************************/								
						case UI_DETECTION1_PAGE:
							Ui_postPage(UI_DETECT_1_PAGE);
							break;
						case UI_DETECT_1_PAGE:
							Ui_postPage(UI_DETECT_1_PAGE);
							break;
						case UI_DETECTION2_PAGE:
							Ui_postPage(UI_DETECT_2_PAGE);
							break;
						
						case UI_SET_SUB_PAGE:
							
							break;
						case UI_FILE_SUB_PAGE:
							
							break;
						case UI_SET_TIME_PAGE:
							
							break;
						case UI_SET_LAG_PAGE:
							
							break;
						case UI_SET_SD_PAGE:
							
							break;
					}
					break;
				case KEY_RIGHT_ID:						//按右键
					switch(ui_state->ui_page){
						case UI_HOME_PAGE:
							Ui_postPage(UI_PATTERN_1_PAGE);
							break;
						case UI_PATTERN_1_PAGE:
							Ui_postPage(UI_PATTERN_2_PAGE);
							break;
						case UI_PATTERN_2_PAGE:
							Ui_postPage(UI_PATTERN_3_PAGE);
							break;
						case UI_PATTERN_3_PAGE:
							Ui_postPage(UI_SET_PAGE);						
							break;
						case UI_SET_PAGE:
							Ui_postPage(UI_FILE_PAGE);
							break;
						case UI_FILE_PAGE:
							Ui_postPage(UI_HOME_PAGE);
							break;
				////////////////////////////////////////////////////	
						case UI_DETECTION1_PAGE:
							Ui_postPage(UI_PATTERN_1_PAGE);	
							break;
						case UI_DETECTION2_PAGE:
							Ui_postPage(UI_PATTERN_2_PAGE);
							break;
						case UI_DETECTION3_PAGE:
							Ui_postPage(UI_PATTERN_2_PAGE);
							break;
						case UI_DETECT_1_PAGE:
							Ui_postPage(UI_PATTERN_1_PAGE);
							break;
						case UI_DETECT_2_PAGE:
							Ui_postPage(UI_PATTERN_2_PAGE);
							break;
						case UI_DETECT_3_PAGE:
							Ui_postPage(UI_PATTERN_3_PAGE);
							break;
						case UI_SET_SUB_PAGE:
							Ui_postPage(UI_SET_PAGE);
							break;
						case UI_FILE_SUB_PAGE:
							Ui_postPage(UI_FILE_PAGE);
							break;
						/////////////////////////////////////////
						case UI_SET_TIME_PAGE:
							
							break;
						case UI_SET_LAG_PAGE:
							
							break;
						case UI_SET_SD_PAGE:
							
							break;
					}
					break;
			}
		}
		else if(KEY_LONG_PRESS == key->key_state)				//长按
		{             										
			switch(key->key_id){								//长按右键，息屏
				case KEY_RIGHT_ID:
						xEventGroupSetBits(app_event,APP_OLED_DOWN_EVENT);
					break;
				case KEY_MENU_ID:									//如果不在home,到home页面
					if(UI_HOME_PAGE != ui_state->ui_page)
						Ui_postPage(UI_HOME_PAGE);
					break;
			}
		}
	}
	else if( UI_POWER_OFF == ui_state->ui_power )     	//屏的状态为熄灭
	{
		ui_state->ui_power = UI_POWER_ON;
		debug(WARN,"收到亮屏事件");
		Set_Display_On_Off(UI_POWER_ON);
	}
	Key_clearState(key);								//清除按键状态	
}
/**********************************************************************************************************
* 函数名：       App_alarmHandler
* 功能描述：	  闹钟
* 作者：		  胡广豪  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_timeshowHandler(void)
{
	static float batteryVoltage = 2.1;
	float tempBatteryVoltage;
	u16 adcAverage,batteryVoltageADCVal;
	u8 uiBuf[10];
	float highVoltage = 3.3,lowVoltage = 2.2;
	float interval_Voltage = (highVoltage - lowVoltage) / 10;
	
	dateAndTime_t *dateAndTime;
	debug(WARN,"更新显示时间事件");
	dateAndTime = RTC_getDateAndTime();
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	debug(WARN,"时间： %s",uiBuf);
	Ui_showString(1,2,uiBuf,12,5);
	
	batteryVoltageADCVal = Get_Adc(ADC_CH1); 
	tempBatteryVoltage = batteryVoltageADCVal*(3.3/4096);
	
	if(tempBatteryVoltage<batteryVoltage)   //防止电压飘动，时高时低
	{
		batteryVoltage = tempBatteryVoltage;

        /*电池电压和电池电量的关系*/
        //使用滑动变阻器模拟电池电压会有
		if( batteryVoltage > highVoltage - interval_Voltage && batteryVoltage < highVoltage ){
			Ui_showPicture(BATTERY_JPG_100,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 2*interval_Voltage && batteryVoltage<  highVoltage - interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_90,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 3*interval_Voltage && batteryVoltage<  highVoltage - 2*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_80,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 4*interval_Voltage && batteryVoltage<  highVoltage - 3*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_70,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 5*interval_Voltage && batteryVoltage<  highVoltage - 4*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_60,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 6*interval_Voltage  && batteryVoltage < highVoltage - 5*interval_Voltage){
			Ui_showPicture(BATTERY_JPG_50,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 7*interval_Voltage && batteryVoltage<  highVoltage - 6*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_40,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 8*interval_Voltage && batteryVoltage<  highVoltage - 7*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_30,111,0,16,16);
		}else if( batteryVoltage > highVoltage - 9*interval_Voltage && batteryVoltage<  highVoltage - 8*interval_Voltage ){
			Ui_showPicture(BATTERY_JPG_20,111,0,16,16);
		}else if( batteryVoltage > lowVoltage && batteryVoltage < highVoltage - 9*interval_Voltage){
			Ui_showPicture(BATTERY_JPG_10,111,0,16,16);
		}else{
			
		}
	}
}


/**********************************************************************************************************
* 函数名：       App_powerOnHandler
* 功能描述：	  切换ui
* 作者：		  胡广豪  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_powerOnHandler(void)
{
	debug(WARN,"收到亮屏事件");
	Ui_postPage(UI_HOME_PAGE);
}
/**********************************************************************************************************
* 函数名：        App_powerDownHandler
* 功能描述：      切换ui
 
* 参数说明：      none
* 返回值说明：    none
* 修改记录： 
**********************************************************************************************************/
void App_powerDownHandler(void)
{
	ui_state_t *ui_state; 
	ui_state = Ui_stateGet();                         //获取ui的状态
	ui_state->ui_power = UI_POWER_OFF;
	debug(WARN,"收到息屏事件");
	Set_Display_On_Off(UI_POWER_OFF);
}
/**********************************************************************************************************
* 函数名：       App_batteryDetectHandler
* 功能描述：	  电池电量检测
* 作者：		  胡广豪  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_batteryDetectHandler(void)
{
	debug(WARN,"电池检测事件");
//  batteryState_t *batteryState;
//  batteryState = 
	Battery_getState();
  //胡广豪 屏蔽着两句为了避免充电时刷屏，屏蔽后，功能效果为:插上充电下不会立即显示充电UI，而是按按键才显示
 // if(BATTERY_CHARGE_ING == batteryState->chargeState)   //如果检测到充电，立即出发UI主页，显示正在充电
 // 	Ui_postPage(UI_HOME_PAGE); //显示UI主页
}



/***********************************************************************************************************************
***********************************************************************************************************************/
