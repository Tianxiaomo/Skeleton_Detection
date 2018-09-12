#include "sys.h" 
#include "ff.h"	 
#include "text.h"	
#include "string.h"												    
#include "usart.h"		
#include "malloc.h"
#include "oled.h"
//////////////////////////////////////////////////////////////////////////////////	   
//用来保存字库基本信息，地址，大小等
//_font_info ftinfo;

//字库存放在磁盘中的路径
//u8*const GBK24_PATH="/SYSTEM/FONT/GBK24.FON";		//GBK24的存放位置
//u8*const GBK16_PATH="/SYSTEM/FONT/GBK16.FON";		//GBK16的存放位置
//u8*const GBK12_PATH="/SYSTEM/FONT/GBK12.FON";		//GBK12的存放位置
//u8*const UNIGBK_PATH="/SYSTEM/FONT/UNIGBK.BIN";		//UNIGBK.BIN的存放位置

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 size*2 bytes大小	 
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
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<(size*2);i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
//	foffset=((unsigned long)190*qh+ql)*(size*2);//得到字库中的字节偏移量  	//屏蔽原因，对24号字不支持
	switch (size)
	{
		case 16:
		{
			foffset=((unsigned long)190*qh+ql)*(size*2);//得到字库中的字节偏移量  
			f_open(sdfile,GBK16,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,32,&br);
			f_close(sdfile);
			break;
		}
		case 12:
		{
			foffset=((unsigned long)190*qh+ql)*(size*2);//得到字库中的字节偏移量  
			f_open(sdfile,GBK12,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,24,&br);
			f_close(sdfile);
			break;
		} 
		case 24:
		{
			foffset=((unsigned long)190*qh+ql)*(size*3);//得到字库中的字节偏移量  
			f_open(sdfile,GBK24,FA_OPEN_EXISTING|FA_READ);
			f_lseek(sdfile,foffset);
			f_read(sdfile,mat,72,&br);
			f_close(sdfile);
			break;
		} 
		
	}
	myfree(SRAMIN,sdfile);
}

//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	
//point_color:笔画颜色
//back_color:背景色
//fp:画点函数指针
void Show_Font(unsigned char x,unsigned char y,u8 *font,u8 size,u8 mode,int point_color,int back_color,Fun fp)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[100];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int point_color,int back_color,Fun fp)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }
				else display_char(x,y,*str,size,point_color,back_color,mode);
//		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(x0+width-size))//换行
						{	    
							y+=size;
							x=x0;		  
						}
	        if(y>(y0+height-size))break;//越界返回  						     
	        //Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
			Show_Font(x,y,str,size,mode,point_color,back_color,fp);
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}  			 		 
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度			  
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

























		  


















