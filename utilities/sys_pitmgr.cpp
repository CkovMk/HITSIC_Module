#include <sys_pitmgr.hpp>

#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) //CPU Selection

//external definition for static varibles in class "pitMgr_t".
std::list<pitMgr_t> pitMgr_t::isrSet;
uint32_t pitMgr_t::timer_ms;


pitMgr_t& pitMgr_t::insert(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt)
{
	isrSet.push_back(pitMgr_t(_ms, _mso, _handler, _ppt));
	return isrSet.back();
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
	ms = _ms;
	mso = _mso;
	handler = _handler;
	pptFlag = _ppt;
	prevTime_us = 0;
}


#endif // CPU Selection


