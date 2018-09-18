/***********************************************************************************************************************
Skeleton_Detection
�ļ���:        app_handler.c
����   :       ���һЩ��صĴ���

�汾   :       V1.0
�޸�   :   
������ڣ� 
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
* ��������        App_keyHandler
* ����������      ��������
* ����˵����      none
* ����ֵ˵����    none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_keyHandler(void)
{
	debug(WARN,"�յ������¼�");
	key_t * key;  
	ui_state_t *ui_state; 
	ui_state = Ui_stateGet();                         	//��ȡui��״̬
	key = Key_getKey();                           		//��ȡ����״̬
	debug(WARN,"ui page : %d,ui state : %d",ui_state->ui_page,ui_state->ui_power);
	
	if( UI_POWER_ON == ui_state->ui_power )           //����״̬Ϊ��
	{
		if(KEY_SHORT_PRESS == key->key_state)				//�̰�
		{	
			switch(key->key_id){
				case KEY_MENU_ID:						//��MENU
					switch(ui_state->ui_page){
		/*************************һ��ҳ�棬�������************************************************/	
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
		/*************************����ҳ�棬����************************************************/	
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
				case KEY_LEFT_ID:						//�����
					switch(ui_state->ui_page){
		/*************************һ��ҳ�棬��ҳ************************************************/				
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
		/*************************����ҳ�棬����************************************************/								
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
				case KEY_RIGHT_ID:						//���Ҽ�
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
		else if(KEY_LONG_PRESS == key->key_state)				//����
		{             										
			switch(key->key_id){								//�����Ҽ���Ϣ��
				case KEY_RIGHT_ID:
						xEventGroupSetBits(app_event,APP_OLED_DOWN_EVENT);
					break;
				case KEY_MENU_ID:									//�������home,��homeҳ��
					if(UI_HOME_PAGE != ui_state->ui_page)
						Ui_postPage(UI_HOME_PAGE);
					break;
			}
		}
	}
	else if( UI_POWER_OFF == ui_state->ui_power )     	//����״̬ΪϨ��
	{
		ui_state->ui_power = UI_POWER_ON;
		debug(WARN,"�յ������¼�");
		Set_Display_On_Off(UI_POWER_ON);
	}
	Key_clearState(key);								//�������״̬	
}
/**********************************************************************************************************
* ��������       App_alarmHandler
* ����������	  ����
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_timeshowHandler(void)
{
	u8 uiBuf[10];
	dateAndTime_t *dateAndTime;
	debug(WARN,"������ʾʱ���¼�");
	dateAndTime = RTC_getDateAndTime();
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	debug(WARN,"ʱ�䣺 %s",uiBuf);
	Ui_showString(1,2,uiBuf,12,5);
}


/**********************************************************************************************************
* ��������       App_powerOnHandler
* ����������	  �л�ui
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_powerOnHandler(void)
{
	debug(WARN,"�յ������¼�");
//  Motor_ON(MOROT_INTENSITY_3,1);  //�����������1��
//  Pedometer_startSoftTimer();//����app_timer ,�Ƶ�MPU6050��ID������
//  Ui_showAdvertPage();             //�������
//  delay_ms(2000);
	Ui_postPage(UI_HOME_PAGE);
//  
}
/**********************************************************************************************************
* ��������        App_powerDownHandler
* ����������      �л�ui
 
* ����˵����      none
* ����ֵ˵����    none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_powerDownHandler(void)
{
	ui_state_t *ui_state; 
	ui_state = Ui_stateGet();                         //��ȡui��״̬
	ui_state->ui_power = UI_POWER_OFF;
	debug(WARN,"�յ�Ϣ���¼�");
	Set_Display_On_Off(UI_POWER_OFF);
}
/**********************************************************************************************************
* ��������       App_batteryDetectHandler
* ����������	  ��ص������
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_batteryDetectHandler(void)
{
	debug(WARN,"��ؼ���¼�");
//  batteryState_t *batteryState;
//  batteryState = 
	Battery_getState();
  //Momo ����������Ϊ�˱�����ʱˢ�������κ󣬹���Ч��Ϊ:���ϳ���²���������ʾ���UI�����ǰ���������ʾ
 // if(BATTERY_CHARGE_ING == batteryState->chargeState)   //�����⵽��磬��������UI��ҳ����ʾ���ڳ��
 // 	Ui_postPage(UI_HOME_PAGE); //��ʾUI��ҳ
}



/***********************************************************************************************************************
***********************************************************************************************************************/
