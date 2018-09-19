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
* ���ߣ�		  �����  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
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
	debug(WARN,"������ʾʱ���¼�");
	dateAndTime = RTC_getDateAndTime();
	sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute);
	debug(WARN,"ʱ�䣺 %s",uiBuf);
	Ui_showString(1,2,uiBuf,12,5);
	
	batteryVoltageADCVal = Get_Adc(ADC_CH1); 
	tempBatteryVoltage = batteryVoltageADCVal*(3.3/4096);
	
	if(tempBatteryVoltage<batteryVoltage)   //��ֹ��ѹƮ����ʱ��ʱ��
	{
		batteryVoltage = tempBatteryVoltage;

        /*��ص�ѹ�͵�ص����Ĺ�ϵ*/
        //ʹ�û���������ģ���ص�ѹ����
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
* ��������       App_powerOnHandler
* ����������	  �л�ui
* ���ߣ�		  �����  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_powerOnHandler(void)
{
	debug(WARN,"�յ������¼�");
	Ui_postPage(UI_HOME_PAGE);
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
* ���ߣ�		  �����  
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
  //����� ����������Ϊ�˱�����ʱˢ�������κ󣬹���Ч��Ϊ:���ϳ���²���������ʾ���UI�����ǰ���������ʾ
 // if(BATTERY_CHARGE_ING == batteryState->chargeState)   //�����⵽��磬��������UI��ҳ����ʾ���ڳ��
 // 	Ui_postPage(UI_HOME_PAGE); //��ʾUI��ҳ
}



/***********************************************************************************************************************
***********************************************************************************************************************/
