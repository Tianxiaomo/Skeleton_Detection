/*******************************************************************************
Skeleton_Detection
文件名:        ui.h
描述   : 

版本   :       V1.0
修改   :   
完成日期：     2016.10.14

*******************************************************************************/
#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
#include "sys.h"	
/*********************************************************************
 * TYPEDEFS
 */
#pragma pack(1)
typedef struct 
{
	u32 ui_page;	//4级菜单，31-24 1级，23-16 2级， 15-8 3级，7-0 4级
	u8  ui_power;
}ui_state_t;
#pragma pack()
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define UI_POWER_ON             1
#define UI_POWER_OFF            0

#define UI_PAGE_SIZE            12
#define UI_NO_PAGE              0X8000
#define UI_MAIN_PAGE            0X0001
#define UI_PATTERN_1_PAGE       0X0002
#define UI_PATTERN_2_PAGE       0X0004
#define UI_SET_PAGE             0X0008
#define UI_FILE_PAGE            0X0010
#define UI_DETECTION_PAGE       0X0020
#define UI_SET_SUB_PAGE         0X0040
#define UI_FILE_SUB_PAGE        0X0080
#define UI_SET_TIME_PAGE		0X0100
#define UI_SET_LAG_PAGE		  	0X0200
#define UI_SET_SD_PAGE			0X0400
#define UI_ALL_PAGE			  	0XFFFF


#define BATTERY_JPG_10			"0:/SYSTEM/ICO/battery-10.jpg"
#define BATTERY_JPG_20			"0:/SYSTEM/ICO/battery-20.jpg"
#define BATTERY_JPG_30			"0:/SYSTEM/ICO/battery-30.jpg"
#define BATTERY_JPG_40			"0:/SYSTEM/ICO/battery-40.jpg"
#define BATTERY_JPG_50			"0:/SYSTEM/ICO/battery-50.jpg"
#define BATTERY_JPG_60			"0:/SYSTEM/ICO/battery-60.jpg"
#define BATTERY_JPG_70			"0:/SYSTEM/ICO/battery-70.jpg"
#define BATTERY_JPG_80			"0:/SYSTEM/ICO/battery-80.jpg"
#define BATTERY_JPG_90			"0:/SYSTEM/ICO/battery-90.jpg"
#define BATTERY_JPG_100			"0:/SYSTEM/ICO/battery-100.jpg"

#define BATTERY_CHARGING_JPG_10		"0:/SYSTEM/ICO/battery-charging-10.jpg"
#define BATTERY_CHARGING_JPG_20		"0:/SYSTEM/ICO/battery-charging-20.jpg"
#define BATTERY_CHARGING_JPG_30		"0:/SYSTEM/ICO/battery-charging-30.jpg"
#define BATTERY_CHARGING_JPG_40		"0:/SYSTEM/ICO/battery-charging-40.jpg"
#define BATTERY_CHARGING_JPG_50		"0:/SYSTEM/ICO/battery-charging-50.jpg"
#define BATTERY_CHARGING_JPG_60		"0:/SYSTEM/ICO/battery-charging-60.jpg"
#define BATTERY_CHARGING_JPG_70		"0:/SYSTEM/ICO/battery-charging-70.jpg"
#define BATTERY_CHARGING_JPG_80		"0:/SYSTEM/ICO/battery-charging-80.jpg"
#define BATTERY_CHARGING_JPG_90		"0:/SYSTEM/ICO/battery-charging-90.jpg"
#define BATTERY_CHARGING_JPG_100	"0:/SYSTEM/ICO/battery-charging-100.jpg"

#define LOGO_ICO		"0:/SYSTEM/ICO/logo.jpg"
#define PATTERY_1_ICO	"0:/SYSTEM/ICO/detection-1.jpg"
#define PATTERY_2_ICO	"0:/SYSTEM/ICO/detection-2.jpg"
#define PATTERY_3_ICO	"0:/SYSTEM/ICO/detection-3.jpg"
#define SET_ICO			"0:/SYSTEM/ICO/seting.jpg"
#define FILE_ICO		"0:/SYSTEM/ICO/folder.jpg"
#define LOADING_ICO		"0:/SYSTEM/ICO/autorenew.jpg"

#define TIME_ICO		"0:/SYSTEM/ICO/ac-timer.jpg"
#define CLOCK_ICO		"0:/SYSTEM/ICO/calendar-clock.jpg"
#define LANG_ICO		"0:/SYSTEM/ICO/language.jpg"
#define CONTENT_ICO		"0:/SYSTEM/ICO/content-save,jpg"
#define ABOUT_ICO		"0:/SYSTEM/ICO/about.jpg"

#define pic_wel		"0:/SYSTEM/PICTURE/1.gif"
#define pic_menu 	"0:/SYSTEM/PICTURE/MENU.jpg"
#define Menu_PC 			BLACK
#define Menu_BC 			WHITE
#define TIME_COLOR			WHITE
#define BACKGROUD_COLOR		BLACK
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
ui_state_t * Ui_stateGet(void);
void Ui_postPage(u32 page);
void Ui_poll(void);
void Ui_init(void);
void Ui_delPage(u32 page);	
void Ui_showAdvertPage(void);

void Ui_Welcome(void);
void Ui_Menu(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
