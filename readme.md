## �ƹ����ϵͳ.����豸

### version : 0.1.0
	- ���ߣ������
	- ���ܣ�ADXL355 sensor ���ݵĶ�ȡ
		���� x,y,z�����ݺ�T�¶�����
	- ע�����
	- Bug: ��������ɣ�������������⣬��Ҫ���������ݶ�ȡ������IIC�ղ���ACK�ظ���������ʱ��������⣬�����Ƕ�дЭ�鲻��ȷ

#### version�� 0.1.1		
	- ���ߣ� �����
	- �޸��� �޸�IIC������ACK����
	- ������ û����ȷ���ߵ��£�ADXL355 senser SPIЭ������µ�SPI_CK��IICЭ������Ҫ��GND

#### version : 0.1.2
	- ���ߣ� �����
	- ���ܣ� IIC��ȡADXL355����
	- ע������ڶ�����ʱ��������Ĵ�����ַ����Ҫ�ٴο���IIC_start���ſ��Զ�������
	- Bug�����ݶ�ȡ�״γɹ���������ȡ����

#### version ��0.1.3
	- ���ߣ� �����
	- �޸��� ���ݶ�ȡ������������д�����޸�
	- Bug�� ���ݽ��㲻��ȷ

### version ��0.2.0	
	- ���ߣ� �����
	- ���ܣ� ���ݽ���
	- ������ ����x,y,z����ٶ����ݶ��Ǵ洢��������ϵ�ļĴ����У�ÿһ��������20bit��ʾ
		��x��Ϊ����
		```
		XDATA3	0x08 [7:0(xdata,19:12)]
		XDATA2 	0x09 [7:0(xdata,11:4)]
		XDATA1  0x0A [7:4(xdata,3:0)|3:0(������δʹ��)]
		```
		
		���㷽ʽ:
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
	- Bug�� ��int����ת��Ϊfloat����ʱ����ֵת����������ֵת������

#### version ��0.2.1
	- ���ߣ� �����
	- �޸��� ���ݽ���ʱ������ int -> float ����
	- ������ ԭ�� SensorX,SensorY,SensorZ ������������Ϊ unsight int ���ͣ�ֻ�ܱ�ʾ��ֵ������
		����ת��ʱ���Ϊ��ֵ����ֵ������ 0xFFF00000 ,�ҷ��Ų��ԡ�
		�����ͱ�Ϊ int ����������

### version : 0.3.0
	- ���ߣ� �����
	- ���ܣ� ����ļ�ϵͳ
	- ������ Ϊ����ɼ������ݣ������ļ�ϵͳ�������ݱ��浽SD�����ļ��У�������λ������

### version �� 0.4.0
	- ���ߣ� �����
	- ���ܣ� ���������λ��Э��
	- ������ Ϊ�鿴���Σ���ʹ��������λ��4.1�汾�����֧�ֵ�Э��

<<<<<<< HEAD
## version ��1.0.0
=======
### version ��1.0.0
- ���ߣ� �����
- ���ܣ� ���������ɼ�����������Ҫ�����Ӧ��ADC������OLED����
- ������AD7192��SPI_miso�ź�RDYΪһ���ţ����ʹ�����ģ��SPI��������һ����һ��GPIO���ɣ����ʹ��Ӳ��SPI�������ӵ�һ��������ΪRDY��

### version��1.0.1 ʹ��freertos
=======
## version��1.0.1 ʹ��freertos
- ���ߣ������
- ���ܣ�freertos����ϵͳ
- ����������oled��ʾ��������ui���棬���ܰ����ȣ�

```
���ε���stm32f4xx_if.c�е�
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
### version��1.0.2����ʱ���޸�

- ���ߣ������

- ���ܣ�������ʱ�޸ģ���֤��������

- ϸ��

  ```
  ���ڰ�����Ӳ������̧��һ��Ҫ2-4 * 10�����ģ��������3 * 10������
  ����һ���� 10-15 * 10�����ģ�����LONG_PRESS_TIME ����λ10
  ��ֻ��⵽һ���ǣ�����Ϊ��
  home����������homeҳ�棬�Ҽ�����Ϣ����
  ```
### version��1.1.0 UI����ʵ��
- ���ߣ������
- ���ܣ�UI,OLED�����Ż�
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbff6ty7oj303s03qaa3.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfhtj0hzj303r03pwec.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfi2zllcj303r03u745.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfibl04mj303r03pa9z.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfijo0dsj303r03rmx0.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfjb4dl7j303o03pglg.jpg)
![](https://ws1.sinaimg.cn/large/005PF9Rqgy1fvbfjqng3dj303n03s746.jpg)
  

