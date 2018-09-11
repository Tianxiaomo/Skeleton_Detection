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

	
    HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(192,8,2,4);   	//����ʱ��,180Mhz
    delay_init(100);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
	debug_init(DEBUG,1);			//�Ƿ������ԣ���ӡ�Ƿ����ɫ
   	LED_Init();						//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();						//��ʼ������

	debug(DEBUG,"led ok");
	
	OLED_Init();					//��ʼ��OLED��
	piclib_init();					//��ʼ����ͼ
 	debug(DEBUG,"oled ok");
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	while(SD_Init())				//��ⲻ��SD��
	{
		delay_ms(500);
		LED=!LED;					//DS0��˸
	}
	debug(DEBUG,"sd card ok");
	
	exfuns_init();					//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 			//����SD�� 
	debug(DEBUG,"sd card mount succed\r\n");
	
	Show_Str(32,32,50,50,"˼����̽",24,1,GRAYBLUE,BLACK,draw_point);	
//	Fill_Block(0,oleddev.width-1,0,oleddev.height-1,WHITE);

	
	Ui_Welcome();
	Ui_Menu();
	while(1);
}




