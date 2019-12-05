/*
 * sys_extint_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_MK66F18_SYS_EXTINT_PORT_HPP_
#define D_MK66F18_SYS_EXTINT_PORT_HPP_

#include "inc_fsl_mk66f18.h"

#define HTISIC_EXTINT_DEFAULT_IRQ 1
#define PORT_SetPinInterruptConfig


typedef port_interrupt_t gpio_interrupt_mode_t;

PORT_Type *EXTINT_GetPortInst(GPIO_Type *gpio);



#endif /* D_MK66F18_SYS_EXTINT_PORT_HPP_ */
