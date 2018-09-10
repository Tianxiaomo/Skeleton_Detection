#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "string.h"
#include "malloc.h"
#include "oled.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"
#include "ui.h"
#include "log.h"
#include "rtc.h"

int main(void)
{	

	
    HAL_Init();                     //初始化HAL库   
	Stm32_Clock_Init(192,8,2,4);   	//设置时钟,180Mhz
    delay_init(100);                //初始化延时函数
    uart_init(115200);              //初始化USART
	debug_init(DEBUG,1);			//是否开启调试，打印是否带颜色
   	LED_Init();						//初始化与LED连接的硬件接口
	KEY_Init();						//初始化按键

	debug(DEBUG,"led ok");
	
	OLED_Init();					//初始化OLED屏
	piclib_init();					//初始化画图
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	while(SD_Init())				//检测不到SD卡
	{
		delay_ms(500);
		LED=!LED;					//DS0闪烁
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 			//挂载SD卡 
	debug(DEBUG,"sd card mount succed\r\n");
	
	Show_Str(32,32,50,50,"思博声探",24,1,GRAYBLUE,BLACK,draw_point);	
//	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);

	
	Ui_Welcome();
	Ui_Menu();
	while(1);
}




