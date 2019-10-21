#include "rte_softi2c.h"


#ifdef __cplusplus
extern "C" {
#endif

//foward declaration...
//软件IIC所有操作函数
inline void IIC_Delay(si2c_master_t* p);				//IIC延时函数
inline void IIC_Delay2(si2c_master_t* p);				//IIC延时函数
void IIC_Start(si2c_master_t* p);				//发送IIC开始信号
inline void IIC_Stop(si2c_master_t* p);	  			//发送IIC停止信号
inline void IIC_Send_Byte(si2c_master_t* p, uint8_t txd);			//IIC发送一个字节
inline uint8_t IIC_Read_Byte(si2c_master_t* p, uint8_t ack);//IIC读取一个字节
inline uint8_t IIC_Wait_Ack(si2c_master_t* p); 				//IIC等待ACK信号
inline void IIC_Ack(si2c_master_t* p);					//IIC发送ACK信号
inline void IIC_NAck(si2c_master_t* p);				//IIC不发送ACK信号
inline void SCL_out(si2c_master_t* base);
inline void SDA_out(si2c_master_t* base);
inline void SDA_in(si2c_master_t* base);
inline void SCL_in(si2c_master_t* base);
inline void SDA_H(si2c_master_t* base);
inline void SDA_L(si2c_master_t* base);
inline void SCL_H(si2c_master_t* base);
inline void SCL_L(si2c_master_t* base);
inline uint32_t SDA_val(si2c_master_t* base);
inline uint32_t SCL_val(si2c_master_t* base);


status_t SI2C_Init(si2c_master_t* base)
{
	//引脚路由并且配置开漏上拉
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

status_t SI2C_MasterReadBlocking(si2c_master_t* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 0);//发送器件地址+写命令	
	if (IIC_Wait_Ack(p))	//等待应答
	{
		IIC_Stop(p);
		return 1;
	}
	IIC_Send_Byte(p, reg);	//写寄存器地址
	IIC_Wait_Ack(p);		//等待应答
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 1);//发送器件地址+读命令	
	IIC_Wait_Ack(p);		//等待应答 
	while (size)
	{
		if (size == 1)* data = IIC_Read_Byte(p, 0);//读数据,发送nACK 
		else *data = IIC_Read_Byte(p, 1);		//读数据,发送ACK  
		size--;
		data++;
	}
	IIC_Stop(p);	//产生一个停止条件 
	return 0;
}

status_t SI2C_MasterWriteBlocking(si2c_master_t* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	uint8_t i;
	IIC_Start(p);
	IIC_Send_Byte(p, (SlaveAddress << 1) | 0);//发送器件地址+写命令	
	if (IIC_Wait_Ack(p))	//等待应答
	{
		IIC_Stop(p);
		return 1;
	}
	IIC_Send_Byte(p, reg);	//写寄存器地址
	IIC_Wait_Ack(p);		//等待应答
	for (i = 0; i < size; i++)
	{
		IIC_Send_Byte(p, data[i]);	//发送数据
		if (IIC_Wait_Ack(p))		//等待ACK
		{
			IIC_Stop(p);
			return 1;
		}
	}
	IIC_Stop(p);
	return 0;
}


PORT_Type* PORTxGet(GPIO_Type* GPIOx)
{
	if (GPIOx == GPIOA)
	{
		return PORTA;
	}
	else if (GPIOx == GPIOB)
	{
		return PORTB;
	}
	else if (GPIOx == GPIOC)
	{
		return PORTC;
	}
	else if (GPIOx == GPIOD)
	{
		return PORTD;
	}
	else if (GPIOx == GPIOE)
	{
		return PORTE;
	}
	else
	{
		return NULL;
	}
}





gpio_pin_config_t pin_out = {
		kGPIO_DigitalOutput,
		1,
};
gpio_pin_config_t pin_in = {
		kGPIO_DigitalInput,
		1,
};
inline void SCL_out(si2c_master_t* base)
{
	base->SCL->PDDR |= (1 << base->SCL_pin);
}

inline void SDA_out(si2c_master_t* base)
{
	base->SDA->PDDR |= (1 << base->SDA_pin);
}

inline void SDA_in(si2c_master_t* base)
{
	base->SDA->PDDR &= ~(1 << base->SDA_pin);
}

inline void SCL_in(si2c_master_t* base)
{
	base->SCL->PDDR &= ~(1 << base->SCL_pin);
}

inline void SDA_H(si2c_master_t* base)
{
	base->SDA->PSOR = 1 << base->SDA_pin;
}

inline void SDA_L(si2c_master_t* base)
{
	base->SDA->PCOR = 1 << base->SDA_pin;
}

inline void SCL_H(si2c_master_t* base)
{
	base->SCL->PSOR = 1 << base->SCL_pin;
}

inline void SCL_L(si2c_master_t* base)
{
	base->SCL->PCOR = 1 << base->SCL_pin;
}

inline uint32_t SDA_val(si2c_master_t* base)
{
	return (((base->SDA->PDIR) >> base->SDA_pin) & 0x01U);
}

inline uint32_t SCL_val(si2c_master_t* base)
{
	return (((base->SCL->PDIR) >> base->SCL_pin) & 0x01U);
}

inline void IIC_Delay(si2c_master_t* p)
{
	uint32_t a = p->nDELAY;
	while (--a) {}
}

inline void IIC_Delay2(si2c_master_t* p)
{
	uint32_t a = p->nDELAY >> 1;
	while (--a) {}
}

inline void IIC_Start(si2c_master_t* p)
{
	SDA_H(p);
	SCL_H(p);
	SDA_out(p);     //sda线输出
	SCL_out(p);

	IIC_Delay(p);
	SDA_L(p);//START:when CLK is high,DATA change form high to low 
	IIC_Delay(p);
	SCL_L(p); //钳住I2C总线，准备发送或接收数据
}

inline void IIC_Stop(si2c_master_t* p)
{
	SCL_L(p);
	SDA_L(p);//STOP:when CLK is high DATA change form low to high
	SDA_out(p);     //sda线输出
	IIC_Delay(p);
	SCL_H(p);
	SDA_H(p);//发送I2C总线结束信号
	IIC_Delay(p);
}

inline void IIC_Send_Byte(si2c_master_t* p, uint8_t txd)
{
	uint8_t t;
	SDA_out(p);
	SCL_L(p);// 拉低时钟开始数据传输
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

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
inline uint8_t IIC_Read_Byte(si2c_master_t* p, uint8_t ack)
{
	uint8_t i, receive = 0;
	SDA_in(p);//SDA设置为输入
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
		IIC_NAck(p);//发送nACK
	else
		IIC_Ack(p); //发送ACK   
	return receive;
}


inline uint8_t IIC_Wait_Ack(si2c_master_t* p)
{
	uint8_t ucErrTime = 0;
	SDA_in(p);      //SDA设置为输入  
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
	SCL_L(p);//时钟输出0 	   
	return 0;
}

inline void IIC_Ack(si2c_master_t* p)
{
	SCL_L(p);
	SDA_out(p);
	SDA_L(p);
	IIC_Delay(p);
	SCL_H(p);
	IIC_Delay(p);
	SCL_L(p);
}

inline void IIC_NAck(si2c_master_t* p)
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
