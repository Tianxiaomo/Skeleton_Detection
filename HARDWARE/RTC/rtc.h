/*******************************************************************************
Skeleton_Detection
�ļ���:        rtc.h
����   :       ʵʱʱ��ͷ�ļ�

�汾   :       V1.0
�޸�   :   
������ڣ�     2017.419
*******************************************************************************/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "sys.h"
#include "stm32f4xx.h"

/*********************************************************************
 * TYPEDEFS
 */
typedef struct 
{
  u8 year;
  u8 month;
  u8 date;
  u8 week;
  u8 hour;
  u8 minute;
  u8 second;	
  u8 ampm;
}dateAndTime_t;
typedef	struct
{
	u8 hour;
	u8 minute;
	u8 second;
	u8 twentyMsCount;
}timeStamp_t;
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern RTC_HandleTypeDef RTC_Handler;  //RTC���

/*********************************************************************
 * CONSTANTS
 */
#define   MONDAY         1
#define   TUESDAY        2
#define   WEDNESDAY      3
#define   THURSDAY       4
#define   FRIDAY         5
#define   SATURDAY       6
#define   SUNDAY         7
 

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
u8 RTC_init(void);
u8 RTC_initMode(void);
void RTC_setAlarmA(u8 hour, u8 min, u8 sec, u8 week);
dateAndTime_t *RTC_getDateAndTime(void);
u8 RTC_setDateAndTime(dateAndTime_t *dateAndTime);
dateAndTime_t *RTC_returnDateTimePointer(void);



u8 RTC_Init(void);              //RTC��ʼ��
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);      //RTCʱ������
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);	//RTC��������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec); //��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);             //�����Ի��Ѷ�ʱ������
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H */


