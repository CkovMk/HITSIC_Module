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

#ifndef D_RT1052_SYS_PITMGR_PORT_HPP_
#define D_RT1052_SYS_PITMGR_PORT_HPP_

#include "hitsic_common.h"

#define HITSIC_USE_PITMGR (1U)

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#define HITSIC_PITMGR_CNTFREQ	 	(75000000U)

#define HITSIC_PITMGR_INITLIZE		(1U)

#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
inline void PITMGR_PlatformInit(void)
{
    status_t rStatus = kStatus_Success;

    pit_config_t pit_cfg =
    { .enableRunInDebug = true, };
    PIT_Init(PIT, &pit_cfg);

    NVIC_SetPriority(PIT_IRQn, 4);
    EnableIRQ(PIT_IRQn);

    //kPIT_Chnl2 used for generating interrupts
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2,
            MSEC_TO_COUNT(1, HITSIC_PITMGR_CNTFREQ));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, kPIT_Chnl_2);

    return rStatus;
}
#endif // ! HITSIC_PITMGR_INITLIZE

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)

#endif // ! HITSIC_USE_PITMGR

#endif // ! D_RT1052_SYS_PITMGR_PORT_HPP_
