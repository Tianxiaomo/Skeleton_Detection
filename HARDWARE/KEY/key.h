//#ifndef _KEY_H
//#define _KEY_H
//#include "sys.h"

////下面的方式是通过位带操作方式读取IO
////#define KEY0        PHin(3) //KEY0按键PH3
////#define KEY1        PHin(2) //KEY1按键PH2
////#define KEY2        PCin(13)//KEY2按键PC13
////#define WK_UP       PAin(0) //WKUP按键PA0


////下面的方式是通过直接操作HAL库函数方式读取IO
//#define KEY0        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //KEY0按键PH3
//#define KEY1        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)  //KEY1按键PH2
//#define KEY2        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2) //KEY2按键PC13

//#define KEY0_PRES 	1
//#define KEY1_PRES		2
//#define KEY2_PRES		3

//void KEY_Init(void);
//u8 KEY_Scan(u8 mode);
//#endif

/*******************************************************************************
Skeleton_Detection
文件名:        key.h
描述   :   

版本   :       V1.0
修改   :   
完成日期：     2016.10.13
*******************************************************************************/
#ifndef KEY_H
#define KEY_H

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
typedef enum
{
 KEY_UP   = 0 ,
 KEY_DOWN ,  
 KEY_SHORT_PRESS,
 KEY_LONG_PRESS
}key_state;
typedef struct 
{
  u8 key_id;
  u8 key_state;
}key_t;
//自定义按键的ID
typedef enum
{
	KEY_HOME_ID = 1,         //home键
	KEY_MENU_ID,        //餐单键 
	KEY_RE_ID,			//返回键
	KEY_NULL_ID			//无方向，即按键处理完  
}key_id;
/*********************************************************************
*  EXTERNAL VARIABLES
*/
/*********************************************************************
 * CONSTANTS
 */
#define LONG_PRESS_TIME       100         //10毫秒*300等于3秒	
/*********************************************************************
 * MACROS
 */
#define HOME_KEY_INPUT(i) ((GPIOA->IDR & 1<<i) && 1)  //PA0  高有效

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
key_t *Key_getKey();
void Key_clearState(key_t *key);
void Key_Init(void);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* KEY_H */
