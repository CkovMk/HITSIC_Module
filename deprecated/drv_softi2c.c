#include "rte_softi2c.h"


#ifdef __cplusplus
extern "C" {
#endif

//foward declaration...
//软件IIC所有操作函数
static void IIC_Delay(SI2C_Type* p);				//IIC延时函数
static void IIC_Delay2(SI2C_Type* p);				//IIC延时函数
static void IIC_Start(SI2C_Type* p);				//发送IIC开始信号
static void IIC_Stop(SI2C_Type* p);	  			//发送IIC停止信号
static void IIC_Send_Byte(SI2C_Type* p,uint8_t txd);			//IIC发送一个字节
static uint8_t IIC_Read_Byte(SI2C_Type* p,uint8_t ack);//IIC读取一个字节
static uint8_t IIC_Wait_Ack(SI2C_Type* p); 				//IIC等待ACK信号
static void IIC_Ack(SI2C_Type* p);					//IIC发送ACK信号
static void IIC_NAck(SI2C_Type* p);				//IIC不发送ACK信号

static void SCL_out(SI2C_Type* base);
static void SDA_out(SI2C_Type* base);
static void SDA_in(SI2C_Type* base);
static void SCL_in(SI2C_Type* base);
static void SDA_H(SI2C_Type* base);
static void SDA_L(SI2C_Type* base);
static void SCL_H(SI2C_Type* base);
static void SCL_L(SI2C_Type* base);
static uint32_t SDA_val(SI2C_Type* base);
static uint32_t SCL_val(SI2C_Type* base);


status_t SI2C_Init(SI2C_Type* base)
{
	//����·�ɲ������ÿ�©����
	PORT_SetPinMux(PORTxGet(base->SCL), base->SCL_pin, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTxGet(base->SDA), base->SDA_pin, kPORT_MuxAsGpio);
	PORTxGet(base->SCL)->PCR[base->SCL_pin] = ((PORTxGet(base->SCL)->PCR[base->SCL_pin] &
		/* Mask bits to zero which are setting */
		(~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))
		/* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is
		 * configured as a digital output. */
		| PORT_PCR_ODE(kPORT_OpenDrainEnable)
		/* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
		 * corresponding PE field is set. */
		| PORT_PCR_PS(kPORT_PullUp));
	PORTxGet(base->SDA)->PCR[base->SDA_pin] = ((PORTxGet(base->SDA)->PCR[base->SDA_pin] &
		/* Mask bits to zero which are setting */
		(~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))
		/* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is
		 * configured as a digital output. */
		| PORT_PCR_ODE(kPORT_OpenDrainEnable)
		/* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
		 * corresponding PE field is set. */
		| PORT_PCR_PS(kPORT_PullUp));
	SCL_H(base);
	SDA_H(base);
	SCL_out(base);
	SDA_out(base);
	return 0;
}

status_t SI2C_MasterReadBlocking(SI2C_Type* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 0);//����������ַ+д����	
	if (IIC_Wait_Ack(p))	//�ȴ�Ӧ��
	{
		IIC_Stop(p);
		return 1;
	}
	IIC_Send_Byte(p, reg);	//д�Ĵ�����ַ
	IIC_Wait_Ack(p);		//�ȴ�Ӧ��
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 1);//����������ַ+������	
	IIC_Wait_Ack(p);		//�ȴ�Ӧ�� 
	while (size)
	{
		if (size == 1)* data = IIC_Read_Byte(p, 0);//������,����nACK 
		else *data = IIC_Read_Byte(p, 1);		//������,����ACK  
		size--;
		data++;
	}
	IIC_Stop(p);	//����һ��ֹͣ���� 
	return 0;
}

status_t SI2C_MasterWriteBlocking(SI2C_Type* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	uint8_t i;
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 0);//����������ַ+д����	
	if (IIC_Wait_Ack(p))	//�ȴ�Ӧ��
	{
		IIC_Stop(p);
		return 1;
	}
	IIC_Send_Byte(p, reg);	//д�Ĵ�����ַ
	IIC_Wait_Ack(p);		//�ȴ�Ӧ��
	for (i = 0; i < size; i++)
	{
		IIC_Send_Byte(p, data[i]);	//��������
		if (IIC_Wait_Ack(p))		//�ȴ�ACK
		{
			IIC_Stop(p);
			return 1;
		}
	}
	IIC_Stop(p);
	return 0;
}







inline void IIC_Start(SI2C_Type* p)
{
	SDA_H(p);
	SCL_H(p);
	SDA_out(p);     //sda�����
	SCL_out(p);

	IIC_Delay(p);
	SDA_L(p);//START:when CLK is high,DATA change form high to low 
	IIC_Delay(p);
	SCL_L(p); //ǯסI2C���ߣ�׼�����ͻ��������
}

inline void IIC_Stop(SI2C_Type* p)
{
	SCL_L(p);
	SDA_L(p);//STOP:when CLK is high DATA change form low to high
	SDA_out(p);     //sda�����
	IIC_Delay(p);
	SCL_H(p);
	SDA_H(p);//����I2C���߽����ź�
	IIC_Delay(p);
}

inline void IIC_Send_Byte(SI2C_Type* p, uint8_t txd)
{
	uint8_t t;
	SDA_out(p);
	SCL_L(p);// ����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7) {
			SDA_H(p);
		}
		else {
			SDA_L(p);
		}
		txd <<= 1;
		SCL_H(p);
		IIC_Delay(p);
		SCL_L(p);
		IIC_Delay(p);
	}
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
inline uint8_t IIC_Read_Byte(SI2C_Type* p, uint8_t ack)
{
	uint8_t i, receive = 0;
	SDA_in(p);//SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		SCL_L(p);
		IIC_Delay(p);
		SCL_H(p);
		receive <<= 1;
		if (SDA_val(p))receive++;
		IIC_Delay(p);
	}
	if (!ack)
		IIC_NAck(p);//����nACK
	else
		IIC_Ack(p); //����ACK   
	return receive;
}


inline uint8_t IIC_Wait_Ack(SI2C_Type* p)
{
	uint8_t ucErrTime = 0;
	SDA_in(p);      //SDA����Ϊ����  
	SDA_H(p); IIC_Delay(p);
	SCL_H(p); IIC_Delay(p);
	while (SDA_val(p))
	{
		IIC_Delay(p);
		ucErrTime++;
		if (ucErrTime > 10)
		{
			IIC_Stop(p);
			return 1;
		}
	}
	SCL_L(p);//ʱ�����0 	   
	return 0;
}

inline void IIC_Ack(SI2C_Type* p)
{
	SCL_L(p);
	SDA_out(p);
	SDA_L(p);
	IIC_Delay(p);
	SCL_H(p);
	IIC_Delay(p);
	SCL_L(p);
}

inline void IIC_NAck(SI2C_Type* p)
{
	SCL_L(p);
	SDA_out(p);
	SDA_H(p);
	IIC_Delay(p);
	SCL_H(p);
	IIC_Delay(p);
	SCL_L(p);
}



#ifdef __cplusplus
}
#endif
