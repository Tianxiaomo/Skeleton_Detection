#include "ADXL355.h" 
#include "delay.h" 				 
#include "usart.h"
static u8 DeviceAddr_1,DeviceAddr_2;
/************************* Local scope functions *****************************/

/**
   @brief ��ADXL355_1ָ����ַ����һ������
   @param ReadAddr  :��ȡ���ݵ�Ŀ�ĵ�ַ  
   @return ����������
**/
u8 ADXL355_1_ReadOneByte(u8 ReadAddr){				  
	u8 temp=0;
    IIC1_Start();
	IIC1_Send_Byte( (DeviceAddr_1<<1) | 0x00);   			//���͵�ַ
	IIC1_Wait_Ack();
	
	IIC1_Send_Byte(ReadAddr);
	IIC1_Wait_Ack();
	
	IIC1_Start();
	IIC1_Send_Byte( (DeviceAddr_1<<1) | 0x01);   			//���͵�ַ
	IIC1_Wait_Ack();
	temp=IIC1_Read_Byte(0);
    IIC1_Stop();//����һ��ֹͣ����	    
	return temp;
}

/**
   @brief ��ADXL355_1ָ����ַд��һ������
   @param WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ  
   @param DataToWrite:Ҫд�������
   @return none
**/
static void ADXL355_1_WriteOneByte(u8 WriteAddr,u8 DataToWrite){
    IIC1_Start();  
	IIC1_Send_Byte( (DeviceAddr_1<<1) | 0x00);		//дģʽ
	printf("w_ack = %d\r\n",IIC1_Wait_Ack());
    IIC1_Send_Byte(WriteAddr);   		//���͵�ַ
	printf("w_ack = %d\r\n",IIC1_Wait_Ack());
	IIC1_Send_Byte(DataToWrite);     //�����ֽ�							   
	printf("w_ack = %d\r\n",IIC1_Wait_Ack());
    IIC1_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

/**
   @brief ��ADXL355_1�����ָ����ַ��ʼд�볤��ΪLen������,�ú�������д��16bit����32bit������.
   @param WriteAddr  :��ʼд��ĵ�ַ 
   @param DataToWrite:���������׵�ַ
   @param Len        :Ҫд�����ݵĳ���2,4
   @return none
**/
static void ADXL355_1_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len){
	u8 t;
	for(t=0;t<Len;t++)
	{
		ADXL355_1_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}	
}


/**
   @brief ��ADXL355_1�����ָ����ַ��ʼ��������ΪLen������
   @param ReadAddr  :��ʼ�����ĵ�ַ 
   @param Len       :Ҫ�������ݵĳ���2,3,4
   @return ����ֵ   :����
**/
static u32 ADXL355_1_ReadLenByte(u8 ReadAddr,u8 Len){  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=ADXL355_1_ReadOneByte(ReadAddr+t); 
	}
	return temp;												    
}

/**
   @brief Convert the two's complement data in X,Y,Z registers to signed integers
   @param ui32SensorData - raw data from register
   @return int32_t - signed integer data
**/
static u32 ADXL355_1_Acceleration_Data_Conversion (uint32_t ui32SensorData){
   u32 volatile i32Conversion = 0;
   ui32SensorData = (ui32SensorData  >> 4);
   ui32SensorData = (ui32SensorData & 0x000FFFFF);
	
   if((ui32SensorData & 0x00080000)  == 0x00080000){
         i32Conversion = (ui32SensorData | 0xFFF00000);
   }
   else{
         i32Conversion = ui32SensorData;
   }
   return i32Conversion;
}


/************************* Global scope functions *****************************/
/**
	@brief ADXL355_1 init
	@para  Deviceaddr	device addr
	@reture none
**/
void ADXL355_1_Init(u8 Deviceaddr){
	IIC1_Init();//IIC��ʼ��
	DeviceAddr_1 = Deviceaddr;
//   /* Quick verification test for boards */
//   uint32_t volatile ui32test = SPI_Read(DEVID_AD, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test2 = SPI_Read(DEVID_MST, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test3 = SPI_Read(PARTID, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test4 = SPI_Read(REVID, SPI_READ_ONE_REG);                 /* Read the ID register */
}

/**
   @brief Turns on accelerometer measurement mode.
   @return none
**/
void ADXL355_1_Start_Sensor(void){
	uint8_t ui8temp = 0;
	ui8temp = ADXL355_1_ReadOneByte(POWER_CTL);       /* Read POWER_CTL register, before modifying it */
	printf("ui8temp  = 0x%2x\r\n", ui8temp);                 /* Print the ID register */
	ui8temp = ui8temp & 0xFE;                       /* Set measurement bit in POWER_CTL register */
	ADXL355_1_WriteOneByte(POWER_CTL, ui8temp);       /* Write the new value to POWER_CTL register */
}


/**
   @brief Puts the accelerometer into standby mode.
   @return none
**/
void ADXL355_1_Stop_Sensor(void){
   uint8_t ui8temp;
   ui8temp = (uint8_t)ADXL355_1_ReadOneByte(POWER_CTL);        /*Read POWER_CTL register, before modifying it */
   ui8temp = ui8temp | 0x01;                                 /* Clear measurement bit in POWER_CTL register */
   ADXL355_1_WriteOneByte(POWER_CTL, ui8temp);                 /* Write the new value to POWER_CTL register */
}

/**
   @brief Set the range of measurement
   @param Range :2 , 4 , 8
   @return  Precision
**/
float ADXL_RANGE_SET_1(u8 Range){
	float adxl355Scale;
	switch (Range){
		case 2:
			ADXL355_1_WriteOneByte(RANGE, 0x81);          /* Set sensor range within RANGE register */
			adxl355Scale = 256000.0f;
			break;
		case 4:
			ADXL355_1_WriteOneByte(RANGE, 0x82);          /* Set sensor range within RANGE register */
			adxl355Scale = 128000.0f;
			break;
		case 8:
			ADXL355_1_WriteOneByte(RANGE, 0x83);          /* Set sensor range within RANGE register */
			adxl355Scale = 64000.0f;
			break;
		default :
			ADXL355_1_WriteOneByte(RANGE, 0x81);          /* Set sensor range within RANGE register */
			adxl355Scale = 256000.0f;
			break;
	}
	return adxl355Scale;
}

/**
   @brief Get Device ID
   @return  ID
**/
u8 ADXL355_1_DEVICE_ID(void){
	return ADXL355_1_ReadOneByte(DEVID_AD);                  /* Read the ID register */
}

/**
   @brief Get Manufacturer Info
   @return  Manufacturer Info
**/
u8 ADXL355_1_MI(void){
	return ADXL355_1_ReadOneByte(DEVID_MST);               	/* Read the Manufacturer register */
}

/**
   @brief Get Device ID
   @return  ID
**/
u8 ADXL355_1_PART_ID(void){
	return ADXL355_1_ReadOneByte(PARTID);                  	/* Read the Part ID register */
}

/**
   @brief Reads the accelerometer data.
   @para SensorX : X acclelerate data pointer
   @para SensorY : Y acclelerate data pointer
   @para SensorZ : Z acclelerate data pointer
   @para SensorZ : Temperature data pointer
   @return none
**/
void ADXL355_1_Data_Scan(int_least32_t* SensorX,int_least32_t* SensorY,int_least32_t* SensorZ,u32* SensorT){
	volatile uint32_t ui32SensorX;
	volatile uint32_t ui32SensorY;
	volatile uint32_t ui32SensorZ;
	volatile uint32_t ui32SensorT;
	ui32SensorX = ADXL355_1_ReadLenByte(XDATA3,SPI_READ_THREE_REG);
	ui32SensorY = ADXL355_1_ReadLenByte(YDATA3,SPI_READ_THREE_REG);
	ui32SensorZ = ADXL355_1_ReadLenByte(ZDATA3, SPI_READ_THREE_REG);
	ui32SensorT = ADXL355_1_ReadLenByte(TEMP2, SPI_READ_TWO_REG);
	*SensorT = ui32SensorT & 0x00000FFF;
	*SensorX = ADXL355_1_Acceleration_Data_Conversion(ui32SensorX);
	*SensorY = ADXL355_1_Acceleration_Data_Conversion(ui32SensorY);
	*SensorZ = ADXL355_1_Acceleration_Data_Conversion(ui32SensorZ);
}











/************************* Local scope functions *****************************/

/**
   @brief ��ADXL355_2ָ����ַ����һ������
   @param ReadAddr  :��ȡ���ݵ�Ŀ�ĵ�ַ  
   @return ����������
**/
u8 ADXL355_2_ReadOneByte(u8 ReadAddr){				  
	u8 temp=0;
    IIC2_Start();
	IIC2_Send_Byte( (DeviceAddr_2<<1) | 0x00);   			//���͵�ַ
	IIC2_Wait_Ack();
	
	IIC2_Send_Byte(ReadAddr);
	IIC2_Wait_Ack();
	
	IIC2_Start();
	IIC2_Send_Byte( (DeviceAddr_2<<1) | 0x01);   			//���͵�ַ
	IIC2_Wait_Ack();
	temp=IIC2_Read_Byte(0);
    IIC2_Stop();//����һ��ֹͣ����	    
	return temp;
}

/**
   @brief ��ADXL355_2ָ����ַд��һ������
   @param WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ  
   @param DataToWrite:Ҫд�������
   @return none
**/
static void ADXL355_2_WriteOneByte(u8 WriteAddr,u8 DataToWrite){
    IIC2_Start();  
	IIC2_Send_Byte( (DeviceAddr_2<<1) | 0x00);		//дģʽ
	printf("w_ack = %d\r\n",IIC2_Wait_Ack());
    IIC2_Send_Byte(WriteAddr);   		//���͵�ַ
	printf("w_ack = %d\r\n",IIC2_Wait_Ack());
	IIC2_Send_Byte(DataToWrite);     //�����ֽ�							   
	printf("w_ack = %d\r\n",IIC2_Wait_Ack());
    IIC2_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

/**
   @brief ��ADXL355_2�����ָ����ַ��ʼд�볤��ΪLen������,�ú�������д��16bit����32bit������.
   @param WriteAddr  :��ʼд��ĵ�ַ 
   @param DataToWrite:���������׵�ַ
   @param Len        :Ҫд�����ݵĳ���2,4
   @return none
**/
static void ADXL355_2_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len){
	u8 t;
	for(t=0;t<Len;t++)
	{
		ADXL355_2_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}	
}


/**
   @brief ��ADXL355_2�����ָ����ַ��ʼ��������ΪLen������
   @param ReadAddr  :��ʼ�����ĵ�ַ 
   @param Len       :Ҫ�������ݵĳ���2,3,4
   @return ����ֵ   :����
**/
static u32 ADXL355_2_ReadLenByte(u8 ReadAddr,u8 Len){  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=ADXL355_2_ReadOneByte(ReadAddr+t); 
	}
	return temp;												    
}

/**
   @brief Convert the two's complement data in X,Y,Z registers to signed integers
   @param ui32SensorData - raw data from register
   @return int32_t - signed integer data
**/
static u32 ADXL355_2_Acceleration_Data_Conversion (uint32_t ui32SensorData){
   u32 volatile i32Conversion = 0;
   ui32SensorData = (ui32SensorData  >> 4);
   ui32SensorData = (ui32SensorData & 0x000FFFFF);
	
   if((ui32SensorData & 0x00080000)  == 0x00080000){
         i32Conversion = (ui32SensorData | 0xFFF00000);
   }
   else{
         i32Conversion = ui32SensorData;
   }
   return i32Conversion;
}


/************************* Global scope functions *****************************/
/**
	@brief ADXL355_2 init
	@para  Deviceaddr	device addr
	@reture none
**/
void ADXL355_2_Init(u8 Deviceaddr){
	IIC2_Init();//IIC��ʼ��
	DeviceAddr_2 = Deviceaddr;
//   /* Quick verification test for boards */
//   uint32_t volatile ui32test = SPI_Read(DEVID_AD, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test2 = SPI_Read(DEVID_MST, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test3 = SPI_Read(PARTID, SPI_READ_ONE_REG);                  /* Read the ID register */
//   uint32_t volatile ui32test4 = SPI_Read(REVID, SPI_READ_ONE_REG);                 /* Read the ID register */
}

/**
   @brief Turns on accelerometer measurement mode.
   @return none
**/
void ADXL355_2_Start_Sensor(void){
	uint8_t ui8temp = 0;
	ui8temp = ADXL355_2_ReadOneByte(POWER_CTL);       /* Read POWER_CTL register, before modifying it */
	printf("ui8temp  = 0x%2x\r\n", ui8temp);                 /* Print the ID register */
	ui8temp = ui8temp & 0xFE;                       /* Set measurement bit in POWER_CTL register */
	ADXL355_2_WriteOneByte(POWER_CTL, ui8temp);       /* Write the new value to POWER_CTL register */
}


/**
   @brief Puts the accelerometer into standby mode.
   @return none
**/
void ADXL355_2_Stop_Sensor(void){
   uint8_t ui8temp;
   ui8temp = (uint8_t)ADXL355_2_ReadOneByte(POWER_CTL);        /*Read POWER_CTL register, before modifying it */
   ui8temp = ui8temp | 0x01;                                 /* Clear measurement bit in POWER_CTL register */
   ADXL355_2_WriteOneByte(POWER_CTL, ui8temp);                 /* Write the new value to POWER_CTL register */
}

/**
   @brief Set the range of measurement
   @param Range :2 , 4 , 8
   @return  Precision
**/
float ADXL_RANGE_SET_2(u8 Range){
	float adxl355Scale;
	switch (Range){
		case 2:
			ADXL355_2_WriteOneByte(RANGE, 0x81);          /* Set sensor range within RANGE register */
			adxl355Scale = 256000.0f;
			break;
		case 4:
			ADXL355_2_WriteOneByte(RANGE, 0x82);          /* Set sensor range within RANGE register */
			adxl355Scale = 128000.0f;
			break;
		case 8:
			ADXL355_2_WriteOneByte(RANGE, 0x83);          /* Set sensor range within RANGE register */
			adxl355Scale = 64000.0f;
			break;
		default :
			ADXL355_2_WriteOneByte(RANGE, 0x81);          /* Set sensor range within RANGE register */
			adxl355Scale = 256000.0f;
			break;
	}
	return adxl355Scale;
}

/**
   @brief Get Device ID
   @return  ID
**/
u8 ADXL355_2_DEVICE_ID(void){
	return ADXL355_2_ReadOneByte(DEVID_AD);                  /* Read the ID register */
}

/**
   @brief Get Manufacturer Info
   @return  Manufacturer Info
**/
u8 ADXL355_2_MI(void){
	return ADXL355_2_ReadOneByte(DEVID_MST);               	/* Read the Manufacturer register */
}

/**
   @brief Get Device ID
   @return  ID
**/
u8 ADXL355_2_PART_ID(void){
	return ADXL355_2_ReadOneByte(PARTID);                  	/* Read the Part ID register */
}

/**
   @brief Reads the accelerometer data.
   @para SensorX : X acclelerate data pointer
   @para SensorY : Y acclelerate data pointer
   @para SensorZ : Z acclelerate data pointer
   @para SensorZ : Temperature data pointer
   @return none
**/
void ADXL355_2_Data_Scan(int_least32_t* SensorX,int_least32_t* SensorY,int_least32_t* SensorZ,u32* SensorT){
	volatile uint32_t ui32SensorX;
	volatile uint32_t ui32SensorY;
	volatile uint32_t ui32SensorZ;
	volatile uint32_t ui32SensorT;
	ui32SensorX = ADXL355_2_ReadLenByte(XDATA3,SPI_READ_THREE_REG);
	ui32SensorY = ADXL355_2_ReadLenByte(YDATA3,SPI_READ_THREE_REG);
	ui32SensorZ = ADXL355_2_ReadLenByte(ZDATA3, SPI_READ_THREE_REG);
	ui32SensorT = ADXL355_2_ReadLenByte(TEMP2, SPI_READ_TWO_REG);
	*SensorT = ui32SensorT & 0x00000FFF;
	*SensorX = ADXL355_2_Acceleration_Data_Conversion(ui32SensorX);
	*SensorY = ADXL355_2_Acceleration_Data_Conversion(ui32SensorY);
	*SensorZ = ADXL355_2_Acceleration_Data_Conversion(ui32SensorZ);
}




