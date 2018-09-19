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
float adxl355Scale;
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
void App_detect_Init(void){
	switch (DetectPattern.detect_pattern){
		case pattern_one:
			AD7192Initialization();
			break;
		case pattern_two:
			ADXL355_1_Init(ASEL_LOW);
			ADXL355_2_Init(ASEL_HIGH);
			break;
		case pattern_three:
			AD7192Initialization();
			ADXL355_1_Init(ASEL_LOW);
			ADXL355_2_Init(ASEL_HIGH);
			break;
	}
	
	detect_timer=xTimerCreate((const char*		)"DetectTimer",
						(TickType_t			)25,
						(UBaseType_t		)pdFALSE,
						(void*				)1,
						(TimerCallbackFunction_t)Detect_timerHandler); //周期定时器，周期1s(1000个时钟节拍)，周期模式
}

/**********************************************************************************************************
* 函数名：       App_detectHandler
* 功能描述：	  检测模式
* 作者：		  Momo  
* 参数说明：	  none
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void App_detectHandler(void){
	u8 res;
	u8 tbuf[40];
	detect_Num = 0;
	dateAndTime_t * dateAndTime;
	
	debug(WARN,"检测模式%d,状态%d",DetectPattern.detect_pattern,DetectPattern.detect_status);
	switch (DetectPattern.detect_pattern){
		case pattern_one:							//模式一
			switch (DetectPattern.detect_status){
				case start:					// 开始，所以要进行
					dateAndTime = RTC_getDateAndTime();
					sprintf((char*)tbuf,"Pattern1-20%02d-%02d-%02d-%02d-%02d-%02d.txt",dateAndTime->year,dateAndTime->month,
						dateAndTime->date,dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 
					
					debug(ERROR,"%s",tbuf);
					while((res = f_open(&file_txt,(const TCHAR*)tbuf,FA_WRITE|FA_CREATE_NEW))){printf("file open %d \r\n",res);	vTaskDelay(1000);}
					AD7192StartContinuousConvertion(AIN1_COM|AIN2_COM);
					DetectPattern.detect_status = detecting;
					debug(DEBUG,"start read\r\n");
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case stop:
					xTimerStop(detect_timer,0);				//关闭定时器
					f_close(&file_txt);
					AD7192ExitContinuousRead();
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
					Ui_Detecting_Page(0);				//调用一下检测页函数，显示检测完成
					break;
			}
			break;	
		case pattern_two:
			switch (DetectPattern.detect_status){
				case start:					// 开始，所以要进行
					dateAndTime = RTC_getDateAndTime();
					sprintf((char*)tbuf,"Pattern-2-20%02d-%02d-%02d-%02d-%02d-%02d.txt",dateAndTime->year,dateAndTime->month,
						dateAndTime->date,dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 
					
					debug(ERROR,"%s",tbuf);
					while((res = f_open(&file_txt,(const TCHAR*)tbuf,FA_WRITE|FA_CREATE_NEW))){printf("file open %d \r\n",res);	vTaskDelay(1000);}
					adxl355Scale = ADXL_RANGE_SET_1(8);
					adxl355Scale = ADXL_RANGE_SET_2(8);
					ADXL355_1_Start_Sensor();
					ADXL355_2_Start_Sensor();
					DetectPattern.detect_status = detecting;
					debug(DEBUG,"start read\r\n");
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case stop:
					xTimerStop(detect_timer,0);				//关闭定时器
					f_close(&file_txt);
					ADXL355_1_Stop_Sensor();
					ADXL355_2_Stop_Sensor();
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
					Ui_Detecting_Page(0);				//调用一下检测页函数，显示检测完成
					break;
			}
			break;
		case pattern_three:
			switch (DetectPattern.detect_status){
				case start:					// 开始，所以要进行
					dateAndTime = RTC_getDateAndTime();
					sprintf((char*)tbuf,"Pattern-3-20%02d-%02d-%02d-%02d-%02d-%02d.txt",dateAndTime->year,dateAndTime->month,
						dateAndTime->date,dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 
					
					debug(ERROR,"%s",tbuf);
					while((res = f_open(&file_txt,(const TCHAR*)tbuf,FA_WRITE|FA_CREATE_NEW))){printf("file open %d \r\n",res);	vTaskDelay(1000);}
					adxl355Scale = ADXL_RANGE_SET_1(8);
					adxl355Scale = ADXL_RANGE_SET_2(8);
					ADXL355_1_Start_Sensor();
					ADXL355_2_Start_Sensor();
					AD7192StartContinuousConvertion(AIN1_COM|AIN2_COM);
					DetectPattern.detect_status = detecting;
					debug(DEBUG,"start read\r\n");
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case stop:
					xTimerStop(detect_timer,0);				//关闭定时器
					f_close(&file_txt);
					ADXL355_1_Stop_Sensor();
					ADXL355_2_Stop_Sensor();
					AD7192ExitContinuousRead();
					debug(DEBUG,"detect stop end\r\n");
					break;
				case suspend:
					xTimerStop(detect_timer,0);				//关闭定时器
					break;
				case continu:
					xTimerStart(detect_timer,0);			//启动定时器	
					break;
				case end:
					Ui_Detecting_Page(0);				//调用一下检测页函数，显示检测完成
					break;
			}
			break;
	}
}
/**********************************************************************************************************
* 函数名：       Detect_timerHandler
* 功能描述：	  检测间隔
* 作者：		  Momo  
* 参数说明：	  xTimer
* 返回值说明：	  none
* 修改记录： 
**********************************************************************************************************/
void Detect_timerHandler(TimerHandle_t xTimer){
	int_least32_t SensorX,SensorY,SensorZ;
	u32 SensorT,size;
	char tem[125];
	switch (DetectPattern.detect_pattern){
		case pattern_one:
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
				AD7192ExitContinuousRead();
				debug(DEBUG,"read end\r\n");
				xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
				DetectPattern.detect_status = end;
			}
			break;
		case pattern_two:
			if(detect_Num < 10000){
				detect_Num++;
				ADXL355_1_Data_Scan( &SensorX, &SensorY, &SensorZ, &SensorT);
				size=sprintf(tem,"%f;%f;%f\r\n",(float)SensorX / adxl355Scale,(float)SensorX / adxl355Scale,(float)SensorZ / adxl355Scale);
				f_write(&file_txt,tem,size,&flag);
			}else{
				detect_Num = 0;
				f_close(&file_txt);
				ADXL355_1_Stop_Sensor();
				ADXL355_2_Stop_Sensor();
				debug(DEBUG,"read end\r\n");
				xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
				DetectPattern.detect_status = end;
			}
			xTimerStart(detect_timer,0);
			break;
		case pattern_three:
			if(detect_Num < 10000){
				detect_Num++;
				total = AD7192ReadConvertingData();
				f_write(&file_txt,&total,sizeof(total),&flag);
				
				ADXL355_1_Data_Scan( &SensorX, &SensorY, &SensorZ, &SensorT);
				size=sprintf(tem,"%f;%f;%f\r\n",(float)SensorX / adxl355Scale,(float)SensorX / adxl355Scale,(float)SensorZ / adxl355Scale);
				f_write(&file_txt,tem,size,&flag);
				xTimerStart(detect_timer,0);
			}else{
				detect_Num = 0;
				f_close(&file_txt);
				ADXL355_1_Stop_Sensor();
				ADXL355_2_Stop_Sensor();
				AD7192ExitContinuousRead();
				debug(DEBUG,"read end\r\n");
				xEventGroupSetBits(app_event,APP_DETECTION_EVENT);
				DetectPattern.detect_status = end;
			}
			xTimerStart(detect_timer,0);
			break;
	}
}

/**********************************************************************************************************
* 函数名：       get_DetectStatus
* 功能描述：	  获取detect_t
* 作者：		  Momo  
* 参数说明：	  void
* 返回值说明：	  DetectPattern
* 修改记录： 
**********************************************************************************************************/
detect_t *get_DetectStatus(void){
	return &DetectPattern;
}
/***********************************************************************************************************************
***********************************************************************************************************************/
