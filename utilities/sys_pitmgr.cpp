#include "sys_pitmgr.hpp"

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

/*!
 * @addtogroup pitmgr
 * @{
 */

//CPU Selection
#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) || defined (D_MK66F18_SYS_PITMGR_PORT_HPP_) || defined (D_KV10Z7_SYS_PITMGR_PORT_HPP_)

//external definition for static varibles in class "pitMgr_t".
std::list<pitMgr_t> pitMgr_t::isrSet;
uint32_t pitMgr_t::timer_ms = 0U;

status_t pitMgr_t::init(void)
{

	isrSet.clear();

	timer_ms = 0U;
#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
	PITMGR_PlatformInit();
#endif // HITSIC_PITMGR_INITLIZE
	return kStatus_Success;
}

pitMgr_t* pitMgr_t::insert(uint32_t _ms, uint32_t _mso, handler_t _handler,
		uint32_t _ppt)
{

	isrSet.push_back(pitMgr_t(_ms, _mso, _handler, _ppt));
	return &isrSet.back();

}
status_t pitMgr_t::remove(pitMgr_t &_handle)
{
	HAL_EnterCritical();
	for (auto it = isrSet.begin(); it != isrSet.end(); ++it)
	{
		if (&(*it) == &_handle)
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
	for (auto isr : isrSet)
	{
		if (isr.pptFlag & enable && timer_ms % isr.ms == isr.mso)
		{
			(*isr.handler)(isr.userData);
			if (isr.pptFlag & runOnce)
			{
				isr.pptFlag ^= enable;
			}
			isr.prevTime_ms = timer_ms;
		}
	}
	++timer_ms;
}

void pitMgr_t::setup(uint32_t _ms, uint32_t _mso, handler_t _handler,
		uint32_t _ppt)
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
	prevTime_ms = 0;
	HAL_ExitCritical();
}

#endif // CPU Selection

/* @} */

#endif // ! HITSIC_USE_PITMGR

