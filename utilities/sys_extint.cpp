#include <sys_extint.hpp>

#if defined(D_RT1052_SYS_EXTINT_PORT_HPP_) //CPU Selection

std::map<uint32_t, std::map<uint32_t, extInt_t>> extInt_t::isrSet;

extInt_t& extInt_t::insert(GPIO_Type* _gpio, uint32_t _pin, handler_t _handler, gpio_interrupt_mode_t _mode, bool _enable)
{
	isrSet[(uint32_t)_gpio][_pin] = extInt_t(_gpio, _pin, _handler, _mode, _enable);
	return isrSet[(uint32_t)_gpio][_pin];
}

void extInt_t::isr(GPIO_Type* _gpio)
{
	uint32_t flag = GPIO_PortGetInterruptFlags(_gpio);
	
	for (auto it : isrSet[(uint32_t)_gpio])
	{
		if (flag & (1 << it.first))
		{
			(*it.second.handler)();
		}
	}
	GPIO_PortClearInterruptFlags(_gpio, 0xffff);
}

void extInt_t::setup(GPIO_Type* _gpio, uint32_t _pin, handler_t _handler, gpio_interrupt_mode_t _mode, bool _enable)
{
#ifdef DEBUG
	if (_handler == NULL)
	{
		throw std::invalid_argument(std::string("extInt_t setup failure.\n"));
		return;
	}
#else

#endif // ! DEBUG
	gpio = _gpio;
	pin = _pin;
	handler = _handler;
	setMode(_mode);
	setEnable(_enable);
}

#endif // ! D_RT1052_SYS_PITMGR_PORT_HPP_
