/***********************************************************************************************************************

文件名:        key.c
描述   :       按键

版本   :       V2.0
修改   :   
完成日期：     
************************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 */

#include "key.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "app_task.h"
#include "timers.h"
#include "app_handler.h"
#include "log.h"
/***********************************************************************************************************************
 * CONSTANTS
 */

/***********************************************************************************************************************
 * TYPEDEFS
 */


/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
TimerHandle_t 	key_timer_handle;			//周期定时器句柄
key_t key; 
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */
void Key_timerHandler(TimerHandle_t xTimer); 	//周期定时器回调函数
u8 Key_getState(u8 id);
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */


/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */
/*********************************************************************************************************
* 函数名：    Key_init
* 功能描述：  按键初始化
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;    //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //中断线0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,6,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
    
    //中断线2-PH2
    HAL_NVIC_SetPriority(EXTI1_IRQn,7,0);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);             //使能中断线2
    
    //中断线3-PH3
    HAL_NVIC_SetPriority(EXTI2_IRQn,8,0);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2

//	软件消抖，判断点击，长按
	key_timer_handle = xTimerCreate((const char*		)"KeyTimerTimer",
		(TickType_t			)2000,
		(UBaseType_t		)pdTRUE,
		(void*				)1,
		(TimerCallbackFunction_t)Key_timerHandler); //周期定时器，周期20ms(10个时钟节拍)，周期模式
}
/*********************************************************************************************************
* 函数名：    key_timerHandler
* 功能描述：  按键软定时器中断服务函数
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：由于按键较硬，按下抬起一般要2-4 * 10个节拍，大多数是3 * 10个节拍
	长按一般是 10-15 * 10个节拍，所以LONG_PRESS_TIME 设置位10
	当只检测到一次是，定义为误触
**********************************************************************************************************/

void Key_timerHandler(TimerHandle_t xTimer)
{
	volatile static u16 time_cnt = 0;
	u8 keyInputState ;
	xTimerStop(key_timer_handle,0); 	//关闭key_timer
	time_cnt ++;
	
	debug(DEBUG,"按键事件 %d",time_cnt);
	
	switch(time_cnt)
	{
		case 1:		//定义为误触
			keyInputState = Key_getState(key.key_id);
			if(KEY_DOWN == keyInputState)//按键状态为按下
			{
				xTimerStart(key_timer_handle,0);//启动key_timer
			}
			else                         //按键状态为松开
			{
				debug(WARN,"按键松开");
				time_cnt = 0;
				key.key_state = KEY_UP;
			}	  
			break;  	
		case LONG_PRESS_TIME:
			time_cnt = 0;
			key.key_state = KEY_LONG_PRESS;
			debug(WARN,"长按事件");
			xEventGroupSetBits(app_event,APP_KEY_EVENT); //触发app事件
			xTimerStop(key_timer_handle,0);				//关闭key_timer	
			break;
		default:
			keyInputState = Key_getState(key.key_id);
			if(KEY_DOWN == keyInputState)				//按键状态为按下
			{
				xTimerStart(key_timer_handle,0);		//启动key_timer
			}
			else                         				//按键状态为松开
			{
				time_cnt = 0;
				key.key_state = KEY_SHORT_PRESS;
				debug(WARN,"短按事件");
				xEventGroupSetBits(app_event,APP_KEY_EVENT);//触发app按键事件
			}	  
			break;
	}
}

/*********************************************************************************************************
* 函数名：    Key_setState
* 功能描述：  获取按键状态
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
u8 Key_getState(u8 id)
{
	return HOME_KEY_INPUT(id);
}

/*********************************************************************************************************
* 函数名：    Key_setState
* 功能描述：  设置按键状态
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Key_setState(u8 id,u8 state)
{
  key.key_id = id;
  key.key_state = state;
}
/*********************************************************************************************************
* 函数名：    Key_clearState
* 功能描述：  清除按键状态
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void Key_clearState(key_t *key)
{
	key->key_id = KEY_NULL_ID;
	key->key_state = KEY_UP;
}

/*********************************************************************************************************
* 函数名：    EXTI0_IRQHandler
* 功能描述：  外部中断0服务程序,普通按键
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//调用中断处理公用函数
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);//调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);//调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t* err;
	
	debug(WARN,"按下 %d",GPIO_Pin);
	Key_setState(GPIO_Pin,KEY_DOWN);
	while(pdFAIL == xTimerStartFromISR(key_timer_handle,err));	//开启周期定时器
}

/*********************************************************************************************************
* 函数名：    Key_getKey
* 功能描述：  获取按键的状态
* 作者：      Momo  
* 参数说明：  
* 返回值说明：
* 修改记录：
**********************************************************************************************************/
key_t *Key_getKey()
{
	return &key;
}

/***********************************************************************************************************************
***********************************************************************************************************************/

