/***********************************************************************************************************************
Skeleton_Detection
文件名:        app_task.c
描述   :       主线程

版本   :       V1.0
修改   :   
************************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 */
#include "app_task.h"
#include "FreeRTOS.h"
#include "app_handler.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "oled.h"
#include "ui.h"
#include "rtc.h"
#include "log.h"
#include "malloc.h"
#include "exfuns.h"
#include "ui.h"
//#include "watchInfo.h"
//#include "protocol.h"
//#include "power.h"
//#include "battery.h"
//#include "bluetooth.h"
//#include "WatchDog.h"
/***********************************************************************************************************************
 * CONSTANTS
 */


/***********************************************************************************************************************
 * TYPEDEFS
 */


/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
EventGroupHandle_t app_event;			//事件标志组句柄 
TimerHandle_t 	   app_timer;			//主线程定时器句柄，用于watchdag
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */
void App_timerHandler(TimerHandle_t xTimer);
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */

/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */
/***************************************************************************************************************
* 函数名：App_Init
* 功能描述：初始化
* 作者：Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
****************************************************************************************************************/
void App_init()	 
{
	app_event=xEventGroupCreate();	 //创建事件标志组
	app_timer=xTimerCreate((const char*		)"AutoReloadTimer",
							(TickType_t			)1000,
							(UBaseType_t		)pdTRUE,
							(void*				)1,
							(TimerCallbackFunction_t)App_timerHandler); //周期定时器，周期1s(1000个时钟节拍)，周期模式
	xTimerStart(app_timer,0);			//启动定时器				
	
	LED_Init();						//初始化与LED连接的硬件接口
	Key_Init();						//初始化按键
	RTC_Init();						//RTC初始化
	OLED_Init();					//初始化OLED屏
	piclib_init();					//初始化画图
	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);		
 	debug(DEBUG,"oled ok");
	Ui_init();
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	while(SD_Init())				//检测不到SD卡
	{
		delay_ms(500);
		LED=!LED;					//DS0闪烁
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 			//挂载SD卡 
	
	xEventGroupSetBits(app_event,APP_OLED_ON_EVENT);
}
/**************************************************************************************************************
* 函数名：  App_task
* 功能描述：主线程
* 作者：    Momo  
* 参数说明：	
* 返回值说明：
* 修改记录：
**************************************************************************************************************/

void App_task(void *pvParameters)
{
	EventBits_t EventValue;
	App_init();
	
	for(;;){
		//阻塞等待事件发生
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)app_event,		
									   (EventBits_t			)APP_EVENT_ALL,
									   (BaseType_t			)pdTRUE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)portMAX_DELAY);
		
		EventValue &= APP_NOT_DOG;
		debug(DEBUG,"事件 %d",EventValue);
		switch(EventValue)
		{
//		case APP_PROTOCOL_PARASE_EVENT:
//			App_delEvent(APP_PROTOCOL_PARASE_EVENT);
//			Protocol_paraseHandler(uart_rx_buf);
//			break;					
		case APP_KEY_EVENT:
			App_keyHandler();
			break;
		case APP_ALARM_EVENT:
			App_alarmHandler();
			break;
		case APP_BATTERY_DETECT_EVENT:
			App_batteryDetectHandler();
			break; 
		case APP_OLED_ON_EVENT:
			App_powerOnHandler();
			break;
//		case APP_CAL_STEP_EVENT:
//			App_calStepHandler();
			break;
		case APP_OLED_DOWN_EVENT:
			App_powerDownHandler();
			break;	
//		case APP_WATCH_DOG:
//			IWDG_feed();//喂狗
//			break;
		default:
//			app_event &= ~(1<<i);
			break;
		}
		Ui_poll();  //ui轮训
	}
}
/*************************************************************************************************************
* 函数名：App_timerHandler,软定时器中断函数
* 功能描述：软定时中断，触发信号量，进行喂狗
* 作者：Momo	
* 参数说明：	 
* 返回值说明：
* 修改记录：
*************************************************************************************************************/

void App_timerHandler(TimerHandle_t xTimer) 
{
	xEventGroupSetBits(app_event,APP_WATCH_DOG);	//发送看门狗事件
	Ui_postPage(UI_NO_PAGE);//调度Ui_poll();	
}

/***********************************************************************************************************************
***********************************************************************************************************************/
