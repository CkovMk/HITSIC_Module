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

#ifndef D_RT1052_SYS_EXTINT_PORT_HPP_
#define D_RT1052_SYS_EXTINT_PORT_HPP_

#include "hitsic_common.h"

//config marco

//HAL marco
#define INTC_Type 			GPIO_Type
#define interrupt_mode_t 		gpio_interrupt_mode_t

#define EXTINT_SetInterruptConfig(_base, _pin, _cfg) 	GPIO_PinSetInterruptConfig(_base, _pin, _cfg)
#define EXTINT_GetInterruptFlags(_base)  				GPIO_PortGetInterruptFlags(_base)
#define EXTINT_ClearInterruptFlags(_base, _mask) 				GPIO_PortClearInterruptFlags(_base, _mask)

#define HITSIC_EXTMGR_INITLIZE 		(1U)

#if defined(HITSIC_EXTMGR_INITLIZE) && (HITSIC_EXTMGR_INITLIZE > 0)
inline void EXTINT_PlatformInit(void)
{
    NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, 6);
    NVIC_SetPriority(GPIO1_Combined_16_31_IRQn, 6);
    NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, 6);
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, 6);
    NVIC_SetPriority(GPIO3_Combined_0_15_IRQn, 6);
    NVIC_SetPriority(GPIO3_Combined_16_31_IRQn, 6);

    EnableIRQ(GPIO1_Combined_0_15_IRQn);
    EnableIRQ(GPIO1_Combined_16_31_IRQn);
    EnableIRQ(GPIO2_Combined_0_15_IRQn);
    EnableIRQ(GPIO2_Combined_16_31_IRQn);
    EnableIRQ(GPIO3_Combined_0_15_IRQn);
    EnableIRQ(GPIO3_Combined_16_31_IRQn);
}
#endif // ! HITSIC_EXTMGR_INITLIZE

#define HTISIC_EXTINT_DEFAULT_IRQ 	(1U)

inline INTC_Type* EXTINT_GetPortInst(GPIO_Type *gpio)
{
    return gpio;
}

#endif // ! D_RT1052_SYS_EXTINT_PORT_HPP_
