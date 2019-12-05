#pragma once
#ifndef UTILITIES_SYS_EXTINT_HPP_
#define UTILITIES_SYS_EXTINT_HPP_

#include "inc_stdlib.h"
#include "inc_gnc.h"
#include "sys_extint_port.hpp"



#if defined(D_RT1052_SYS_EXTINT_PORT_HPP_) //CPU Selection

class extInt_t
{
public:
	typedef void (*handler_t)(void);

	static std::map<uint32_t, std::map<uint32_t, extInt_t>> isrSet;
	static extInt_t& insert(GPIO_Type* _gpio, uint32_t _pin, handler_t _handler, gpio_interrupt_mode_t _mode, bool _enable);
	static void isr(GPIO_Type* _gpio);

	GPIO_Type* gpio;
	uint32_t pin;
	handler_t handler;
	gpio_interrupt_mode_t mode;

	void setup(GPIO_Type* _gpio, uint32_t _pin, handler_t _handler, gpio_interrupt_mode_t _mode, bool _enable);

	void setMode(gpio_interrupt_mode_t _mode)
	{
		mode = _mode;
		GPIO_PinSetInterruptConfig(gpio, pin, _mode);
	}
	void setEnable(bool b)
	{
		GPIO_PortEnableInterrupts(gpio, 1 << pin);
	}
	extInt_t(void);
private:
	
	extInt_t(GPIO_Type* _gpio, uint32_t _pin, handler_t _handler, gpio_interrupt_mode_t _mode, bool _enable)
	{
		setup(_gpio, _pin, _handler, _mode, _enable);
	}
};
#else	//CPU Selection

#error "C++ API does NOT support this CPU!"

#endif // CPU Selection

#endif // ! UTILITIES_SYS_EXTINT_HPP_


