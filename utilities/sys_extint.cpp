#include <sys_extint.hpp>

//CPU Selection
#if defined(D_RT1052_SYS_EXTINT_PORT_HPP_) || defined(D_MK66F18_SYS_EXTINT_PORT_HPP_)

std::map<INTC_Type*, std::map<uint32_t, extInt_t>> extInt_t::isrSet;

status_t extInt_t::init(void)
{
	status_t result = kStatus_Success;
	isrSet.clear();
#if defined(HITSIC_EXTMGR_INITLIZE) && (HITSIC_EXTMGR_INITLIZE > 0)
    NVIC_SetPriority(PORTA_IRQn, 6);
	NVIC_SetPriority(PORTB_IRQn, 6);
	NVIC_SetPriority(PORTC_IRQn, 6);
	NVIC_SetPriority(PORTD_IRQn, 6);
	NVIC_SetPriority(PORTE_IRQn, 6);
	EnableIRQ(PORTA_IRQn);
	EnableIRQ(PORTB_IRQn);
	EnableIRQ(PORTC_IRQn);
	EnableIRQ(PORTD_IRQn);
	EnableIRQ(PORTE_IRQn);
#endif // ! HITSIC_EXTMGR_INITLIZE
	return result;
}

extInt_t* extInt_t::insert(INTC_Type* _gpio, uint32_t _pin, handler_t _handler)
{
	isrSet[_gpio][_pin] = extInt_t(_gpio, _pin, _handler);
	return &isrSet[_gpio][_pin];
}

status_t extInt_t::remove(INTC_Type* _gpio, uint32_t _pin)
{
	HAL_EnterCritical();
	auto set_it = isrSet.find(_gpio);
	if(set_it == isrSet.end())
	{
		HAL_ExitCritical();
		return kStatus_Fail;
	}
	auto it = set_it->second.find(_pin);
	if(it != set_it->second.end())
	{
		set_it->second.erase(it);

	}
	HAL_ExitCritical();
	return kStatus_Success;
}

status_t extInt_t::remove(extInt_t* _inst)
{
	return remove(_inst->gpio, _inst->pin);
}

void extInt_t::isr(INTC_Type* _gpio)
{
	uint32_t flag = EXTINT_GetInterruptFlags(_gpio);
	
	for (auto it : isrSet[_gpio])
	{
		if (flag & (1 << it.first))
		{
			(*it.second.handler)();
		}
	}
	EXTINT_ClearInterruptFlags(_gpio, 0xffff);
}

void extInt_t::setup(INTC_Type* _gpio, uint32_t _pin, handler_t _handler)
{
#ifdef DEBUG
	if (_handler == NULL)
	{
		throw std::invalid_argument(std::string("extInt_t setup failure.\n"));
		return;
	}
#else

#endif // ! DEBUG
	HAL_EnterCritical();
	gpio = _gpio;
	pin = _pin;
	handler = _handler;
	HAL_ExitCritical();
}

#endif // ! D_RT1052_SYS_PITMGR_PORT_HPP_
