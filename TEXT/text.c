#include "sys.h" 
#include "ff.h"	 
#include "text.h"	
#include "string.h"												    
#include "usart.h"		
#include "malloc.h"
#include "oled.h"
//////////////////////////////////////////////////////////////////////////////////	   
//���������ֿ������Ϣ����ַ����С��
//_font_info ftinfo;

//�ֿ����ڴ����е�·��
//u8*const GBK24_PATH="/SYSTEM/FONT/GBK24.FON";		//GBK24�Ĵ��λ��
//u8*const GBK16_PATH="/SYSTEM/FONT/GBK16.FON";		//GBK16�Ĵ��λ��
//u8*const GBK12_PATH="/SYSTEM/FONT/GBK12.FON";		//GBK12�Ĵ��λ��
//u8*const UNIGBK_PATH="/SYSTEM/FONT/UNIGBK.BIN";		//UNIGBK.BIN�Ĵ��λ��

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ size*2 bytes��С	 
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
	FIL *sdfile;
	UINT br;		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset;
	sdfile=(FIL*)mymalloc(SRAMIN,sizeof(FIL)); 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<(size*2);i++)*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
//	foffset=((unsigned long)190*qh+ql)*(size*2);//�õ��ֿ��е��ֽ�ƫ����  	//����ԭ�򣬶�24���ֲ�֧��
	switch (size)
	{
		case 16:
		{
			foffset=((unsigned long)190*qh+ql)*(size*2);//�õ��ֿ��е��ֽ�ƫ����  
			f_open(sdfile,GBK16,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,32,&br);
			f_close(sdfile);
			break;
		}
		case 12:
		{
			foffset=((unsigned long)190*qh+ql)*(size*2);//�õ��ֿ��е��ֽ�ƫ����  
			f_open(sdfile,GBK12,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,24,&br);
			f_close(sdfile);
			break;
		} 
		case 24:
		{
			foffset=((unsigned long)190*qh+ql)*(size*3);//�õ��ֿ��е��ֽ�ƫ����  
			f_open(sdfile,GBK24,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,72,&br);
			f_close(sdfile);
			break;
		} 
		
	}
	myfree(SRAMIN,sdfile);
}

//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	
//point_color:�ʻ���ɫ
//back_color:����ɫ
//fp:���㺯��ָ��
void Show_Font(unsigned char x,unsigned char y,u8 *font,u8 size,u8 mode,int point_color,int back_color,Fun fp)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[100];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)fp(x,y,point_color);
			else if(mode)fp(x,y,back_color); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int point_color,int back_color,Fun fp)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }
				else display_char(x,y,*str,size,point_color,back_color,mode);
//		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
						{	    
							y+=size;
							x=x0;		  
						}
	        if(y>(y0+height-size))break;//Խ�緵��  						     
	        //Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
			Show_Font(x,y,str,size,mode,point_color,back_color,fp);
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
//void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
//{
//	u16 strlenth=0;
//   	strlenth=strlen((const char*)str);
//	strlenth*=size/2;
//	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
//	else
//	{
//		strlenth=(len-strlenth)/2;
//	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
//	}
//}   

























		  


















