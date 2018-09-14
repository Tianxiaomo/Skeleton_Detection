/*******************************************************************************
毕业设计
文件名:       battery.h
描述   : 

版本   :       V1.0
修改   :   
完成日期：     2016.10.6
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
  u8 chargeState;   //充电状态
  u8 batteryRemain; //剩余电量
}batteryState_t;


/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define BATTERY_CHARGE_FINISH 2    //充电完成
#define BATTERY_CHARGE_ING    1    //电池充电中
#define BATTERY_DIS_CHARGE    0    //电池放电中

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
