/*******************************************************************************
 * INCLUDES
 */
#include "FreeRTOS.h"
#include "task.h"  		//����freertos����ļ�
#include "delay.h"
#include "app_task.h"
#include "usart.h"
#include "log.h"
/*******************************************************************************
 * CONSTANTS
 */
#define START_TASK_PRIO      				10        //start_task�����ȼ�����Ϊ���
#define START_STK_SIZE  				    64        //����start_task��ջ��С
#define APP_TASK_PRIO       				4         //����App_task���ȼ�
#define APP_STK_SIZE  					    3072      //����App_task�����ջ��С

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */	
TaskHandle_t AppTask_Handler;						//������
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
	Stm32_Clock_Init(192,8,2,4);   	//����ʱ��,180Mhz
	delay_init(100);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	debug_init(ERROR,1);			//�Ƿ������ԣ���ӡ�Ƿ����ɫ

	debug(DEBUG,"led ok");

		//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler(); 
}

static void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           		//�����ٽ��� 	
 	//���߳�
	xTaskCreate((TaskFunction_t )App_task,            	//������
                (const char*    )"App_task",          	//��������
                (uint16_t       )APP_STK_SIZE,        	//�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )APP_TASK_PRIO,         //�������ȼ�
                (TaskHandle_t*  )&AppTask_Handler);   //������      
	 
 	//OSTaskCreate(Pedometer_task,
	//             (void *)0,
	//						 (OS_STK*)&PEDOMETER_TASK_STK[PEDOMETER_STK_SIZE-1],
	//						 PEDOMETER_TASK_PRIO);	
 	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}	  


/*******************************************************************************
*******************************************************************************/






