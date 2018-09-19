#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	
   	   		   
//IO��������
#define SDA1_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA1_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define IIC1_SCL    PBout(6) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define READ_SDA1   PBin(7)  //����SDA 

//IIC1���в�������
void IIC1_Init(void);                //��ʼ��IIC1��IO��				 
void IIC1_Start(void);				//����IIC1��ʼ�ź�
void IIC1_Stop(void);	  			//����IIC1ֹͣ�ź�
void IIC1_Send_Byte(u8 txd);			//IIC1����һ���ֽ�
u8 IIC1_Read_Byte(unsigned char ack);//IIC1��ȡһ���ֽ�
u8 IIC1_Wait_Ack(void); 				//IIC1�ȴ�ACK�ź�
void IIC1_Ack(void);					//IIC1����ACK�ź�
void IIC1_NAck(void);				//IIC1������ACK�ź�

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);


//IO��������
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define IIC2_SCL    PBout(6) //SCL
#define IIC2_SDA    PBout(7) //SDA	 
#define READ_SDA2   PBin(7)  //����SDA 

//IIC2���в�������
void IIC2_Init(void);                //��ʼ��IIC2��IO��				 
void IIC2_Start(void);				//����IIC2��ʼ�ź�
void IIC2_Stop(void);	  			//����IIC2ֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC2����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC2�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC2����ACK�ź�
void IIC2_NAck(void);				//IIC2������ACK�ź�

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 
#endif






