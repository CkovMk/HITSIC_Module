/*
 * sys_uartmgr_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_DEFAULT_SYS_UARTMGR_PORT_HPP_
#define D_DEFAULT_SYS_UARTMGR_PORT_HPP_

#include "hitsic_common.h"

#ifndef HITSIC_USE_UARTMGR
#define HITSIC_USE_UARTMGR 		(0U)
#endif // ! HITSIC_USE_UARTMGR

#if defined(HITSIC_USE_UARTMGR) && (HITSIC_USE_UARTMGR > 0)




#define UART_TPCTRL_HEADER 0xae


#define UARTMGR_INST_DEFINITION	\
	static uartMgr_t uart0(UART0);
	//static uartMgr_t uart1(UART1);

#define UARTMGR_INST_SWITCHCASE \
	case UART0_BASE: return uart0; break;
	//case UART1_BASE: return uart1; break;

#endif // ! HITSIC_USE_UARTMGR

#endif // ! D_DEFAULT_SYS_UARTMGR_PORT_HPP_
