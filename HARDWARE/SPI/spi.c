/***********************************************************************************************************************
项目名称文件名
文件名:        rtc.c
描述   :       实时时钟
版本   :       V1.0
修改   :   
完成日期：     2017.4.19
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
SPI_HandleTypeDef SPI1_Handler;  //SPI句柄
SPI_HandleTypeDef SPI2_Handler;  //SPI句柄


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
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   	//设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);						//初始化
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
}

void SPI2_Init(void)
{
	u8 t = 0xff;
    SPI2_Handler.Instance=SPI2;                         //SP2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //使能SPI2
	 HAL_SPI_Transmit(&SPI2_Handler,&t,1,1000);
}

//SPI2底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	if(hspi == &SPI2_Handler){
		__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
		__HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟

		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
		GPIO_Initure.Alternate=GPIO_AF5_SPI2;           //复用为SPI2
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}else if(hspi == &SPI1_Handler){
		__HAL_RCC_GPIOA_CLK_ENABLE();       //使能GPIOF时钟
		__HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI5时钟

		//PA7,5
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_7;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}


void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	HAL_SPI_TransmitReceive(&SPI2_Handler,WriteBuffer,ReadBuffer,NumberOfByte, 1000);   
}

//SPI5 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
void SPI1_WriteByte(u8 TxData)
{
	HAL_SPI_Transmit(&SPI1_Handler,&TxData,1,1000);
}


/***********************************************************************************************************************
************************************************************************************************************************/






