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
* ��������        App_keyHandler
* ����������      ��������
* ����˵����      none
* ����ֵ˵����    none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_keyHandler(void)
{
	key_t * key;  
	ui_state_t ui_state; 
	ui_state = Ui_stateGet();                         //��ȡui��״̬
	if( UI_POWER_ON == ui_state.ui_power )           //����״̬Ϊ��
	{
		key = Key_getKey();                           	//��ȡ����״̬
		switch(key->key_state){
//			case KEY_SHORT_PRESS:                           //�̰�
//				switch(key->key_id){
//					case KEY_HOME_ID:
//					ui_state->ui_page = ui_state->ui_page<<1; //��һҳ
//					if(ui_state->ui_page > UI_LAST_PAGE)		//������������һҳ
//					{
//					ui_state->ui_page = UI_FIRST_PAGE;		//�л������һҳ
//					}


//					//APP������Ѫѹ����ʱ����ֹ��ҳ����֤���������߼���
//					if(PROTOCOL_OPEN_PULSE_BP == Protocol_getPulseBPOpenState())
//					{
//					//do nothing 
//					}
//					else  //PROTOCOL_CLOSE_PULSE_BP == Protocol_getPulseBPOpenState()
//					{
//					Ui_postPage(ui_state->ui_page);		  		//����UIҳ
//					}


//					break;
//					default:
//					//do nothing
//					break;
//				}
				break;
			case KEY_LONG_PRESS:                            	//����
				switch(key->key_id){							//�����Ҽ���Ϣ��
					case KEY_RIGHT_ID:
						xEventGroupSetBits(app_event,APP_OLED_DOWN_EVENT);
						break;
					case KEY_MENU_ID:							//�������home,��homeҳ��
						if(UI_HOME_PAGE != ui_state.ui_page)
							xEventGroupSetBits(app_event,APP_HOME_EVENT);
						break;
				}
				break;
			default:
			//do nothing
				break;
		}
		Key_clearState(key);						//�������״̬	
	}
  else if( UI_POWER_OFF == ui_state.ui_power )     //����״̬ΪϨ��
  {
//    key = Key_getKey();                           	//��ȡ����״̬
//    switch(key->key_state)
//    {
//    case KEY_SHORT_PRESS:                         //�̰�	
//      //��ʼ����
//      ui_state->ui_page = UI_HOME_PAGE;           //����main_page
//      ui_state->ui_power = UI_POWER_ON;           //���ĵ�Դ��
//      Ui_postPage(ui_state->ui_page);             //����UIҳ	
//      Key_clearState(key);                        //�������״̬				
//      break;
//    case KEY_LONG_PRESS:
//      switch(key->key_id)
//      {
//      case KEY_HOME_ID:
//        App_postEvent(APP_POWER_DOWN_ENENT);
//        break;
//      }
//      Key_clearState(key);                          //�������״̬	
//      break;
//    }
  }
}
/**********************************************************************************************************
* ��������       App_alarmHandler
* ����������	  ����
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_alarmHandler(void)
{
//  Motor_ON(MOROT_INTENSITY_3,3);  //�����������3��
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
//  Motor_ON(MOROT_INTENSITY_3,1);  //�����������1��
//  Pedometer_startSoftTimer();//����app_timer ,�Ƶ�MPU6050��ID������
//  Ui_showAdvertPage();             //�������
//  delay_ms(2000);
//  Ui_postPage(UI_HOME_PAGE);
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
//  OLED_displayOff();
//  Hp_6_PowerOFF();
//  Pedometer_stopSoftTimer();
//  Battery_stopSoftTimer();
//  Motor_ON(MOROT_INTENSITY_3,1);	//�ػ��������1��
//  Power_down();  //����ģʽ
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
  batteryState_t *batteryState;
  batteryState = Battery_getState();
  //Momo ����������Ϊ�˱�����ʱˢ�������κ󣬹���Ч��Ϊ:���ϳ���²���������ʾ���UI�����ǰ���������ʾ
 // if(BATTERY_CHARGE_ING == batteryState->chargeState)   //�����⵽��磬��������UI��ҳ����ʾ���ڳ��
 // 	Ui_postPage(UI_HOME_PAGE); //��ʾUI��ҳ
}

/***********************************************************************************************************************
***********************************************************************************************************************/
