/*******************************************************************************
��ҵ���
�ļ���:       battery.h
����   : 

�汾   :       V1.0
�޸�   :   
������ڣ�     2016.10.6
*******************************************************************************/
#ifndef _BATTERY_H
#define _BATTERY_H

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
  u8 chargeState;   //���״̬
  u8 batteryRemain; //ʣ�����
}batteryState_t;


/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define BATTERY_CHARGE_FINISH 2    //������
#define BATTERY_CHARGE_ING    1    //��س����
#define BATTERY_DIS_CHARGE    0    //��طŵ���

#define BATTERY_REMAIN_100     10
#define BATTERY_REMAIN_90     9
#define BATTERY_REMAIN_80     8
#define BATTERY_REMAIN_70     7
#define BATTERY_REMAIN_60     6
#define BATTERY_REMAIN_50     5
#define BATTERY_REMAIN_40     4
#define BATTERY_REMAIN_30     3
#define BATTERY_REMAIN_20     2
#define BATTERY_REMAIN_10     1



/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void Battery_init(void);
batteryState_t*  Battery_getState(void);
batteryState_t*  Battery_getStatus(void);
void Battery_stopSoftTimer(void);
void Battery_startSoftTimer(void);



/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _BATTERY_H */
