/***********************************************************************************************************************
项目名称文件名
文件名:        oled.c
描述   :       显示屏
版本   :       V1.1
修改   :   
完成日期：     2018.9.1
************************************************************************************************************************/

/***********************************************************************************************************************
* INCLUDES
*/
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "spi.h"

/***********************************************************************************************************************
* CONSTANTS
*/
//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

/***********************************************************************************************************************
* TYPEDEFS
*/

/***********************************************************************************************************************
* LOCAL VARIABLES
*/
//管理LCD重要参数
//默认为竖屏
_oled_dev oleddev;

/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
void SPI1_WriteByte(u8 TxData);

/***********************************************************************************************************************
* LOCAL FUNCTIONS  
*/
/***********************************************************************************************************************
* PUBLIC FUNCTIONS
*/

void Write_Command(unsigned char Data)
{
	OLED_CS_Clr();
	OLED_RS_Clr();
	SPI1_WriteByte(Data);
	OLED_RS_Set();
}

void Write_Data(unsigned char Data)
{
	OLED_CS_Clr();	
	OLED_RS_Set();
	SPI1_WriteByte(Data);
	OLED_RS_Set();
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x15);			// Set Column Address
	Write_Data(a);				//   Default => 0x00 (Start Address)
	Write_Data(b);				//   Default => 0x7F (End Address)
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x75);			// Set Row Address
	Write_Data(a);				//   Default => 0x00 (Start Address)
	Write_Data(b);				//   Default => 0x7F (End Address)
}


void Set_Write_RAM()
{
	Write_Command(0x5C);			// Enable MCU to Write into RAM
}


void Set_Read_RAM()
{
	Write_Command(0x5D);			// Enable MCU to Read from RAM
}


void Set_Remap_Format(unsigned char d)
{
	Write_Command(0xA0);			// Set Re-Map / Color Depth
	Write_Data(d);				//   Default => 0x40
						//     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Color Sequence: A => B => C
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
						//     65,536 Colors
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0xA1);			// Set Vertical Scroll by RAM
	Write_Data(d);				//   Default => 0x00
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xA2);			// Set Vertical Scroll by Row
	Write_Data(d);				//   Default => 0x60
}


void Set_Display_Mode(unsigned char d)
{
	Write_Command(0xA4|d);			// Set Display Mode
						//   Default => 0xA6
						//     0xA4 (0x00) => Entire Display Off, All Pixels Turn Off
						//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 63
						//     0xA6 (0x02) => Normal Display
						//     0xA7 (0x03) => Inverse Display
}


void Set_Function_Selection(unsigned char d)
{
	Write_Command(0xAB);			// Function Selection
	Write_Data(d);				//   Default => 0x01
						//     Enable Internal VDD Regulator
						//     Select 8-bit Parallel Interface
}


void Set_Display_On_Off(unsigned char d)
{
	Write_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off (Sleep Mode On)
						//     0xAF (0x01) => Display On (Sleep Mode Off)
}


void Set_Phase_Length(unsigned char d)
{
	Write_Command(0xB1);			// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	Write_Data(d);				//   Default => 0x82 (8 Display Clocks [Phase 2] / 5 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 5~31 Display Clocks
						//     D[7:4] => Phase 2 Period in 3~15 Display Clocks
}


void Set_Display_Enhancement(unsigned char d)
{
	Write_Command(0xB2);			// Display Enhancement
	Write_Data(d);				//   Default => 0x00 (Normal)
	Write_Data(0x00);
	Write_Data(0x00);
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xB3);			// Set Display Clock Divider / Oscillator Frequency
	Write_Data(d);				//   Default => 0x00
						//     A[3:0] => Display Clock Divider
						//     A[7:4] => Oscillator Frequency
}


void Set_VSL(unsigned char d)
{
	Write_Command(0xB4);			// Set Segment Low Voltage
	Write_Data(0xA0|d);			//   Default => 0xA0
						//     0xA0 (0x00) => Enable External VSL
						//     0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	Write_Data(0xB5);
	Write_Data(0x55);
}


void Set_GPIO(unsigned char d)
{
	Write_Command(0xB5);			// General Purpose IO
	Write_Data(d);				//   Default => 0x0A (GPIO Pins output Low Level.)
}


void Set_Precharge_Period(unsigned char d)
{
	Write_Command(0xB6);			// Set Second Pre-Charge Period
	Write_Data(d);				//   Default => 0x08 (8 Display Clocks)
}


void Set_Precharge_Voltage(unsigned char d)
{
	Write_Command(0xBB);			// Set Pre-Charge Voltage Level
	Write_Data(d);				//   Default => 0x17 (0.50*VCC)
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xBE);			// Set COM Deselect Voltage Level
	Write_Data(d);				//   Default => 0x05 (0.82*VCC)
}


void Set_Contrast_Color(unsigned char a, unsigned char b, unsigned char c)
{
	Write_Command(0xC1);			// Set Contrast Current for Color A, B, C
	Write_Data(a);				//   Default => 0x8A (Color A)
	Write_Data(b);				//   Default => 0x51 (Color B)
	Write_Data(c);				//   Default => 0x8A (Color C)
}


void Set_Master_Current(unsigned char d)
{
	Write_Command(0xC7);			// Master Contrast Current Control
	Write_Data(d);				//   Default => 0x0F (Maximum)
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xCA);			// Set Multiplex Ratio
	Write_Data(d);				//   Default => 0x7F (1/128 Duty)
}


void Set_Command_Lock(unsigned char d)
{
	Write_Command(0xFD);			// Set Command Lock
	Write_Data(d);				//   Default => 0x12
						//     0x12 => Driver IC interface is unlocked from entering command.
						//     0x16 => All Commands are locked except 0xFD.
						//     0xB0 => Command 0xA2, 0xB1, 0xB3, 0xBB & 0xBE are inaccessible.
						//     0xB1 => All Commands are accessible.
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define Max_Column	0x7f			// 128-1
#define Max_Row		0x7f			// 128-1
#define	Brightness	0x0F

////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End
//    c: Row Address of Start
//    d: Row Address of End
//    e: Line Width
//    f: Line Color - RRRRRGGG
//    g: Line Color - GGGBBBBB
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g)
{
unsigned char i,j;

	Set_Column_Address(a,b);
	Set_Row_Address(c,(c+e-1));
	Set_Write_RAM();
	for(i=0;i<(b-a+1);i++)
	{
		for(j=0;j<e;j++)
		{
			Write_Data(f);		// Line Color - RRRRRGGG
			Write_Data(g);		// Line Color - GGGBBBBB
		}
	}

	Set_Column_Address((b-e+1),b);
	Set_Row_Address(c,d);
	Set_Write_RAM();
	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<e;j++)
		{
			Write_Data(f);		// Line Color - RRRRRGGG
			Write_Data(g);		// Line Color - GGGBBBBB
		}
	}

	Set_Column_Address(a,b);
	Set_Row_Address((d-e+1),d);
	Set_Write_RAM();
	for(i=0;i<(b-a+1);i++)
	{
		for(j=0;j<e;j++)
		{
			Write_Data(f);		// Line Color - RRRRRGGG
			Write_Data(g);		// Line Color - GGGBBBBB
		}
	}

	Set_Column_Address(a,(a+e-1));
	Set_Row_Address(c,d);
	Set_Write_RAM();
	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<e;j++)
		{
			Write_Data(f);		// Line Color - RRRRRGGG
			Write_Data(g);		// Line Color - GGGBBBBB
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//
//    a: RRRRRGGG
//    b: GGGBBBBB
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char a, unsigned char b)
{
unsigned char i,j;

	Set_Column_Address(0x00,0x7F);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<128;i++)
	{
		for(j=0;j<128;j++)
		{
			Write_Data(a);
			Write_Data(b);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End
//    c: Row Address of Start
//    d: Row Address of End
//    e: RRRRRGGGGGGBBBBB
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d,u16 e)
{
unsigned char i,j;

	Set_Column_Address(a,b);
	Set_Row_Address(c,d);
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(e>>8);
			Write_Data(e);
		}
	}
}
void Fill_Block_A(unsigned char a, unsigned char b, unsigned char c, unsigned char d,u16 e)
{
	Fill_Block(a,a+c,b,b+d,e);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;

	Set_Column_Address(0x00,0x7F);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(0xFF);
			Write_Data(0xFF);
			Write_Data(0x00);
			Write_Data(0x00);
		}
		for(j=0;j<64;j++)
		{
			Write_Data(0x00);
			Write_Data(0x00);
			Write_Data(0xFF);
			Write_Data(0xFF);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Color Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Rainbow()
{
	// White => Column 1~16
		Fill_Block(0x00,0x0F,0x00,Max_Row,0xFFFF);

	// Yellow => Column 17~32
		Fill_Block(0x10,0x1F,0x00,Max_Row,0xFFE0);

	// Purple => Column 33~48
		Fill_Block(0x20,0x2F,0x00,Max_Row,0xF81F);

	// Cyan => Column 49~64
		Fill_Block(0x30,0x3F,0x00,Max_Row,0x07FF);

	// Red => Column 65~80
		Fill_Block(0x40,0x4F,0x00,Max_Row,0xF800);

	// Green => Column 81~96
		Fill_Block(0x50,0x5F,0x00,Max_Row,0x07E0);

	// Blue => Column 97~112
		Fill_Block(0x60,0x6F,0x00,Max_Row,0x001F);

	// Black => Column 113~128
		Fill_Block(0x70,Max_Column,0x00,Max_Row,0x0000);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_64k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;
//j=(b-a+1)*(d-c+1);
	Src_Pointer=Data_Pointer;
	Set_Column_Address(a,b);
	Set_Row_Address(c,d);	
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(*Src_Pointer);
			Src_Pointer++;
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}
void draw_point(unsigned char chXpos, unsigned char chYpos, int hwColor)//自左到右参数分别为X,Y,颜色
{

	Write_Command(0x15);			// Set Column Address
	Write_Data(chXpos);				//   Default => 0x00 (Start Address)

	Write_Command(0x75);			// Set Row Address
	Write_Data(chYpos);				//   Default => 0x00 (Start Address)
	Set_Write_RAM();
    //fill 16bit colour
	Write_Data(hwColor >> 8);
	Write_Data(hwColor);  
}
void display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chSize, int hwColor,int backColor,u8 mode)//自左到右参数分别为X,Y,字符，颜色
{      	
	uint8_t i, j, chTemp,chNum;
	uint8_t chYpos0 = chYpos;

	if(chSize == 32)chNum = 64;
	else chNum = chSize;
    for (i = 0; i < chNum; i ++) {   
		if(chSize == 12)	 chTemp = oled_asc2_1206[chChr - 0x20][i];
		else if(chSize == 16)chTemp = oled_asc2_1608[chChr - 0x20][i];
		else if(chNum == 64) chTemp = oled_asc2_3216[chChr - 0x30][i];			
        for (j = 0; j < 8; j ++) {
    		if (chTemp & 0x80) {
				draw_point(chXpos, chYpos, hwColor);
    		}	
			else if(mode) draw_point(chXpos, chYpos, backColor);				
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}  	 
    } 
}


u32 oled_pow(unsigned char m,unsigned char n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char size,int hwColor)//自左到右参数分别为X,Y,整数值，字号大小，颜色值
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				display_char(x+(size/2)*t,y,' ',size,hwColor,0,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	display_char(x+(size/2)*t,y,temp+'0',size,hwColor,0,0); 
	}
}
void Show_256k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned char *Src_Pointer;
	unsigned char i,j;
	Src_Pointer=Data_Pointer;
	Set_Column_Address(a,b);
	Set_Row_Address(c,d);	
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(*Src_Pointer);
			Src_Pointer++;
			Write_Data(*Src_Pointer);
			Src_Pointer++;
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}

void OLED_Fill_Color(u16 x,u16 y,u16 width,u16 height,u16 *color)
{  
	u16 *Src_Pointer;
	unsigned char i,j;
	Src_Pointer=color;
	Set_Column_Address(x,x+width-1);
	Set_Row_Address(y,y+height-1);	
	Set_Write_RAM();

	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			Write_Data((*Src_Pointer)>>8);
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
unsigned int i,j;	

	switch(a)
	{
		case 0:
			for(i=0;i<128;i+=b)
			{
				Set_Start_Line(i);
				for(j=0;j<c;j++)
				{
					delay_us(200);
				}
			}
			break;
		case 1:
			for(i=0;i<128;i+=b)
			{
				Set_Start_Line(128-i);
				for(j=0;j<c;j++)
				{
					delay_us(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Automatic Horizontal Scrolling (Instruction)
//
//    a: Scrolling Direction
//       "0x00" (Rightward)
//       "0x01" (Leftward)
//    b: Set Numbers of Column Scroll per Step
//    c: Set Row Address of Start
//    d: Set Numbers of Row to Be Scrolled
//    e: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    f: delay_ms Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f)
{
	Write_Command(0x96);			// Horizontal Scroll Setup
	Write_Data((a<<7)|b);
	Write_Data(c);
	Write_Data(d);
	Write_Data(0x00);
	Write_Data(e);
	Write_Command(0x9F);			// Activate Horizontal Scroll
	delay_ms(f);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x9E);			// Deactivate Scrolling
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
unsigned int i;	

	Set_Display_On_Off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Master_Current(i);
		delay_us(200);
		delay_us(200);
		delay_us(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
unsigned int i;	

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Master_Current(i-1);
		delay_us(200);
		delay_us(200);
		delay_us(200);
	}
	Set_Display_On_Off(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x00" Enter Sleep Mode
//    "0x01" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0x00);
			Set_Display_Mode(0x01);
			break;
		case 1:
			Set_Display_Mode(0x02);
			Set_Display_On_Off(0x01);
			break;
	}
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);
	Write_Data(0x02);			// Gray Scale Level 1
	Write_Data(0x03);			// Gray Scale Level 2
	Write_Data(0x04);			// Gray Scale Level 3
	Write_Data(0x05);			// Gray Scale Level 4
	Write_Data(0x06);			// Gray Scale Level 5
	Write_Data(0x07);			// Gray Scale Level 6
	Write_Data(0x08);			// Gray Scale Level 7
	Write_Data(0x09);			// Gray Scale Level 8
	Write_Data(0x0A);			// Gray Scale Level 9
	Write_Data(0x0B);			// Gray Scale Level 10
	Write_Data(0x0C);			// Gray Scale Level 11
	Write_Data(0x0D);			// Gray Scale Level 12
	Write_Data(0x0E);			// Gray Scale Level 13
	Write_Data(0x0F);			// Gray Scale Level 14
	Write_Data(0x10);			// Gray Scale Level 15
	Write_Data(0x11);			// Gray Scale Level 16
	Write_Data(0x12);			// Gray Scale Level 17
	Write_Data(0x13);			// Gray Scale Level 18
	Write_Data(0x15);			// Gray Scale Level 19
	Write_Data(0x17);			// Gray Scale Level 20
	Write_Data(0x19);			// Gray Scale Level 21
	Write_Data(0x1B);			// Gray Scale Level 22
	Write_Data(0x1D);			// Gray Scale Level 23
	Write_Data(0x1F);			// Gray Scale Level 24
	Write_Data(0x21);			// Gray Scale Level 25
	Write_Data(0x23);			// Gray Scale Level 26
	Write_Data(0x25);			// Gray Scale Level 27
	Write_Data(0x27);			// Gray Scale Level 28
	Write_Data(0x2A);			// Gray Scale Level 29
	Write_Data(0x2D);			// Gray Scale Level 30
	Write_Data(0x30);			// Gray Scale Level 31
	Write_Data(0x33);			// Gray Scale Level 32
	Write_Data(0x36);			// Gray Scale Level 33
	Write_Data(0x39);			// Gray Scale Level 34
	Write_Data(0x3C);			// Gray Scale Level 35
	Write_Data(0x3F);			// Gray Scale Level 36
	Write_Data(0x42);			// Gray Scale Level 37
	Write_Data(0x45);			// Gray Scale Level 38
	Write_Data(0x48);			// Gray Scale Level 39
	Write_Data(0x4C);			// Gray Scale Level 40
	Write_Data(0x50);			// Gray Scale Level 41
	Write_Data(0x54);			// Gray Scale Level 42
	Write_Data(0x58);			// Gray Scale Level 43
	Write_Data(0x5C);			// Gray Scale Level 44
	Write_Data(0x60);			// Gray Scale Level 45
	Write_Data(0x64);			// Gray Scale Level 46
	Write_Data(0x68);			// Gray Scale Level 47
	Write_Data(0x6C);			// Gray Scale Level 48
	Write_Data(0x70);			// Gray Scale Level 49
	Write_Data(0x74);			// Gray Scale Level 50
	Write_Data(0x78);			// Gray Scale Level 51
	Write_Data(0x7D);			// Gray Scale Level 52
	Write_Data(0x82);			// Gray Scale Level 53
	Write_Data(0x87);			// Gray Scale Level 54
	Write_Data(0x8C);			// Gray Scale Level 55
	Write_Data(0x91);			// Gray Scale Level 56
	Write_Data(0x96);			// Gray Scale Level 57
	Write_Data(0x9B);			// Gray Scale Level 58
	Write_Data(0xA0);			// Gray Scale Level 59
	Write_Data(0xA5);			// Gray Scale Level 60
	Write_Data(0xAA);			// Gray Scale Level 61
	Write_Data(0xAF);			// Gray Scale Level 62
	Write_Data(0xB4);			// Gray Scale Level 63
}


void Set_Linear_Gray_Scale_Table()
{
	Write_Command(0xB9);			// Default
}

void OLED_Init()
{
 	GPIO_InitTypeDef  GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();//使能GPIOA,B时钟

	GPIO_Initure.Pin = GPIO_PIN_4|GPIO_PIN_6; //PB1,0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4|GPIO_PIN_6,GPIO_PIN_SET);

	__HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIOA,B时钟

	GPIO_Initure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);

	SPI1_Init();
	
	OLED_RST_Clr();
		delay_ms(1);
	OLED_RST_Set(); 
	OLED_RST_Set();
	Set_Command_Lock(0x12);			// Unlock Driver IC (0x12/0x16/0xB0/0xB1)
	Set_Command_Lock(0xB1);			// Unlock All Commands (0x12/0x16/0xB0/0xB1)
	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0xF1);		// Set Clock as 90 Frames/Sec
	Set_Multiplex_Ratio(0x7F);		// 1/128 Duty (0x0F~0x7F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x7F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x7F)
	Set_Remap_Format(0x74);			// Set Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Color Sequence D[15:0]=[RRRRR:GGGGGG:BBBBB]
						//     Scan from COM127 to COM0
						//     Enable COM Split Odd Even
						//     65,536 Colors Mode (0x74)
						//     * 262,144 Colors Mode (0xB4)
	Set_GPIO(0x00);				// Disable GPIO Pins Input
	Set_Function_Selection(0x01);		// Enable Internal VDD Regulator
						// Select 8-bit Parallel Interface
	Set_VSL(0xA0);				// Enable External VSL
	Set_Contrast_Color(0xC8,0x80,0xC8);	// Set Contrast of Color A (Red)
						// Set Contrast of Color B (Green)
						// Set Contrast of Color C (Blue)
	Set_Master_Current(Brightness);		// Set Scale Factor of Segment Output Current Control
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Phase_Length(0x32);			// Set Phase 1 as 5 Clocks & Phase 2 as 3 Clocks
	Set_Precharge_Voltage(0x17);		// Set Pre-Charge Voltage Level as 0.50*VCC
	Set_Display_Enhancement(0xA4);		// Enhance Display Performance
	Set_Precharge_Period(0x01);		// Set Second Pre-Charge Period as 1 Clock
	Set_VCOMH(0x05);			// Set Common Pins Deselect Voltage Level as 0.82*VCC
	Set_Display_Mode(0x02);			// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(0x00,0x00);			// Clear Screen

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
	
	oleddev.width = 128;
	oleddev.height = 128;
	oleddev.dir		= 0;
}

/***********************************************************************************************************************
************************************************************************************************************************/

