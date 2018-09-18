/***********************************************************************************************************************
Skeleton_Detection
文件名:        app_handler.c
描述   :       完成一些相关的处理

版本   :       V1.0
修改   :   
完成日期： 
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
* 函数名：       App_detect1Handler
* 功能描述：	  检测模式1
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void detect_Init(){
	AD7192Initialization();
	ADXL355_Init(ASEL_LOW);

	detect_timer=xTimerCreate((const char*		)"DetectTimer",
						(TickType_t			)25,
						(UBaseType_t		)pdFALSE,
						(void*				)1,
						(TimerCallbackFunction_t)Detect_timerHandler); //周期定时器，周期1s(1000个时钟节拍)，周期模式
}

/**********************************************************************************************************
* 函数名：       App_detect2Handler
* 功能描述：	  检测模式2
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_detectHandler(void)
{
	u8 res;
	u8 tbuf[40];
	detect_Num = 0;
	dateAndTime_t * dateAndTime;
	
	debug(WARN,"检测模式%d,状态%d",DetectPattern.detect_pattern,DetectPattern.detect_status);
	switch (DetectPattern.detect_pattern){
		case pattern_one:
			switch (DetectPattern.detect_status){
				case start:					// 开始，所以要进行
					dateAndTime = RTC_getDateAndTime();
					sprintf((char*)tbuf,"20%02d-%02d-%02d-%02d-%02d-%02d.txt",dateAndTime->year,dateAndTime->month,
						dateAndTime->date,dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 
					
					debug(ERROR,"%s.txt",tbuf);
					while((res = f_open(&file_txt,(const TCHAR*)tbuf,FA_WRITE|FA_CREATE_NEW))){printf("file open %d \r\n",res);	vTaskDelay(1000);}
					AD7192StartContinuousConvertion(AIN1_COM|AIN2_COM);
					DetectPattern.detect_status = detecting;
					debug(DEBUG,"start read\r\n");
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case stop:
					xTimerStop(detect_timer,0);				//关闭定时器
					f_close(&file_txt);
					debug(DEBUG,"detect stop end\r\n");
					break;
//				case detecting:
//					debug(DEBUG,"start read\r\n");
//					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case suspend:
					xTimerStop(detect_timer,0);				//关闭定时器
					break;
				case continu:
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case end:
					Ui_detection_sub_page();				//调用一下检测页函数，显示检测完成
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
				//启动定时器
//	vTaskDelay(25);			//延时25个节拍
}

/**********************************************************************************************************
* 函数名：       App_detect2Handler
* 功能描述：	  检测模式2
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_detect2Handler(void)
{
	debug(WARN,"检测模式2");

}

/**********************************************************************************************************
* 函数名：       App_detect3Handler
* 功能描述：	  检测模式3
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_detect3Handler(void)
{
	debug(WARN,"检测模式3");

}
detect_t *get_DetectStatus(void){
	return &DetectPattern;
}
/***********************************************************************************************************************
***********************************************************************************************************************/
