/***********************************************************************************************************************
毕业设计
文件名:        battery.c
描述   :       电池电量、电池充电相关函数

版本   :       V1.0
修改   :   
完成日期：     2016.10.6
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
TimerHandle_t   battery_timer;	   //电池电量检测软件定时器

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
* 函数名： 	   BATTERY_init
* 功能描述：	   电池初始化，初始化ADC通道，并初始化一个软定时器
* 作者：		   Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Battery_init(void)
{
	u8 err;
	MY_ADC_Init();

	Battery_standByPinInit();  //初始化IO口，用于检测充电是否完成
	/********************<软定时器初始化>***************************/

	//电池电量检测软定时器battery_timer，50ms执行一次
	battery_timer = xTimerCreate((const char*		)"BatteryTimerTimer",
								(TickType_t			)50/portTICK_PERIOD_MS,
								(UBaseType_t		)pdTRUE,
								(void*				)1,
								(TimerCallbackFunction_t)Battery_timerHandler); //周期定时器，周期50ms(50个时钟节拍)，周期模式

	Battery_startSoftTimer();
}
/**********************************************************************************************************
* 函数名： 	   Battery_startSoftTimer
* 功能描述：	   打开电池检测软定时器
* 作者：		   Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Battery_startSoftTimer(void)
{
	xTimerStart(battery_timer,0);	//启动battery_timer
}
/**********************************************************************************************************
* 函数名： 	   Battery_stopSoftTimer
* 功能描述：	   打开电池检测软定时器
* 作者：		   Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Battery_stopSoftTimer(void)
{
	xTimerStop(battery_timer,0);
}
/**********************************************************************************************************
* 函数名： 	   		BATTERY_init
* 功能描述：	   初始化IO口，用于检测充电是否完成
* 作者：		     Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Battery_standByPinInit(void)
{
//  RCC->AHB1ENR|=1<<1;//使能PORTB时钟 
//  GPIO_set(GPIOB,PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PU); //PB6用于检测充电是否完成 
}

/**********************************************************************************************************
* 函数名：        BATTERY_getState
* 功能描述：      获取电池的状态:是否充电?电池剩余电量为多少
 
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
batteryState_t*  Battery_getState(void)
{
  static u8 i;  //静态变量
  static u32 adcValSum;
  u16 adcAverage;
  u16 batteryVoltageADCVal;
  static float batteryVoltage = 2.1;
  float tempBatteryVoltage;
  batteryVoltageADCVal = Get_Adc(ADC_CH1);                       //获取ADC值
  
  tempBatteryVoltage = Battery_calVoltage(batteryVoltageADCVal);    //由ADC的值来计算实际电压
  if( tempBatteryVoltage > 1.98f )//硬件将电池电量减半，充电时电池电量为5v,减半为2.5v,需要实时性，无需取平均值来获得精准度
  {
    if( BATTERY_CHARGE_FINISH == Battery_chargeFinishDetct())
      batteryState.chargeState = BATTERY_CHARGE_FINISH;	//电池状态为:充电完成
    else if( BATTERY_CHARGE_ING == Battery_chargeFinishDetct())
      batteryState.chargeState = BATTERY_CHARGE_ING;	//电池状态为:充电中

	batteryVoltage = 2.1f;  //Momo  放置充满电，电池电量"格数"也不回弹
  }
  else  //batteryVoltage < 2.3处于放电状态，电池电量饱满时，电池电压为4.2v
  {
    batteryState.chargeState = BATTERY_DIS_CHARGE;  //电池状态为:放电中
    
    if( 20 == i )  //Momo :这是一个大坑，如果判断条件为if( 19 == i )，就会少算一次
    {
      adcAverage = (u16)(adcValSum/20);//计算20个ADC值得平均值，目的是:减小误差
      tempBatteryVoltage = Battery_calVoltage(adcAverage);//由平均值，计算电池电压
      if(tempBatteryVoltage<batteryVoltage)   //防止电压飘动，时高时低
      {
        batteryVoltage = tempBatteryVoltage;
        
        /*电池电压和电池电量的关系*/
        //使用滑动变阻器模拟电池电压会有
        if( batteryVoltage > 1.84f && batteryVoltage < 1.96f )      //电量为100%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_100;
        }
        else if( batteryVoltage >1.78f && batteryVoltage< 1.84f ) //电量为75%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_70;
        }
        else if( batteryVoltage >1.73f && batteryVoltage< 1.78f ) //电量为50%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_50;
        }
        else if( batteryVoltage >1.67f && batteryVoltage< 1.73f )  //电量为25%
        {
          batteryState.batteryRemain = BATTERY_REMAIN_20;
        }
        else if( batteryVoltage >1.61f && batteryVoltage< 1.67f )   //电量低，即将关机
        {
          batteryState.batteryRemain = BATTERY_REMAIN_10;
        }
        else if(batteryVoltage >1.58f  && batteryVoltage <1.61f)// batteryVoltage <1.61  ，电量<5% ,关机保护电池
        {
//          App_postEvent(APP_POWER_DOWN_ENENT);                 //电量过低，关机保护电池
        }
        else
        {
          //do nothing
        }
      }
      i = 0;         //清零
      adcValSum = 0; //清零
    }
    else 
    {
      adcValSum = adcValSum+batteryVoltageADCVal;//保存20个值
      i++;
    }
  }
  return &batteryState;
}

/**********************************************************************************************************
* 函数名：        Battery_getStatus
* 功能描述：      获取电池的状态:是否充电?电池剩余电量为多少,该函数和Battery_getState()函数不一样
 
* 参数说明：      返回batteryState
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
batteryState_t*  Battery_getStatus(void)
{
  return &batteryState;
}

/**********************************************************************************************************
* 函数名：        Battery_calVoltage
* 功能描述：      根据ADC值，转化为电压值
 
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
float Battery_calVoltage(u16 adcVal)
{
  
  return ((float)adcVal*(3.3/4096));       //Momo :这里可以优化，把除法改成移位
  
}
/**********************************************************************************************************
* 函数名：        Battery_timerHandler
* 功能描述：      battery_timer软定时器中断函数
 
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Battery_timerHandler(void)
{
	xEventGroupSetBits(app_event,APP_BATTERY_DETECT_EVENT);
}
/**********************************************************************************************************
* 函数名：        Battery_chargeFinishDetct
* 功能描述：      检测充电是否完成,检测到standby管脚为低电平，则充电完成，高电平就是充电中
 
* 参数说明：  
* 返回值说明：
* 修改记录：
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
