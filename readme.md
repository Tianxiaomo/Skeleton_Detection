## 髌骨诊断系统.检测设备

### version : 0.1.0
	- 作者：胡广豪
	- 功能：ADXL355 sensor 数据的读取
		包括 x,y,z轴数据和T温度数据
	- 注意事项：
	- Bug: 驱动已完成，但存在诸多问题，主要表现是数据读取不到，IIC收不到ACK回复，怀疑是时序出现问题，或者是读写协议不正确

#### version： 0.1.1		
	- 作者： 胡广豪
	- 修复： 修复IIC发送无ACK问题
	- 描述： 没有正确连线导致，ADXL355 senser SPI协议情况下的SPI_CK在IIC协议中需要接GND

#### version : 0.1.2
	- 作者： 胡广豪
	- 功能： IIC读取ADXL355数据
	- 注意事项：在读操作时，发送完寄存器地址后，需要再次开启IIC_start，才可以读到数据
	- Bug：数据读取首次成功，后续读取不到

#### version ：0.1.3
	- 作者： 胡广豪
	- 修复： 数据读取正常，连续读写问题修复
	- Bug： 数据解算不正确

### version ：0.2.0	
	- 作者： 胡广豪
	- 功能： 数据解算
	- 描述： 由于x,y,z轴加速度数据都是存储在三个联系的寄存器中，每一个数据又20bit表示
		以x轴为例：
		```
		XDATA3	0x08 [7:0(xdata,19:12)]
		XDATA2 	0x09 [7:0(xdata,11:4)]
		XDATA1  0x0A [7:4(xdata,3:0)|3:0(保留，未使用)]
		```
		
		计算方式:
		```		
		x += XDATA3;
		x << 8;
		x += XDATA2;
		x << 8;
		x += XDATA1;
		x >> 4;
		x = x & 0x000FFFFF;
		if((x & 0x00080000) == 0x00080000)
			x = x | 0xFFF00000;
	```			
	- Bug： 由int类型转化为float类型时，正值转换正常，负值转换出错

#### version ：0.2.1
	- 作者： 胡广豪
	- 修复： 数据解算时，负数 int -> float 出错
	- 描述： 原来 SensorX,SensorY,SensorZ 三个变量定义为 unsight int 类型，只能表示正值，导致
		负数转换时误解为正值，数值被增加 0xFFF00000 ,且符号不对。
		将类型变为 int 后，问题解决。

### version : 0.3.0
	- 作者： 胡广豪
	- 功能： 添加文件系统
	- 描述： 为保存采集到数据，增加文件系统，将数据保存到SD卡的文件中，方便上位机处理

### version ： 0.4.0
	- 作者： 胡广豪
	- 功能： 添加匿名上位机协议
	- 描述： 为查看波形，暂使用匿名上位机4.1版本，添加支持的协议

<<<<<<< HEAD
## version ：1.0.0
=======
### version ：1.0.0
- 作者： 胡广豪
- 功能： 增加声音采集传感器，需要添加相应的ADC驱动，OLED驱动
- 描述：AD7192的SPI_miso脚和RDY为一个脚，如果使用软件模拟SPI可以连接一个到一个GPIO即可，如果使用硬件SPI需再连接到一个引脚作为RDY脚

### version：1.0.1 使用freertos
=======
## version：1.0.1 使用freertos
- 作者：胡广豪
- 功能：freertos调度系统
- 描述：增加oled显示屏驱动，ui界面，功能按键等，

```
屏蔽掉了stm32f4xx_if.c中的
void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
```
### version：1.0.2按键时间修改

- 作者：胡广豪

- 功能：按键延时修改，保证体验良好

- 细节

  ```
  由于按键较硬，按下抬起一般要2-4 * 10个节拍，大多数是3 * 10个节拍
  长按一般是 10-15 * 10个节拍，所以LONG_PRESS_TIME 设置位10
  当只检测到一次是，定义为误触
  home键，长按到home页面，右键长按息屏，
  ```
### version：1.1.0 UI界面实现
- 作者：胡广豪
- 功能：UI,OLED驱动优化
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbff6ty7oj303s03qaa3.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfhtj0hzj303r03pwec.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfi2zllcj303r03u745.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfibl04mj303r03pa9z.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfijo0dsj303r03rmx0.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfjb4dl7j303o03pglg.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfjqng3dj303n03s746.jpg)
  

