//////////////////////////////////////////////////////////////////////////////////	 
//  功能描述   : OLED 4接口
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES  接PB3
//              DC   接PA6
//              CS   接PA4               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : Hugaunghao
// 修改内容   : 创建文件
//Copyright(C) 2018/9/3
//All rights reserved
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 
//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_oled_dev; 	  

//LCD参数
extern _oled_dev oleddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

//画笔颜色
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//OLED模式设置
//0:4线串行模式
#define OLED_MODE 0    /* 0为四线SPI模式，1为80xx并口*/

//#define OLED_MODE 1
		    						  
//-----------------OLED端口定义----------------  					   
//-----------------OLED端口定义----------------  					   
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

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数

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
void draw_point(unsigned char chXpos, unsigned char chYpos, int hwColor);//自左到右参数分别为X,Y,颜色
void display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chSize, int hwColor,int backColor,u8 mode);//自左到右参数分别为X,Y,字符，颜色
void display_Chinese1616(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, int hwColor);//自左到右参数分别为X,Y,汉字在数组中的位置，颜色
u32 oled_pow(unsigned char m,unsigned char n);
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char size,int hwColor);//自左到右参数分别为X,Y,整数值，字号大小，颜色值
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
	 



