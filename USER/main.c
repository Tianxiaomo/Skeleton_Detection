#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "string.h"
#include "malloc.h"
#include "oled.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"
#include "ui.h"
#include "log.h"
#include "rtc.h"
#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		1280  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

int main(void)
{ 
	HAL_Init();
	Stm32_Clock_Init(192,8,2,4);   	//设置时钟,180Mhz
	delay_init(100);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	debug_init(DEBUG,1);			//是否开启调试，打印是否带颜色
   	LED_Init();						//初始化与LED连接的硬件接口
	KEY_Init();						//初始化按键
	debug(DEBUG,"led ok");
	
//	RTC_Init();						//RTC初始化
	OLED_Init();					//初始化OLED屏
	piclib_init();					//初始化画图
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	while(SD_Init())				//检测不到SD卡
	{
		delay_ms(500);
		LED=!LED;					//DS0闪烁
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 			//挂载SD卡 
	
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED=~LED;
        vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
//    while(1)
//    {
		Ui_Menu();
		vTaskDelete(LED1Task_Handler); //删除开始任务
//        vTaskDelay(1000);
//    }
}

//浮点测试任务
void float_task(void *pvParameters)
{
	static float float_num=0.00;
	while(1)
	{
		float_num+=0.01f;
		printf("float_num的值为: %.4f\r\n",float_num);
        vTaskDelay(1000);
	}
}


/*
int main(void)
{	

	
    HAL_Init();                     //初始化HAL库   
	Stm32_Clock_Init(192,8,2,4);   	//设置时钟,180Mhz
    delay_init(100);                //初始化延时函数
    uart_init(115200);              //初始化USART
	debug_init(DEBUG,1);			//是否开启调试，打印是否带颜色
   	LED_Init();						//初始化与LED连接的硬件接口
	KEY_Init();						//初始化按键

	debug(DEBUG,"led ok");
	
	OLED_Init();					//初始化OLED屏
	piclib_init();					//初始化画图
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	while(SD_Init())				//检测不到SD卡
	{
		delay_ms(500);
		LED=!LED;					//DS0闪烁
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 			//挂载SD卡 
	debug(DEBUG,"sd card mount succed\r\n");
	
//	Show_Str(32,32,50,50,"思博声探",24,1,GRAYBLUE,BLACK,draw_point);	
//	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);

//	Ui_Welcome();
	Ui_Menu();
	while(1);
}


*/

