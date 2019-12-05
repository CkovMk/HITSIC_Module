#include "sys_pitmgr.hpp"

//CPU Selection
#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) || defined (D_MK66F18_SYS_PITMGR_PORT_HPP_)

//external definition for static varibles in class "pitMgr_t".
std::list<pitMgr_t> pitMgr_t::isrSet;
uint32_t pitMgr_t::timer_ms = 0U;

status_t pitMgr_t::init(void)
{
	isrSet.clear();
	timer_ms = 0U;
#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
	pit_config_t cfg;
	{
		cfg.enableRunInDebug = true;
	}
	PIT_Init(PIT, &cfg);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, ULONG_MAX);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, ULONG_MAX);
	PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(1000, RTE_PIT_CLKFREQ));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
	NVIC_SetPriority(PIT2_IRQn, 8);
	EnableIRQ(PIT2_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
	PIT_StartTimer(PIT, kPIT_Chnl_1);
	PIT_StartTimer(PIT, kPIT_Chnl_2);
#endif // HITSIC_PITMGR_INITLIZE
	return kStatus_Success;
}


pitMgr_t* pitMgr_t::insert(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt)
{
	isrSet.push_back(pitMgr_t(_ms, _mso, _handler, _ppt));
	return &isrSet.back();
}
status_t pitMgr_t::remove(pitMgr_t& _handle)
{
	HAL_EnterCritical();
	for (auto it = isrSet.begin(); it != isrSet.end(); ++it)
	{
		if(&(*it) == &_handle)
		{
			isrSet.erase(it);
			HAL_ExitCritical();
			return kStatus_Success;
		}
	}
	HAL_ExitCritical();
	return kStatus_Fail;
}

void pitMgr_t::isr(void)
{
	uint64_t initialTime = getLTC_us();
	for (auto isr : isrSet)
	{
		if (isr.pptFlag & enable && timer_ms % isr.ms == isr.mso)
		{
			uint64_t timeBuf = getLTC_us();
			(*isr.handler)();
			if (isr.pptFlag & runOnce)
			{
				isr.pptFlag ^= enable;
			}
			isr.prevTime_us = getLTC_us() - timeBuf;
		}
	}
	++timer_ms;
}

void pitMgr_t::setup(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt)
{
#ifdef DEBUG
	if (_mso >= _ms || _handler == NULL)
	{
		throw std::invalid_argument(std::string("pitMgr_t setup failure.\n"));
		return;
	}
#else

#endif // ! DEBUG
	HAL_EnterCritical();
	ms = _ms;
	mso = _mso;
	handler = _handler;
	pptFlag = _ppt;
	prevTime_us = 0;
	HAL_ExitCritical();
}


#endif // CPU Selection


