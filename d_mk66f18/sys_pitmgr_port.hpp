/*
 * sys_pitmgr_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_MK66F18_SYS_PITMGR_PORT_HPP_
#define D_MK66F18_SYS_PITMGR_PORT_HPP_

#include "inc_fsl_mk66f18.h"

//TODO: fix this.
#define HITSIC_PITMGR_CNTFREQ 60000000

#define HITSIC_PITMGR_INITLIZE		(1U)

#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
inline void PITMGR_PlatformInit(void)
{
	pit_config_t cfg;
	{
		cfg.enableRunInDebug = true;
	}
	PIT_Init(PIT, &cfg);
	PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(1000, RTE_PIT_CLKFREQ));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
	NVIC_SetPriority(PIT2_IRQn, 8);
	EnableIRQ(PIT2_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_2);
}
#endif // ! HITSIC_PITMGR_INITLIZE

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)


#endif /* D_MK66F18_SYS_PITMGR_PORT_HPP_ */
