#include "myiic.h"
#include "delay.h"

//��ʼ��IIC1
void IIC1_Init(void)
{			
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��

	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7; //PB1,0
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0��1 

	IIC1_SCL=1;
	IIC1_SDA=1;
}
//����IIC1��ʼ�ź�
void IIC1_Start(void)
{
	SDA1_OUT();     //sda�����
	IIC1_SDA=1;	  	  
	IIC1_SCL=1;
	delay_us(4);
 	IIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC1_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IIC1ֹͣ�ź�
void IIC1_Stop(void)
{
	SDA1_OUT();//sda�����
	IIC1_SCL=1;
	IIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
//	IIC1_SCL=1; 
	IIC1_SDA=1;//����I2C���߽����ź�
	delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA1_IN();      //SDA����Ϊ����  
	IIC1_SDA=1;delay_us(1);	   
	IIC1_SCL=1;delay_us(1);	 
	while(READ_SDA1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC1_Ack(void)
{
	IIC1_SCL=0;
	SDA1_OUT();
	IIC1_SDA=0;
	delay_us(2);
	IIC1_SCL=1;
	delay_us(2);
	IIC1_SCL=0;
}
//������ACKӦ��		    
void IIC1_NAck(void)
{
	IIC1_SCL=0;
	SDA1_OUT();
	IIC1_SDA=1;
	delay_us(2);
	IIC1_SCL=1;
	delay_us(2);
	IIC1_SCL=0;
}					 				     
//IIC1����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA1_OUT(); 	    
    IIC1_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC1_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC1_SCL=1;
		delay_us(2); 
		IIC1_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA1_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC1_SCL=0; 
        delay_us(2);
		IIC1_SCL=1;
        receive<<=1;
        if(READ_SDA1)receive++;   
		delay_us(2); 
    }					 
    if (!ack)
        IIC1_NAck();//����nACK
    else
        IIC1_Ack(); //����ACK   
    return receive;
}



/********************����*****************************************/


//��ʼ��IIC2
void IIC2_Init(void)
{			
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��

	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7; //PB1,0
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0��1 

	IIC2_SCL=1;
	IIC2_SDA=1;
}
//����IIC2��ʼ�ź�
void IIC2_Start(void)
{
	SDA2_OUT();     //sda�����
	IIC2_SDA=1;	  	  
	IIC2_SCL=1;
	delay_us(4);
 	IIC2_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC2_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IIC2ֹͣ�ź�
void IIC2_Stop(void)
{
	SDA2_OUT();//sda�����
	IIC2_SCL=1;
	IIC2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
//	IIC2_SCL=1; 
	IIC2_SDA=1;//����I2C���߽����ź�
	delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA2_IN();      //SDA����Ϊ����  
	IIC2_SDA=1;delay_us(1);	   
	IIC2_SCL=1;delay_us(1);	 
	while(READ_SDA2)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	IIC2_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC2_Ack(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=0;
	delay_us(2);
	IIC2_SCL=1;
	delay_us(2);
	IIC2_SCL=0;
}
//������ACKӦ��		    
void IIC2_NAck(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=1;
	delay_us(2);
	IIC2_SCL=1;
	delay_us(2);
	IIC2_SCL=0;
}					 				     
//IIC2����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC2_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA2_OUT(); 	    
    IIC2_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC2_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC2_SCL=1;
		delay_us(2); 
		IIC2_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA2_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC2_SCL=0; 
        delay_us(2);
		IIC2_SCL=1;
        receive<<=1;
        if(READ_SDA2)receive++;   
		delay_us(2); 
    }					 
    if (!ack)
        IIC2_NAck();//����nACK
    else
        IIC2_Ack(); //����ACK   
    return receive;
}





