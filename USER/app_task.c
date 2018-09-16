/***********************************************************************************************************************
Skeleton_Detection
�ļ���:        app_task.c
����   :       ���߳�

�汾   :       V1.0
�޸�   :   
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
EventGroupHandle_t app_event;			//�¼���־���� 
TimerHandle_t 	   app_timer;			//���̶߳�ʱ�����������watchdag
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
* ��������App_Init
* ������������ʼ��
* ���ߣ�Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
****************************************************************************************************************/
void App_init()	 
{
	app_event=xEventGroupCreate();	 //�����¼���־��
	app_timer=xTimerCreate((const char*		)"AutoReloadTimer",
							(TickType_t			)1000,
							(UBaseType_t		)pdTRUE,
							(void*				)1,
							(TimerCallbackFunction_t)App_timerHandler); //���ڶ�ʱ��������1s(1000��ʱ�ӽ���)������ģʽ
	xTimerStart(app_timer,0);			//������ʱ��				
	
	LED_Init();						//��ʼ����LED���ӵ�Ӳ���ӿ�
	Key_Init();						//��ʼ������
	RTC_Init();						//RTC��ʼ��
	OLED_Init();					//��ʼ��OLED��
	piclib_init();					//��ʼ����ͼ
	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);		
 	debug(DEBUG,"oled ok");
	Ui_init();
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	while(SD_Init())				//��ⲻ��SD��
	{
		delay_ms(500);
		LED=!LED;					//DS0��˸
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 			//����SD�� 
	
	xEventGroupSetBits(app_event,APP_OLED_ON_EVENT);
}
/**************************************************************************************************************
* ��������  App_task
* �������������߳�
* ���ߣ�    Momo  
* ����˵����	
* ����ֵ˵����
* �޸ļ�¼��
**************************************************************************************************************/

void App_task(void *pvParameters)
{
	EventBits_t EventValue;
	App_init();
	
	for(;;){
		//�����ȴ��¼�����
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)app_event,		
									   (EventBits_t			)APP_EVENT_ALL,
									   (BaseType_t			)pdTRUE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)portMAX_DELAY);
		
		EventValue &= APP_NOT_DOG;
		debug(DEBUG,"�¼� %d",EventValue);
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
//			IWDG_feed();//ι��
//			break;
		default:
//			app_event &= ~(1<<i);
			break;
		}
		Ui_poll();  //ui��ѵ
	}
}
/*************************************************************************************************************
* ��������App_timerHandler,��ʱ���жϺ���
* ������������ʱ�жϣ������ź���������ι��
* ���ߣ�Momo	
* ����˵����	 
* ����ֵ˵����
* �޸ļ�¼��
*************************************************************************************************************/

void App_timerHandler(TimerHandle_t xTimer) 
{
	xEventGroupSetBits(app_event,APP_WATCH_DOG);	//���Ϳ��Ź��¼�
	Ui_postPage(UI_NO_PAGE);//����Ui_poll();	
}

/***********************************************************************************************************************
***********************************************************************************************************************/
