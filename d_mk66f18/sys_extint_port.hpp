/*
 * sys_extint_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_MK66F18_SYS_EXTINT_PORT_HPP_
#define D_MK66F18_SYS_EXTINT_PORT_HPP_

#include "inc_fsl_mk66f18.h"

//config marco
#define HTISIC_EXTINT_DEFAULT_IRQ 	(1U)
#define HITSIC_EXTMGR_INITLIZE 		(1U)


//HAL marco
#define INTC_Type 			PORT_Type
#define interrupt_mode_t 		port_interrupt_t
#define EXTINT_SetInterruptConfig(_intc, _pin, _cfg) 	PORT_SetPinInterruptConfig(_intc, _pin, _cfg)
#define EXTINT_GetInterruptFlags(_intc) 				PORT_GetPinsInterruptFlags(_intc)
#define EXTINT_ClearInterruptFlags(_intc, _mask) 				PORT_ClearPinsInterruptFlags(_intc, _mask)


/* reserved for RT1052:
#define EXTINT_SetInterruptConfig(_intc, _pin, _cfg) 	GPIO_PinSetInterruptConfig(_intc, _pin, _cfg)
#define EXTINT_GetInterruptFlags(_intc)  				GPIO_PortGetInterruptFlags(_intc)
#define EXTINT_ClearInterruptFlags(_intc) 				GPIO_PortClearInterruptFlags(_intc)

*/


PORT_Type *EXTINT_GetPortInst(GPIO_Type *gpio);



#endif /* D_MK66F18_SYS_EXTINT_PORT_HPP_ */
