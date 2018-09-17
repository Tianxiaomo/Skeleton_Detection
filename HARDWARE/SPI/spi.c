/***********************************************************************************************************************
��Ŀ�����ļ���
�ļ���:        rtc.c
����   :       ʵʱʱ��
�汾   :       V1.0
�޸�   :   
������ڣ�     2017.4.19
************************************************************************************************************************/

/***********************************************************************************************************************
* INCLUDES
*/
#include "spi.h"

/***********************************************************************************************************************
* CONSTANTS
*/
/***********************************************************************************************************************
* TYPEDEFS
*/
/***********************************************************************************************************************
* LOCAL VARIABLES
*/
SPI_HandleTypeDef SPI1_Handler;  //SPI���
SPI_HandleTypeDef SPI2_Handler;  //SPI���


/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
/***********************************************************************************************************************
* LOCAL FUNCTIONS  
*/
/***********************************************************************************************************************
* PUBLIC FUNCTIONS
*/
void SPI1_Init(void)
{
	SPI1_Handler.Instance=SPI1;                         //SP1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   	//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);						//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //ʹ��SPI1
}

void SPI2_Init(void)
{
	u8 t = 0xff;
    SPI2_Handler.Instance=SPI2;                         //SP2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI2_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI2_Handler);
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //ʹ��SPI2
	 HAL_SPI_Transmit(&SPI2_Handler,&t,1,1000);
}

//SPI2�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	if(hspi == &SPI2_Handler){
		__HAL_RCC_GPIOB_CLK_ENABLE();       //ʹ��GPIOBʱ��
		__HAL_RCC_SPI2_CLK_ENABLE();        //ʹ��SPI2ʱ��

		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
		GPIO_Initure.Alternate=GPIO_AF5_SPI2;           //����ΪSPI2
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}else if(hspi == &SPI1_Handler){
		__HAL_RCC_GPIOA_CLK_ENABLE();       //ʹ��GPIOFʱ��
		__HAL_RCC_SPI1_CLK_ENABLE();        //ʹ��SPI5ʱ��

		//PA7,5
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_7;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //����ΪSPI1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}


void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	HAL_SPI_TransmitReceive(&SPI2_Handler,WriteBuffer,ReadBuffer,NumberOfByte, 1000);   
}

//SPI5 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void SPI1_WriteByte(u8 TxData)
{
	HAL_SPI_Transmit(&SPI1_Handler,&TxData,1,1000);
}


/***********************************************************************************************************************
************************************************************************************************************************/






