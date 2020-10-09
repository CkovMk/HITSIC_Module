#ifndef D_MK66F18_DRV_SOFTI2C_PORT_H_
#define D_MK66F18_DRV_SOFTI2C_PORT_H_

gpio_pin_config_t pin_out = {
		kGPIO_DigitalOutput,
		1,
};
gpio_pin_config_t pin_in = {
		kGPIO_DigitalInput,
		1,
};

#ifdef __cplusplus
extern "C" {
#endif

inline void SCL_out(SI2C_Type* base)
{
	base->SCL->PDDR |= (1 << base->SCL_pin);
}

inline void SDA_out(SI2C_Type* base)
{
	base->SDA->PDDR |= (1 << base->SDA_pin);
}

inline void SDA_in(SI2C_Type* base)
{
	base->SDA->PDDR &= ~(1 << base->SDA_pin);
}

inline void SCL_in(SI2C_Type* base)
{
	base->SCL->PDDR &= ~(1 << base->SCL_pin);
}

inline void SDA_H(SI2C_Type* base)
{
	base->SDA->PSOR = 1 << base->SDA_pin;
}

inline void SDA_L(SI2C_Type* base)
{
	base->SDA->PCOR = 1 << base->SDA_pin;
}

inline void SCL_H(SI2C_Type* base)
{
	base->SCL->PSOR = 1 << base->SCL_pin;
}

inline void SCL_L(SI2C_Type* base)
{
	base->SCL->PCOR = 1 << base->SCL_pin;
}

inline uint32_t SDA_val(SI2C_Type* base)
{
	return (((base->SDA->PDIR) >> base->SDA_pin) & 0x01U);
}

inline uint32_t SCL_val(SI2C_Type* base)
{
	return (((base->SCL->PDIR) >> base->SCL_pin) & 0x01U);
}

inline void IIC_Delay(SI2C_Type* p)
{
	uint32_t a = p->nDELAY;
	while (--a) {}
}

inline void IIC_Delay2(SI2C_Type* p)
{
	uint32_t a = p->nDELAY >> 1;
	while (--a) {}
}

#ifdef __cplusplus
}
#endif

#endif // ! D_MK66F18_DRV_SOFTI2C_PORT_H_