/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * sys_pitmgr_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_KV58F24_SYS_PITMGR_PORT_HPP_
#define D_KV58F24_SYS_PITMGR_PORT_HPP_

#include "hitsic_common.h"

#ifndef HITSIC_USE_PITMGR
#define HITSIC_USE_PITMGR 		(0U)
#endif // ! HITSIC_USE_PITMGR

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#define HITSIC_PITMGR_CNTFREQ 60000000

#define HITSIC_PITMGR_INITLIZE		(0U)

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
	NVIC_SetPriority(PIT2_IRQn, 4);
	EnableIRQ(PIT2_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_2);
}
#endif // ! HITSIC_PITMGR_INITLIZE

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)

#endif // ! HITSIC_USE_PITMGR

#endif // ! D_KV58F24_SYS_PITMGR_PORT_HPP_
