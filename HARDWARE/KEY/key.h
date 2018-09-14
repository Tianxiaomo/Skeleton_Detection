//#ifndef _KEY_H
//#define _KEY_H
//#include "sys.h"

////����ķ�ʽ��ͨ��λ��������ʽ��ȡIO
////#define KEY0        PHin(3) //KEY0����PH3
////#define KEY1        PHin(2) //KEY1����PH2
////#define KEY2        PCin(13)//KEY2����PC13
////#define WK_UP       PAin(0) //WKUP����PA0


////����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
//#define KEY0        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //KEY0����PH3
//#define KEY1        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)  //KEY1����PH2
//#define KEY2        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2) //KEY2����PC13

//#define KEY0_PRES 	1
//#define KEY1_PRES		2
//#define KEY2_PRES		3

//void KEY_Init(void);
//u8 KEY_Scan(u8 mode);
//#endif

/*******************************************************************************
Skeleton_Detection
�ļ���:        key.h
����   :   

�汾   :       V1.0
�޸�   :   
������ڣ�     2016.10.13
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
//�Զ��尴����ID
typedef enum
{
	KEY_HOME_ID = 1,         //home��
	KEY_MENU_ID,        //�͵��� 
	KEY_RE_ID,			//���ؼ�
	KEY_NULL_ID			//�޷��򣬼�����������  
}key_id;
/*********************************************************************
*  EXTERNAL VARIABLES
*/
/*********************************************************************
 * CONSTANTS
 */
#define LONG_PRESS_TIME       100         //10����*300����3��	
/*********************************************************************
 * MACROS
 */
#define HOME_KEY_INPUT(i) ((GPIOA->IDR & 1<<i) && 1)  //PA0  ����Ч

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
