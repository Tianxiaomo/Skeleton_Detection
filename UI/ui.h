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
typedef enum
{ 
stop,
detecting,
suspend
}detect_state;

typedef enum
{ 
pattern_one,
pattern_two,
pattern_three
}detect_pattern;

typedef struct
{
	u8 detect_pattern;
	detect_state detect_status;
}detect_t;
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

#define UI_FIRST_PAGE			(UI_HOME_PAGE|UI_PATTERN_1_PAGE|UI_PATTERN_2_PAGE|UI_PATTERN_3_PAGE|UI_SET_PAGE|UI_FILE_PAGE)
#define UI_HOME_PAGE            0X000001
#define UI_PATTERN_1_PAGE       0X000002
#define UI_PATTERN_2_PAGE       0X000004
#define UI_PATTERN_3_PAGE       0X000008
#define UI_SET_PAGE             0X000010
#define UI_FILE_PAGE            0X000020

#define UI_SECOND_PAGE			(UI_DETECTION1_PAGE|UI_DETECTION2_PAGE|UI_DETECTION3_PAGE|UI_SET_SUB_PAGE|UI_FILE_SUB_PAGE)
#define UI_DETECTION1_PAGE      0X000040
#define UI_DETECTION2_PAGE      0X000080
#define UI_DETECTION3_PAGE      0X000100
#define UI_SET_SUB_PAGE         0X000200
#define UI_FILE_SUB_PAGE        0X000400


#define UI_DETECT_1_PAGE		0X000800
#define UI_DETECT_2_PAGE		0X001000
#define UI_DETECT_3_PAGE		0X002000

#define UI_SET_TIME_PAGE		0X004000
#define UI_SET_LAG_PAGE		  	0X008000
#define UI_SET_SD_PAGE			0X010000
#define UI_ALL_PAGE			  	0XFFFFFF

////一级菜单
//#define UI_HOME_PAGE            0X000001
//#define UI_PATTERN_1_PAGE       0X000002
//#define UI_PATTERN_2_PAGE       0X000004
//#define UI_SET_PAGE             0X000008
//#define UI_FILE_PAGE            0X000010
//#define UI_ALL_FIRST			0X0000FF
////二级菜单		
//#define UI_DETECTION_PAGE       0X000100
//#define UI_SET_SUB_PAGE         0X000200
//#define UI_FILE_SUB_PAGE        0X000400
////三级菜单
//#define UI_SET_TIME_PAGE		0X001000
//#define UI_SET_LAG_PAGE		  	0X002000
//#define UI_SET_SD_PAGE			0X004000
					
				

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
#define SET_ICO			"0:/SYSTEM/ICO/settings.jpg"
#define FILE_ICO		"0:/SYSTEM/ICO/folder.jpg"
#define LOADING_ICO		"0:/SYSTEM/ICO/autorenew.jpg"

#define LEFT_ICO		"0:/SYSTEM/ICO/chevron-left.jpg"
#define RIGHT_ICO		"0:/SYSTEM/ICO/chevron-right.jpg"
#define MORE_ICO		"0:/SYSTEM/ICO/dots-horizontal.jpg"

#define TIME_ICO		"0:/SYSTEM/ICO/av-timer.jpg"
#define CLOCK_ICO		"0:/SYSTEM/ICO/calendar-clock.jpg"
#define LANG_ICO		"0:/SYSTEM/ICO/language.jpg"
#define CONTENT_ICO		"0:/SYSTEM/ICO/content-save.jpg"
#define ABOUT_ICO		"0:/SYSTEM/ICO/about.jpg"

#define pic_wel		"0:/SYSTEM/PICTURE/1.gif"
#define pic_menu 	"0:/SYSTEM/PICTURE/MENU.jpg"
#define Menu_PC 			WHITE
#define Menu_BC 			WHITE
#define TIME_COLOR			BLACK
#define BACKGROUD_COLOR		WHITE
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void Ui_stateSet(u32 ui_page,u8 ui_power);
ui_state_t *Ui_stateGet(void);
void Ui_postPage(u32 page);
void Ui_poll(void);
void Ui_init(void);
void Ui_delPage(u32 page);	
void Ui_showAdvertPage(void);
void Ui_showString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 Char_Num);

void Ui_Welcome(void);
void Ui_Menu(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
