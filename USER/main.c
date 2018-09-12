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

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		1280  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO		4
//�����ջ��С	
#define FLOAT_STK_SIZE 		128
//������
TaskHandle_t FLOATTask_Handler;
//������
void float_task(void *pvParameters);

int main(void)
{ 
	HAL_Init();
	Stm32_Clock_Init(192,8,2,4);   	//����ʱ��,180Mhz
	delay_init(100);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	debug_init(DEBUG,1);			//�Ƿ������ԣ���ӡ�Ƿ����ɫ
   	LED_Init();						//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();						//��ʼ������
	debug(DEBUG,"led ok");
	
//	RTC_Init();						//RTC��ʼ��
	OLED_Init();					//��ʼ��OLED��
	piclib_init();					//��ʼ����ͼ
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	while(SD_Init())				//��ⲻ��SD��
	{
		delay_ms(500);
		LED=!LED;					//DS0��˸
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 			//����SD�� 
	
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
 
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //�����������
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED=~LED;
        vTaskDelay(500);
    }
}   

//LED1������
void led1_task(void *pvParameters)
{
//    while(1)
//    {
		Ui_Menu();
		vTaskDelete(LED1Task_Handler); //ɾ����ʼ����
//        vTaskDelay(1000);
//    }
}

//�����������
void float_task(void *pvParameters)
{
	static float float_num=0.00;
	while(1)
	{
		float_num+=0.01f;
		printf("float_num��ֵΪ: %.4f\r\n",float_num);
        vTaskDelay(1000);
	}
}


/*
int main(void)
{	

	
    HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(192,8,2,4);   	//����ʱ��,180Mhz
    delay_init(100);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
	debug_init(DEBUG,1);			//�Ƿ������ԣ���ӡ�Ƿ����ɫ
   	LED_Init();						//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();						//��ʼ������

	debug(DEBUG,"led ok");
	
	OLED_Init();					//��ʼ��OLED��
	piclib_init();					//��ʼ����ͼ
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	while(SD_Init())				//��ⲻ��SD��
	{
		delay_ms(500);
		LED=!LED;					//DS0��˸
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 			//����SD�� 
	debug(DEBUG,"sd card mount succed\r\n");
	
//	Show_Str(32,32,50,50,"˼����̽",24,1,GRAYBLUE,BLACK,draw_point);	
//	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);

//	Ui_Welcome();
	Ui_Menu();
	while(1);
}


*/

