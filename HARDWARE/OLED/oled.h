//////////////////////////////////////////////////////////////////////////////////	 
//  ��������   : OLED 4�ӿ�
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES  ��PB3
//              DC   ��PA6
//              CS   ��PA4               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : Hugaunghao
// �޸�����   : �����ļ�
//Copyright(C) 2018/9/3
//All rights reserved
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ�� 
}_oled_dev; 	  

//LCD����
extern _oled_dev oleddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


//OLEDģʽ����
//0:4�ߴ���ģʽ
#define OLED_MODE 0    /* 0Ϊ����SPIģʽ��1Ϊ80xx����*/

//#define OLED_MODE 1
		    						  
//-----------------OLED�˿ڶ���----------------  					   
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)			//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)

#define OLED_RS_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)//DC
#define OLED_RS_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)//SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)//SDA
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
//OLED�����ú���

void uDelay(unsigned char l);
void Delay(unsigned char n);
void Write_Command(unsigned char Data);
void Show_64k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Write_Data(unsigned char Data);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void Set_Write_RAM(void);
void Set_Read_RAM(void);
void Set_Remap_Format(unsigned char d);
void Set_Start_Line(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Mode(unsigned char d);
void Set_Function_Selection(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Phase_Length(unsigned char d);
void Set_Display_Enhancement(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_VSL(unsigned char d);
void Set_GPIO(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Precharge_Voltage(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_Contrast_Color(unsigned char a, unsigned char b, unsigned char c);
void Set_Master_Current(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Command_Lock(unsigned char d);
void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g);
void Fill_RAM(unsigned char a, unsigned char b);
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d, u16 e);
void Fill_Block_A(unsigned char a, unsigned char b, unsigned char c, unsigned char d,u16 e);

void OLED_Fill_Color(u16 x,u16 y,u16 width,u16 height,u16 *color);
void Checkerboard(void);
void Rainbow(void);
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void draw_point(unsigned char chXpos, unsigned char chYpos, int hwColor);//�����Ҳ����ֱ�ΪX,Y,��ɫ
void display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chSize, int hwColor,int backColor,u8 mode);//�����Ҳ����ֱ�ΪX,Y,�ַ�����ɫ
void display_Chinese1616(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, int hwColor);//�����Ҳ����ֱ�ΪX,Y,�����������е�λ�ã���ɫ
u32 oled_pow(unsigned char m,unsigned char n);
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char size,int hwColor);//�����Ҳ����ֱ�ΪX,Y,����ֵ���ֺŴ�С����ɫֵ
void Show_256k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);
void Deactivate_Scroll(void);
void Fade_In(void);
void Fade_Out(void);
void Sleep(unsigned char a);
void Test(void);
void Set_Gray_Scale_Table(void);
void OLED_Init(void);
void Set_Linear_Gray_Scale_Table(void);

#endif  
	 



