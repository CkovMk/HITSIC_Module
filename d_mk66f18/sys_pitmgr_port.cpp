/*
 * sys_pitmgr_port.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#include "sys_pitmgr.hpp"

#ifdef __cplusplus
extern "C"{
#endif

#if defined(HITSIC_PITMGR_DEFAULT_IRQ) && (HITSIC_PITMGR_DEFAULT_IRQ > 0)
void PIT2_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	pitMgr_t::isr();
}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif



