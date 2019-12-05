#pragma once
#ifndef UTILITIES_SYS_PITMGR_HPP_
#define UTILITIES_SYS_PITMGR_HPP_

#include "inc_stdlib.h"
#include "inc_gnc.h"
#include "sys_pitmgr_port.hpp"

#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) //CPU Selection

class pitMgr_t
{
public:

	//LifeTimeCounter
	static uint64_t getLTC(void)
	{
		return ULLONG_MAX - PIT_GetLifetimeTimerCount(PIT);
	}
	static uint64_t getLTC_us(void)
	{
		return COUNT_TO_USEC(getLTC(), RTE_PIT_CLKFREQ);
	}
	static uint64_t getLTC_ms(void)
	{
		return COUNT_TO_MSEC(getLTC(), RTE_PIT_CLKFREQ);
	}
	//delay using LifeTimeCounter
	static void delay(uint64_t _t)
	{
		uint64_t cnt = getLTC() + _t;
		while(getLTC() < cnt);
	}
	static void delay_us(uint64_t _t)
	{
		uint64_t cnt = getLTC() + USEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
		while(getLTC() < cnt);
	}
	static void delay_ms(uint64_t _t)
	{
		uint64_t cnt = getLTC() + MSEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
		while(getLTC() < cnt);
	}

	//isr service manager
	typedef void (*handler_t)(void);
	enum pptFlag_t : uint32_t
	{
		enable = 1 << 0,
		runOnce = 1 << 1,
		//msgWake = 1 << 2,
	};
	static std::list<pitMgr_t> isrSet;
	static uint32_t timer_ms;

	static pitMgr_t& insert(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);
	static status_t remove(pitMgr_t& _handle)
	{
		for (auto it = isrSet.begin(); it != isrSet.end(); ++it)
		{
			if(&(*it) == &_handle)
			{
				isrSet.erase(it);
				return kStatus_Success;
			}
		}
		return kStatus_Fail;
	}
	static void isr(void);

	//isr service content
	uint32_t ms, mso;
	handler_t handler;
	uint32_t pptFlag;
	uint64_t prevTime_us;


	void setup(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);
	void setEnable(bool _b)
	{
		if (_b)
		{
			pptFlag |= enable;
		}
		else
		{
			pptFlag &= (~enable);
		}
	}

	pitMgr_t(void);
	
private:

	pitMgr_t(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt)
	{
		setup(_ms, _mso, _handler, _ppt);
	}

};

#else	//CPU Selection

#error "C++ API does NOT support this CPU!"

#endif // CPU Selection


#endif // ! UTILITIES_SYS_PITMGR_HPP_
