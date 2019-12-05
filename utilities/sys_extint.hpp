#pragma once
#ifndef UTILITIES_SYS_EXTINT_HPP_
#define UTILITIES_SYS_EXTINT_HPP_
#include "inc_stdlib.h"
#include "inc_gnc.h"
#include "hitsic_common.h"
#include "sys_extint_port.hpp"


//CPU Selection
#if defined(D_RT1052_SYS_EXTINT_PORT_HPP_) || defined(D_MK66F18_SYS_EXTINT_PORT_HPP_)

class extInt_t
{
public:
	typedef void (*handler_t)(void);

	static std::map<INTC_Type*, std::map<uint32_t, extInt_t>> isrSet;

	static status_t init(void);
	static extInt_t* insert(INTC_Type* _gpio, uint32_t _pin, handler_t _handler);
	static status_t remove(INTC_Type* _gpio, uint32_t _pin);
	static status_t remove(extInt_t* _inst);
	static void isr(INTC_Type* _gpio);

	INTC_Type* gpio;
	uint32_t pin;
	handler_t handler;

	void setup(INTC_Type* _gpio, uint32_t _pin, handler_t _handler);

	void setMode(interrupt_mode_t _mode)
	{
		//mode = _mode;
		EXTINT_SetInterruptConfig(gpio, pin, _mode);
	}
	extInt_t(void){}
private:
	extInt_t(INTC_Type* _gpio, uint32_t _pin, handler_t _handler)
	{
		setup(_gpio, _pin, _handler);
	}
};


#define EXTINT_Init()	extInt_t::init()
#define EXTINT_PortSetup(_gpio, _pin, _prio, _handler)	extInt_t::insert(_gpio, _pin, _handler)
#define EXTINT_PortRemove(_gpio, _pin) 		extInt_t::remove(_gpio, _pin)
#define PORTX_IRQHandler(_gpio, _list)		extInt_t::isr(_gpio)


#else	//CPU Selection

#error "C++ API does NOT support this CPU!"

#endif // CPU Selection

#endif // ! UTILITIES_SYS_EXTINT_HPP_


