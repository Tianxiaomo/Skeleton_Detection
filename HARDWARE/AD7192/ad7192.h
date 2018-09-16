/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD7192.h

 Description :	 Use the GPIO to simulate the SPI communication of AD7192

 Hardware plateform : 	ADuC7026 and AD7190/92EBZ
********************************************************************************/

#ifndef AD7192_H
#define AD7192_H

// add the header file here

#include "sys.h"

extern unsigned long int AD7192Registers[8];
extern unsigned long int AD7192Data;

//µ˜ ‘”√
#define DEBUG 1
#define SIMULATION_SPI 0
//Register Map and bit definition

//Communications Register
#define	WEN				0x00
#define WEN_DIS			0x80
#define RW_W			0x00
#define	RW_R			0x40
#define REG_COM_STA		0x00
#define	REG_MODE		0x01
#define	REG_CONF		0x02
#define	REG_DATA		0x03
#define	REG_ID			0x04
#define	REG_GPOCON		0x05
#define	REG_OFFSET		0x06
#define	REG_FS			0x07
#define	CREAD_EN		0x04
#define	CREAD_DIS		0x00

//Status Register
#define	RDY_H			0x80
#define	RDY_L			0x00
#define	ERR_H			0x40
#define	ERR_L			0x00
#define	NOREF_H			0x20
#define	NOREF_L			0x00
#define	PARITY_H		0x10
#define	PARITY_L		0x00
#define	CHDST_AIN1_AIN2	0x00
#define	CHDST_AIN3_AIN4	0x01
#define	CHDST_TEMP		0x02
#define	CHDST_AIN2_AIN2	0x03
#define	CHDST_AIN1_COM	0x04
#define	CHDST_AIN2_COM	0x05
#define	CHDST_AIN3_COM	0x06
#define	CHDST_AIN4_COM	0x07

//Mode Register
#define	MODE_CONT				0x000000
#define	MODE_SING				0x200000
#define	MODE_IDLE				0x400000
#define	MODE_PD					0x600000
#define	MODE_INZCL				0x800000
#define	MODE_INFCL				0xA00000
#define	MODE_SYSZCL				0xC00000
#define	MODE_SYSFCL				0xE00000
#define	DAT_STA_EN				0x100000
#define	DAT_STA_DIS				0x000000	  
#define	EXT_XTAL				0x000000
#define	EXT_CLK					0x040000
#define	INCLK_MCLK2TRI			0x080000
#define	INCLK_MCLK2EN			0x0C0000
#define	SINC_4					0x000000
#define	SINC_3					0x008000
#define	ENPAR_EN				0x002000
#define	ENPAR_DIS				0x000000
#define	CLK_DIV_2				0x001000
#define	CLK_DIV_DIS				0x000000
#define	SINGLECYCLE_EN			0x000800
#define	SINGLECYCLE_DIS			0x000000
#define	REJ60_EN				0x000400
#define	REJ60_DIS				0x000000
										
//Configuration Register			
#define CHOP_EN					0x800000
#define	CHOP_DIS				0x000000
#define	REF_IN1					0x000000
#define	REF_IN2					0x100000
#define	AIN1_AIN2				0x000100
#define	AIN3_AIN4				0x000200
#define	TEMP					0x000400
#define	AIN2_AIN2				0x000800
#define	AIN1_COM				0x001000
#define	AIN2_COM				0x002000
#define	AIN3_COM				0x004000
#define	AIN4_COM				0x008000
#define	BURN_EN					0x000080
#define	BURN_DIS				0x000000
#define	REFDET_EN				0x000040
#define	REFDET_DIS				0x000000
#define	BUF_EN					0x000010
#define	BUF_DIS					0x000000
#define	UB_UNI					0x000008
#define UB_BI					0x000000
#define	GAIN_1					0x000000
#define	GAIN_8					0x000003
#define	GAIN_16					0x000004
#define	GAIN_32					0x000005
#define	GAIN_64					0x000006
#define	GAIN_128				0x000007

//GPOCON Register
#define BPDSW_CLOSE				0x40
#define	BPDSW_OPEN				0x00
#define	GP32EN					0x20
#define	GP32DIS					0x00
#define	GP10EN					0x10
#define	GP10DIS					0x00
#define	P3DAT_H					0x08
#define	P3DAT_L					0x00
#define	P2DAT_H					0x04
#define	P2DAT_L					0x00
#define	P1DAT_H					0x02
#define	P1DAT_L					0x00
#define	P0DAT_H					0x01
#define	P0DAT_L					0x00

//No Operation
#define	NOP						0x00									
 
#define SPI_IN 			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) //PD3
#define SPI_CS_LOW  	GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SPI_CS_HIGH  	GPIO_SetBits(GPIOB,GPIO_Pin_6)

unsigned char WriteToAD7192ViaSPI(const unsigned char RegisterStartAddress, const unsigned char NumberOfRegistersToWrite, unsigned long int *DataBuffer, const unsigned char OffsetInBuffer);
unsigned char ReadFromAD7192ViaSPI(const unsigned char RegisterStartAddress, const unsigned char NumberOfRegistersToRead, unsigned long int *DataBuffer, const unsigned char OffsetInBuffer);
void AD7192Initialization(void);
void AD7192SoftwareReset(void);
void AD7192InternalZeroScaleCalibration(void);
void AD7192InternalFullScaleCalibration(void);
void AD7192ExternalZeroScaleCalibration(void);
void AD7192ExternalFullScaleCalibration(void);
void AD7192StartContinuousRead(void);	 
unsigned long int AD7192ContinuousRead(void);
void AD7192ExitContinuousRead(void);
void AD7192StartSingleConvertion(unsigned long int Channels);
void AD7192StartContinuousConvertion(unsigned long int Channels);
unsigned long int AD7192ReadConvertingData(void);
float AD7192ReadTemperature(void);
float RealTemperature(unsigned long int TemperatureCode);
u32 GetSingleConvertionValue(u32 Channels);
#endif

