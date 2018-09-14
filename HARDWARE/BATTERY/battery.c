/***********************************************************************************************************************
��ҵ���
�ļ���:        battery.c
����   :       ��ص�������س����غ���

�汾   :       V1.0
�޸�   :   
������ڣ�     2016.10.6
************************************************************************************************************************/

/***********************************************************************************************************************
* INCLUDES
*/
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "battery.h"
#include "adc.h"
//#include "gpio.h"
#include "app_task.h"
#include "app_handler.h"
//#include "uart.h"


/***********************************************************************************************************************
* CONSTANTS
*/
//#define STAND_BY_PIN_VOLTAGE  ((GPIOB->IDR & 1<<6) && 1) 
#define LOW_VOLTAGE           0
#define HIGH_VOLTAGE          1
/***********************************************************************************************************************
* TYPEDEFS
*/


/***********************************************************************************************************************
* LOCAL VARIABLES
*/
TimerHandle_t   battery_timer;	   //��ص�����������ʱ��

batteryState_t batteryState;
/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
float Battery_calVoltage(u16 adcVal);
void Battery_timerHandler(void);
u8 Battery_chargeFinishDetct(void);
u8 Battery_getStandByPinVoltage(void);
void Battery_standByPinInit(void);

/***********************************************************************************************************************
* LOCAL FUNCTIONS  
*/



/***********************************************************************************************************************
* PUBLIC FUNCTIONS
*/
/**********************************************************************************************************
* �������� 	   BATTERY_init
* ����������	   ��س�ʼ������ʼ��ADCͨ��������ʼ��һ����ʱ��
* ���ߣ�		   Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
void Battery_init(void)
{
	u8 err;
	MY_ADC_Init();

	Battery_standByPinInit();  //��ʼ��IO�ڣ����ڼ�����Ƿ����
	/********************<��ʱ����ʼ��>***************************/

	//��ص��������ʱ��battery_timer��50msִ��һ��
	battery_timer = xTimerCreate((const char*		)"BatteryTimerTimer",
								(TickType_t			)50/portTICK_PERIOD_MS,
								(UBaseType_t		)pdTRUE,
								(void*				)1,
								(TimerCallbackFunction_t)Battery_timerHandler); //���ڶ�ʱ��������50ms(50��ʱ�ӽ���)������ģʽ

	Battery_startSoftTimer();
}
/**********************************************************************************************************
* �������� 	   Battery_startSoftTimer
* ����������	   �򿪵�ؼ����ʱ��
* ���ߣ�		   Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
void Battery_startSoftTimer(void)
{
	xTimerStart(battery_timer,0);	//����battery_timer
}
/**********************************************************************************************************
* �������� 	   Battery_stopSoftTimer
* ����������	   �򿪵�ؼ����ʱ��
* ���ߣ�		   Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
void Battery_stopSoftTimer(void)
{
	xTimerStop(battery_timer,0);
}
/**********************************************************************************************************
* �������� 	   		BATTERY_init
* ����������	   ��ʼ��IO�ڣ����ڼ�����Ƿ����
* ���ߣ�		     Momo  
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
void Battery_standByPinInit(void)
{
//  RCC->AHB1ENR|=1<<1;//ʹ��PORTBʱ�� 
//  GPIO_set(GPIOB,PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PU); //PB6���ڼ�����Ƿ���� 
}

/**********************************************************************************************************
* ��������        BATTERY_getState
* ����������      ��ȡ��ص�״̬:�Ƿ���?���ʣ�����Ϊ����
 
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
batteryState_t*  Battery_getState(void)
{
  static u8 i;  //��̬����
  static u32 adcValSum;
  u16 adcAverage;
  u16 batteryVoltageADCVal;
  static float batteryVoltage = 2.1;
  float tempBatteryVoltage;
  batteryVoltageADCVal = Get_Adc(ADC_CH1);                       //��ȡADCֵ
  
  tempBatteryVoltage = Battery_calVoltage(batteryVoltageADCVal);    //��ADC��ֵ������ʵ�ʵ�ѹ
  if( tempBatteryVoltage > 1.98f )//Ӳ������ص������룬���ʱ��ص���Ϊ5v,����Ϊ2.5v,��Ҫʵʱ�ԣ�����ȡƽ��ֵ����þ�׼��
  {
    if( BATTERY_CHARGE_FINISH == Battery_chargeFinishDetct())
      batteryState.chargeState = BATTERY_CHARGE_FINISH;	//���״̬Ϊ:������
    else if( BATTERY_CHARGE_ING == Battery_chargeFinishDetct())
      batteryState.chargeState = BATTERY_CHARGE_ING;	//���״̬Ϊ:�����

	batteryVoltage = 2.1f;  //Momo  ���ó����磬��ص���"����"Ҳ���ص�
  }
  else  //batteryVoltage < 2.3���ڷŵ�״̬����ص�������ʱ����ص�ѹΪ4.2v
  {
    batteryState.chargeState = BATTERY_DIS_CHARGE;  //���״̬Ϊ:�ŵ���
    
    if( 20 == i )  //Momo :����һ����ӣ�����ж�����Ϊif( 19 == i )���ͻ�����һ��
    {
      adcAverage = (u16)(adcValSum/20);//����20��ADCֵ��ƽ��ֵ��Ŀ����:��С���
      tempBatteryVoltage = Battery_calVoltage(adcAverage);//��ƽ��ֵ�������ص�ѹ
      if(tempBatteryVoltage<batteryVoltage)   //��ֹ��ѹƮ����ʱ��ʱ��
      {
        batteryVoltage = tempBatteryVoltage;
        
        /*��ص�ѹ�͵�ص����Ĺ�ϵ*/
        //ʹ�û���������ģ���ص�ѹ����
        if( batteryVoltage > 1.84f && batteryVoltage < 1.96f )      //����Ϊ100%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_100;
        }
        else if( batteryVoltage >1.78f && batteryVoltage< 1.84f ) //����Ϊ75%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_70;
        }
        else if( batteryVoltage >1.73f && batteryVoltage< 1.78f ) //����Ϊ50%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_50;
        }
        else if( batteryVoltage >1.67f && batteryVoltage< 1.73f )  //����Ϊ25%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_20;
        }
        else if( batteryVoltage >1.61f && batteryVoltage< 1.67f )   //�����ͣ������ػ�
        {
          batteryState.batteryRemain = BATTERY_REMAIN_10;
        }
        else if(batteryVoltage >1.58f  && batteryVoltage <1.61f)// batteryVoltage <1.61  ������<5% ,�ػ��������
        {
//          App_postEvent(APP_POWER_DOWN_ENENT);                 //�������ͣ��ػ��������
        }
        else
        {
          //do nothing
        }
      }
      i = 0;         //����
      adcValSum = 0; //����
    }
    else 
    {
      adcValSum = adcValSum+batteryVoltageADCVal;//����20��ֵ
      i++;
    }
  }
  return &batteryState;
}

/**********************************************************************************************************
* ��������        Battery_getStatus
* ����������      ��ȡ��ص�״̬:�Ƿ���?���ʣ�����Ϊ����,�ú�����Battery_getState()������һ��
 
* ����˵����      ����batteryState
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
batteryState_t*  Battery_getStatus(void)
{
  return &batteryState;
}

/**********************************************************************************************************
* ��������        Battery_calVoltage
* ����������      ����ADCֵ��ת��Ϊ��ѹֵ
 
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
float Battery_calVoltage(u16 adcVal)
{
  
  return ((float)adcVal*(3.3/4096));       //Momo :��������Ż����ѳ����ĳ���λ
  
}
/**********************************************************************************************************
* ��������        Battery_timerHandler
* ����������      battery_timer��ʱ���жϺ���
 
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
void Battery_timerHandler(void)
{
	xEventGroupSetBits(app_event,APP_BATTERY_DETECT_EVENT);
}
/**********************************************************************************************************
* ��������        Battery_chargeFinishDetct
* ����������      ������Ƿ����,��⵽standby�ܽ�Ϊ�͵�ƽ��������ɣ��ߵ�ƽ���ǳ����
 
* ����˵����  
* ����ֵ˵����
* �޸ļ�¼��
**********************************************************************************************************/
u8 Battery_chargeFinishDetct(void)
{
//  if( LOW_VOLTAGE == Battery_getStandByPinVoltage())
//    return BATTERY_CHARGE_FINISH;
//  else 
//    return BATTERY_CHARGE_ING;
}
u8 Battery_getStandByPinVoltage(void)
{
//  return STAND_BY_PIN_VOLTAGE;
}
/***********************************************************************************************************************
***********************************************************************************************************************/
