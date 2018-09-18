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
#include "app_detect.h"
#include "ui.h"
#include "app_task.h"
#include "delay.h"
#include "log.h"
#include "ad7192.h"
#include "ADXL355.h"
#include "fattester.h"
#include "rtc.h"
#include "ui.h"
#include "app_handler.h"
/***********************************************************************************************************************
 * CONSTANTS
 */

/***********************************************************************************************************************
 * TYPEDEFS
 */

/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
TimerHandle_t 	   detect_timer;			//
UINT flag;
FIL file_txt;
u32 total;
u32 detect_Num;
detect_t DetectPattern = {0};

/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */
void Detect_timerHandler(TimerHandle_t xTimer);

/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */

/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */

/**********************************************************************************************************
* ��������       App_detect1Handler
* ����������	  ���ģʽ1
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void detect_Init(){
	AD7192Initialization();
	ADXL355_Init(ASEL_LOW);

	detect_timer=xTimerCreate((const char*		)"DetectTimer",
						(TickType_t			)25,
						(UBaseType_t		)pdFALSE,
						(void*				)1,
						(TimerCallbackFunction_t)Detect_timerHandler); //���ڶ�ʱ��������1s(1000��ʱ�ӽ���)������ģʽ
}

/**********************************************************************************************************
* ��������       App_detect2Handler
* ����������	  ���ģʽ2
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_detectHandler(void)
{
	u8 res;
	u8 tbuf[40];
	detect_Num = 0;
	dateAndTime_t * dateAndTime;
	
	debug(WARN,"���ģʽ%d,״̬%d",DetectPattern.detect_pattern,DetectPattern.detect_status);
	switch (DetectPattern.detect_pattern){
		case pattern_one:
			switch (DetectPattern.detect_status){
				case start:					// ��ʼ������Ҫ����
					dateAndTime = RTC_getDateAndTime();
					sprintf((char*)tbuf,"20%02d-%02d-%02d-%02d-%02d-%02d.txt",dateAndTime->year,dateAndTime->month,
						dateAndTime->date,dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 
					
					debug(ERROR,"%s.txt",tbuf);
					while((res = f_open(&file_txt,(const TCHAR*)tbuf,FA_WRITE|FA_CREATE_NEW))){printf("file open %d \r\n",res);	vTaskDelay(1000);}
					AD7192StartContinuousConvertion(AIN1_COM|AIN2_COM);
					DetectPattern.detect_status = detecting;
					debug(DEBUG,"start read\r\n");
					xTimerStart(detect_timer,0);			//������ʱ��	
					break;
				case stop:
					xTimerStop(detect_timer,0);				//�رն�ʱ��
					f_close(&file_txt);
					debug(DEBUG,"detect stop end\r\n");
					break;
//				case detecting:
//					debug(DEBUG,"start read\r\n");
//					xTimerStart(detect_timer,0);			//������ʱ��	
					break;
				case suspend:
					xTimerStop(detect_timer,0);				//�رն�ʱ��
					break;
				case continu:
					xTimerStart(detect_timer,0);			//������ʱ��	
					break;
				case end:
					Ui_detection_sub_page();				//����һ�¼��ҳ��������ʾ������
					break;
			}
		
		
			break;	
		case pattern_two:
			
			break;
		case pattern_three:
			
			break;
	
	}


	
}

void Detect_timerHandler(TimerHandle_t xTimer){
	total = AD7192ReadConvertingData();
	f_write(&file_txt,&total,sizeof(total),&flag);
	if(detect_Num < 10000){
		detect_Num++;
		total = AD7192ReadConvertingData();
		f_write(&file_txt,&total,sizeof(total),&flag);
		xTimerStart(detect_timer,0);
	}else{
		detect_Num = 0;
		f_close(&file_txt);
		debug(DEBUG,"read end\r\n");
		xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
		DetectPattern.detect_status = end;
	}
				//������ʱ��
//	vTaskDelay(25);			//��ʱ25������
}

/**********************************************************************************************************
* ��������       App_detect2Handler
* ����������	  ���ģʽ2
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_detect2Handler(void)
{
	debug(WARN,"���ģʽ2");

}

/**********************************************************************************************************
* ��������       App_detect3Handler
* ����������	  ���ģʽ3
* ���ߣ�		  Momo  
* ����˵����	  none
* ����ֵ˵����	  none
* �޸ļ�¼�� 
**********************************************************************************************************/
void App_detect3Handler(void)
{
	debug(WARN,"���ģʽ3");

}
detect_t *get_DetectStatus(void){
	return &DetectPattern;
}
/***********************************************************************************************************************
***********************************************************************************************************************/
