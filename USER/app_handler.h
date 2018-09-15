/*******************************************************************************
Skeleton_Detection
文件名:        app_handler.h
描述   : 
版本   :       V1.0
修改   :   
完成日期：
*******************************************************************************/
#ifndef APP_HANDLER_H
#define APP_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define APP_EVENT_SIZE                  32
#define APP_EVENT_NULL                  0X000000
#define APP_OLED_ON_EVENT               0X000001
#define APP_OLED_DOWN_EVENT             0X000002
	
#define APP_HOME_EVENT					0X000004
	
#define APP_PROTOCOL_PARASE_EVENT       0X000004
#define APP_KEY_EVENT                   0X000008
#define APP_BATTERY_DETECT_EVENT        0X000010
#define APP_ALARM_EVENT                 0X000020  
#define APP_CAL_STEP_EVENT              0X000040 
#define APP_WATCH_DOG					0x000080
#define APP_EVENT_ALL                   0XFFFFFF
#define APP_NOT_DOG						0X00007F
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void App_keyHandler(void);
void App_alarmHandler(void);
void App_powerOnHandler(void);
void App_powerDownHandler(void);
void App_batteryDetectHandler(void);
void App_calStepHandler(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* APP_HANDLER_H */
