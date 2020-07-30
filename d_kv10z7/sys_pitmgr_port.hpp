/*
 * sys_pitmgr_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_KV10Z7_SYS_PITMGR_PORT_HPP_
#define D_KV10Z7_SYS_PITMGR_PORT_HPP_

#include "inc_fsl_kv10z7.h"

#define HITSIC_USE_PITMGR (1U)

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#define HITSIC_PITMGR_CNTFREQ	 	(4000000U)

#define HITSIC_PITMGR_INITLIZE		(0U)

#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
inline void PITMGR_PlatformInit(void)
{
	const lptmr_config_t LPTMR0_config = {
	  .timerMode = kLPTMR_TimerModeTimeCounter,
	  .pinSelect = kLPTMR_PinSelectInput_0,
	  .pinPolarity = kLPTMR_PinPolarityActiveHigh,
	  .enableFreeRunning = false,
	  .bypassPrescaler = true,
	  .prescalerClockSource = kLPTMR_PrescalerClock_0,
	  .value = kLPTMR_Prescale_Glitch_0
	};
	/* Initialize the LPTMR */
	LPTMR_Init(LPTMR0, &LPTMR0_config);
	/* Set LPTMR period */
	LPTMR_SetTimerPeriod(LPTMR0, 4000);
	/* Configure timer interrupt */
	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	/* Interrupt vector LPTMR0_IRQn priority settings in the NVIC */
	NVIC_SetPriority(LPTMR0_IRQn, 4);
	/* Enable interrupt LPTMR0_IRQn request in the NVIC */
	EnableIRQ(LPTMR0_IRQn);
}
#endif // ! HITSIC_PITMGR_INITLIZE

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)

#endif // ! HITSIC_USE_PITMGR

#endif /* D_KV10Z7_SYS_PITMGR_PORT_HPP_ */
