#pragma once
#ifndef UTILITIES_SYS_PITMGR_HPP_
#define UTILITIES_SYS_PITMGR_HPP_
#include "inc_stdlib.h"
#include "inc_gnc.h"
#include "hitsic_common.h"
#include "sys_pitmgr_port.hpp"

//CPU Selection
#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) || defined (D_MK66F18_SYS_PITMGR_PORT_HPP_)

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

	static status_t init(void);
	static pitMgr_t* insert(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);
	static status_t remove(pitMgr_t& _handle);
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




// C wrapper
#define PITMGR_Init()			pitMgr_t::init()
#define PITMGR_GetLTC()			pitMgr_t::getLTC()
#define PITMGR_GetLTC_us()		pitMgr_t::getLTC_us()
#define PITMGR_GetLTC_ms()		pitMgr_t::getLTC_ms()
#define PITMGR_Delay(_t)		pitMgr_t::delay(_t)
#define PITMGR_Delay_us(_t)		pitMgr_t::delay_us(_t)
#define PITMGR_Delay_ms(_t)		pitMgr_t::delay_ms(_t)

#define pitmgr_handle_t pitMgr_t
#define pitmgr_pptEnable		pitMgr_t::enable
#define pitmgr_pptRunOnce		pitMgr_t::runOnce

#define PITMGR_HandleSetup(_h, _ms, _mso, _handler, _ppt)	_h->setup(_ms, _mso, _handler, _ppt)
#define PITMGR_HandleInsert(_ms, _mso, _handler, _ppt) 		pitMgr_t::insert(_ms, _mso, _handler, _ppt)
#define PITMGR_HandleSetEnable(_h, b)		_h->setEnable(b)
#define PITMGR_HandleRemove(_h)				pitMgr_t::remove(_h)
#define PITMGR_Isr()						pitMgr_t::isr()



#else	//CPU Selection

#error "C++ API does NOT support this CPU!"

#endif // CPU Selection


#endif // ! UTILITIES_SYS_PITMGR_HPP_
