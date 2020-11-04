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

#ifndef D_DEFAULT_SYS_EXTINT_PORT_HPP_
#define D_DEFAULT_SYS_EXTINT_PORT_HPP_

#include "hitsic_common.h"

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)

typedef PORT_Type INTC_Type;
typedef port_interrupt_t extInt_interruptMode_t;

#define EXTINT_InterruptOrDMADisabled	kPORT_InterruptOrDMADisabled
#define EXTINT_DMARisingEdge 			kPORT_DMARisingEdge
#define EXTINT_DMAFallingEdge 			kPORT_DMAFallingEdge
#define EXTINT_DMAEitherEdge 			kPORT_DMAEitherEdge
#define EXTINT_InterruptLogicZero 		kPORT_InterruptLogicZero
#define EXTINT_InterruptRisingEdge 		kPORT_InterruptRisingEdge
#define EXTINT_InterruptFallingEdge 	kPORT_InterruptFallingEdge
#define EXTINT_InterruptEitherEdge 		kPORT_InterruptEitherEdge
#define EXTINT_InterruptLogicOne		kPORT_InterruptLogicOne

#define EXTINT_SetInterruptConfig(_intc, _pin, _cfg) 	PORT_SetPinInterruptConfig(_intc, _pin, _cfg)
#define EXTINT_GetInterruptFlags(_intc) 				PORT_GetPinsInterruptFlags(_intc)
#define EXTINT_ClearInterruptFlags(_intc, _mask) 				PORT_ClearPinsInterruptFlags(_intc, _mask)



#ifndef HITSIC_EXTMGR_INITLIZE
#define HITSIC_EXTMGR_INITLIZE 		(1U)
#endif // ! HITSIC_EXTMGR_INITLIZE

#if defined(HITSIC_EXTMGR_INITLIZE) && (HITSIC_EXTMGR_INITLIZE > 0)
inline void EXTINT_PlatformInit(void)
{

}
#endif // ! HITSIC_EXTMGR_INITLIZE


#define HTISIC_EXTINT_DEFAULT_IRQ 	(1U)


inline INTC_Type *EXTINT_GetPortInst(GPIO_Type *gpio)
{
	static PORT_Type *lut[] = PORT_BASE_PTRS;
	return lut[((uint32_t)gpio - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)];
}

#endif // ! HITSIC_USE_EXTINT

#endif // ! D_DEFAULT_SYS_EXTINT_PORT_HPP_
