#ifndef D_MK24F12_SYS_EXTINT_PORT_H
#define D_MK24F12_SYS_EXTINT_PORT_H

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
#define EXTINT_ClearInterruptFlags(_intc, _mask) 	    PORT_ClearPinsInterruptFlags(_intc, _mask)


#define HTISIC_EXTINT_DEFAULT_IRQ 	(1U)

#ifdef __cplusplus
extern "C"{
#endif

inline INTC_Type *EXTINT_GetPortInst(GPIO_Type *gpio)
{
	static PORT_Type *lut[] = PORT_BASE_PTRS;
	return lut[((uint32_t)gpio - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)];
}

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_EXTINT

#endif // ! D_MK24F12_SYS_EXTINT_PORT_H
