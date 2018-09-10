#ifndef __TEXT_H__
#define __TEXT_H__	  
#include "fontupd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������ 
//������ʾ ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#define GBK16 "0:SYSTEM/FONT/GBK16.fon"
#define GBK12 "0:SYSTEM/FONT/GBK12.fon"
#define GBK24 "0:SYSTEM/FONT/GBK24.fon"


typedef void(*Fun)(unsigned char,unsigned char,int);

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void Show_Font(unsigned char x,unsigned char y,u8 *font,u8 size,u8 mode,int point_color,int back_color,Fun fp);					//��ָ��λ����ʾһ������
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int point_color,int back_color,Fun fp);	//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
#endif
