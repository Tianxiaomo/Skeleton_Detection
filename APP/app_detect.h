/*******************************************************************************
Skeleton_Detection
文件名:        app_detect.h
描述   :      

版本   :       V1.0
修改   :   
完成日期：     2016.10.13
*******************************************************************************/
#ifndef APP_DETECT_H
#define APP_DETECT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "sys.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/*********************************************************************
 * TYPEDEFS
 */
typedef enum
{ 
start = 1,
next_start,
stop,
detecting,
suspend,
continu,
end
}detect_state_t;

typedef enum
{ 
pattern_one = 1,
pattern_two,
pattern_three
}detect_pattern_t;

typedef struct
{
	u8 detect_pattern;
	u8 detect_status;
}detect_t;
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern EventGroupHandle_t app_event;
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void App_detect_Init(void);
void App_detectHandler(void);
void App_detect3Handler(void);
detect_t *get_DetectStatus(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_H */
