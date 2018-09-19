/*******************************************************************************
 * INCLUDES
 */
#include "FreeRTOS.h"
#include "task.h"  		//包含freertos相关文件
#include "delay.h"
#include "app_task.h"
#include "usart.h"
#include "log.h"
/*******************************************************************************
 * CONSTANTS
 */
#define START_TASK_PRIO      				10        //start_task的优先级设置为最低
#define START_STK_SIZE  				    64        //设置start_task堆栈大小
#define APP_TASK_PRIO       				4         //设置App_task优先级
#define APP_STK_SIZE  					    3072      //设置App_task任务堆栈大小

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */	
TaskHandle_t AppTask_Handler;						//任务句柄
TaskHandle_t StartTask_Handler;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
static void start_task(void *pvParameters);

/*******************************************************************************
 * PROFILE CALLBACKS
 */

/*******************************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{	
	HAL_Init();
	Stm32_Clock_Init(192,8,2,4);   	//设置时钟,180Mhz
	delay_init(100);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	debug_init(ERROR,1);			//是否开启调试，打印是否带颜色

	debug(DEBUG,"led ok");

		//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler(); 
}

static void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           		//进入临界区 	
 	//主线程
	xTaskCreate((TaskFunction_t )App_task,            	//任务函数
                (const char*    )"App_task",          	//任务名称
                (uint16_t       )APP_STK_SIZE,        	//任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )APP_TASK_PRIO,         //任务优先级
                (TaskHandle_t*  )&AppTask_Handler);   //任务句柄      
	 
 	//OSTaskCreate(Pedometer_task,
	//             (void *)0,
	//						 (OS_STK*)&PEDOMETER_TASK_STK[PEDOMETER_STK_SIZE-1],
	//						 PEDOMETER_TASK_PRIO);	
 	vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}	  


/*******************************************************************************
*******************************************************************************/






