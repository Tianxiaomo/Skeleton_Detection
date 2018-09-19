#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	
   	   		   
//IO方向设置
#define SDA1_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA1_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
//IO操作函数	 
#define IIC1_SCL    PBout(6) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define READ_SDA1   PBin(7)  //输入SDA 

//IIC1所有操作函数
void IIC1_Init(void);                //初始化IIC1的IO口				 
void IIC1_Start(void);				//发送IIC1开始信号
void IIC1_Stop(void);	  			//发送IIC1停止信号
void IIC1_Send_Byte(u8 txd);			//IIC1发送一个字节
u8 IIC1_Read_Byte(unsigned char ack);//IIC1读取一个字节
u8 IIC1_Wait_Ack(void); 				//IIC1等待ACK信号
void IIC1_Ack(void);					//IIC1发送ACK信号
void IIC1_NAck(void);				//IIC1不发送ACK信号

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);


//IO方向设置
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
//IO操作函数	 
#define IIC2_SCL    PBout(6) //SCL
#define IIC2_SDA    PBout(7) //SDA	 
#define READ_SDA2   PBin(7)  //输入SDA 

//IIC2所有操作函数
void IIC2_Init(void);                //初始化IIC2的IO口				 
void IIC2_Start(void);				//发送IIC2开始信号
void IIC2_Stop(void);	  			//发送IIC2停止信号
void IIC2_Send_Byte(u8 txd);			//IIC2发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC2读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC2等待ACK信号
void IIC2_Ack(void);					//IIC2发送ACK信号
void IIC2_NAck(void);				//IIC2不发送ACK信号

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 
#endif






