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
//#include "protocol.h"
//#include "motor.h"
//#include "power.h"
#include "oled.h"
//#include "battery.h"
//#include "pedometer.h"
//#include "watchInfo.h"
//#include "hp6.h"
#include "battery.h"
//#include "ui.h"
#include "delay.h"
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
	key_t * key;  
	ui_state_t ui_state; 
	ui_state = Ui_stateGet();                         //获取ui的状态
	if( UI_POWER_ON == ui_state.ui_power )           //屏的状态为亮
	{
		key = Key_getKey();                           	//获取按键状态
		switch(key->key_state){
//			case KEY_SHORT_PRESS:                           //短按
//				switch(key->key_id){
//					case KEY_HOME_ID:
//					ui_state->ui_page = ui_state->ui_page<<1; //下一页
//					if(ui_state->ui_page > UI_LAST_PAGE)		//如果大于于最后一页
//					{
//					ui_state->ui_page = UI_FIRST_PAGE;		//切换到最后一页
//					}


//					//APP打开心率血压测量时，禁止置页，保证程序整体逻辑性
//					if(PROTOCOL_OPEN_PULSE_BP == Protocol_getPulseBPOpenState())
//					{
//					//do nothing 
//					}
//					else  //PROTOCOL_CLOSE_PULSE_BP == Protocol_getPulseBPOpenState()
//					{
//					Ui_postPage(ui_state->ui_page);		  		//触发UI页
//					}


//					break;
//					default:
//					//do nothing
//					break;
//				}
				break;
			case KEY_LONG_PRESS:                            	//长按
				switch(key->key_id){							//长按右键，息屏
					case KEY_RIGHT_ID:
						xEventGroupSetBits(app_event,APP_OLED_DOWN_EVENT);
						break;
					case KEY_MENU_ID:							//如果不在home,到home页面
						if(UI_HOME_PAGE != ui_state.ui_page)
							xEventGroupSetBits(app_event,APP_HOME_EVENT);
						break;
				}
				break;
			default:
			//do nothing
				break;
		}
		Key_clearState(key);						//清除按键状态	
	}
  else if( UI_POWER_OFF == ui_state.ui_power )     //屏的状态为熄灭
  {
//    key = Key_getKey();                           	//获取按键状态
//    switch(key->key_state)
//    {
//    case KEY_SHORT_PRESS:                         //短按	
//      //初始化屏
//      ui_state->ui_page = UI_HOME_PAGE;           //设置main_page
//      ui_state->ui_power = UI_POWER_ON;           //屏的电源打开
//      Ui_postPage(ui_state->ui_page);             //触发UI页	
//      Key_clearState(key);                        //清除按键状态				
//      break;
//    case KEY_LONG_PRESS:
//      switch(key->key_id)
//      {
//      case KEY_HOME_ID:
//        App_postEvent(APP_POWER_DOWN_ENENT);
//        break;
//      }
//      Key_clearState(key);                          //清除按键状态	
//      break;
//    }
  }
}
/**********************************************************************************************************
* 函数名：       App_alarmHandler
* 功能描述：	  闹钟
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_alarmHandler(void)
{
//  Motor_ON(MOROT_INTENSITY_3,3);  //开机，马达震动3次
}


/**********************************************************************************************************
* 函数名：       App_powerOnHandler
* 功能描述：	  切换ui
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_powerOnHandler(void)
{
//  Motor_ON(MOROT_INTENSITY_3,1);  //开机，马达震动1次
//  Pedometer_startSoftTimer();//启动app_timer ,移到MPU6050读ID函数中
//  Ui_showAdvertPage();             //开机广告
//  delay_ms(2000);
//  Ui_postPage(UI_HOME_PAGE);
//  
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
//  OLED_displayOff();
//  Hp_6_PowerOFF();
//  Pedometer_stopSoftTimer();
//  Battery_stopSoftTimer();
//  Motor_ON(MOROT_INTENSITY_3,1);	//关机，马达震动1次
//  Power_down();  //待机模式
}
/**********************************************************************************************************
* 函数名：       App_batteryDetectHandler
* 功能描述：	  电池电量检测
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_batteryDetectHandler(void)
{
  batteryState_t *batteryState;
  batteryState = Battery_getState();
  //Momo 屏蔽着两句为了避免充电时刷屏，屏蔽后，功能效果为:插上充电下不会立即显示充电UI，而是按按键才显示
 // if(BATTERY_CHARGE_ING == batteryState->chargeState)   //如果检测到充电，立即出发UI主页，显示正在充电
 // 	Ui_postPage(UI_HOME_PAGE); //显示UI主页
}

/***********************************************************************************************************************
***********************************************************************************************************************/
