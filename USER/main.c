#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
//#include "lcd.h"
#include "string.h"
//#include "sdram.h"
#include "malloc.h"
//#include "w25qxx.h"
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
/************************************************
 ALIENTEK ������STM32F429������ʵ��43
 ͼƬ��ʾʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ	    			     
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//�����ڴ�
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼ 
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�  	 
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//������/��ĩβ��,�˳�	 		 
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);//�ͷ��ڴ�
	return rval;
}

int main(void)
{	
	u8 res;
 	DIR picdir;	 			//ͼƬĿ¼
	FILINFO *picfileinfo;	//�ļ���Ϣ 
	u8 *pname;				//��·�����ļ���
	u16 totpicnum; 			//ͼƬ�ļ�����
	u16 curindex;			//ͼƬ��ǰ����
	u8 key;					//��ֵ
	u8 pause=0;				//��ͣ���
	u8 t;
	u16 temp;
	u32 *picoffsettbl;	//ͼƬ�ļ�offset������ 
    
	FIL *sdfile;
	char char_temp[100];
	UINT br;
	
    HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(192,8,2,4);   	//����ʱ��,180Mhz
    delay_init(100);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
   	LED_Init();						//��ʼ����LED���ӵ�Ӳ���ӿ�
	printf("led succed\r\n");

	KEY_Init();					//��ʼ������
	OLED_Init();				//��ʼ��OLED��
	piclib_init();				//��ʼ����ͼ
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	while(SD_Init())			//��ⲻ��SD��
	{
		delay_ms(500);
		LED=!LED;				//DS0��˸
	}
	printf("sd card ok \r\n");
	
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
	printf("sd card mount succed\r\n");

	printf("%d \r\n",font_init());

	sdfile=(FIL*)mymalloc(SRAMIN,sizeof(sdfile));
	printf("%d\r\n",f_open(sdfile,"0:/2018-09-05-16-27-20.txt",FA_OPEN_EXISTING|FA_READ));
	printf("%d\r\n",f_read(sdfile,char_temp,72,&br));
	f_close(sdfile);
	myfree(SRAMIN,sdfile);
	
	printf("%s\r\n",char_temp);

	Show_Str(32,32,50,50,"��Һ�,����������",16,0,GREEN,BLACK,draw_point);
	
 	if(f_opendir(&picdir,"0:/PICTURE"))//��ͼƬ�ļ���
 	{	    
		Show_Str(10,10,100,100,"PICTURE�ļ��д���!",16,0,GREEN,BLACK,draw_point);
		delay_ms(200);				  
		Fill_Block(10,110,10,110,BLACK);//�����ʾ	     
		delay_ms(200);
		printf("picture �ļ��д���\r\n");
	}  
	totpicnum=pic_get_tnum("0:/PICTURE"); //�õ�����Ч�ļ���
  	while(totpicnum==NULL)//ͼƬ�ļ�Ϊ0		
 	{	    
		Show_Str(10,10,100,100,"û��ͼƬ�ļ�!",16,0,GREEN,BLACK,draw_point);
		delay_ms(200);				  
		Fill_Block(10,110,10,110,BLACK);//�����ʾ	     
		delay_ms(200);				  
	}
	printf("This is %d picture\r\n",totpicnum);
	picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//�����ڴ�
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);					//Ϊ��·�����ļ��������ڴ�
 	picoffsettbl=mymalloc(SRAMIN,4*totpicnum);					//����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
 	while(!picfileinfo||!pname||!picoffsettbl)					//�ڴ�������
 	{	    	
		Show_Str(10,10,100,100,"�ڴ����ʧ��!",16,0,GREEN,BLACK,draw_point);
		delay_ms(200);				  
		Fill_Block(10,110,10,110,BLACK);//�����ʾ	     
		delay_ms(200);				  				  
	}  	
	//��¼����
    res=f_opendir(&picdir,"0:/PICTURE"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=picdir.dptr;								//��¼��ǰdptrƫ��
	        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�	 	 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				picoffsettbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}   
	Show_Str(10,10,100,100,"��ʼ��ʾ...",16,0,GREEN,BLACK,draw_point);
	delay_ms(1500);
	piclib_init();										//��ʼ����ͼ	   	   
	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//��Ŀ¼
		
	dir_sdi(&picdir,picoffsettbl[curindex]);			//�ı䵱ǰĿ¼����	   
	res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
	strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
	Fill_Block(0,110,0,110,BLACK);;
	ai_load_picfile(pname,0,0,oleddev.width,oleddev.height,1);//��ʾͼƬ   

//	while(res==FR_OK)//�򿪳ɹ�
//	{	
//		dir_sdi(&picdir,picoffsettbl[curindex]);			//�ı䵱ǰĿ¼����	   
//        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
//		strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
//		strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
// 		Fill_Block(0,110,0,110,BLACK);;
// 		ai_load_picfile(pname,0,0,oleddev.width,oleddev.height,1);//��ʾͼƬ    
//		Show_Str(2,2,oleddev.width,16,pname,16,0,GREEN,BLACK,draw_point); 				//��ʾͼƬ����
//		printf("%s\r\n",pname);
//		t=0;
//		while(1) 
//		{
//			key=KEY_Scan(0);		//ɨ�谴��
//			if(t>250)key=1;			//ģ��һ�ΰ���KEY0    
//			if((t%20)==0)LED=!LED;//LED0��˸,��ʾ������������.
//			if(key==KEY2_PRES)		//��һ��
//			{
//				if(curindex)curindex--;
//				else curindex=totpicnum-1;
//				break;
//			}else if(key==KEY0_PRES)//��һ��
//			{
//				curindex++;		   	
//				if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
//				break;
//			}
//			if(pause==0)t++;
//			delay_ms(10); 
//		}					    
//		res=0;  
//	} 		
	while(1);
	myfree(SRAMIN,picfileinfo);			//�ͷ��ڴ�						   		    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,picoffsettbl);			//�ͷ��ڴ�
}



