/*
 * sys_extint_port.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#include "sys_extint.hpp"



#ifdef __cplusplus
extern "C"{
#endif

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
void PORTA_IRQHandler(void){PORTX_IRQHandler(PORTA, &PORTA_irq_list);}
void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
{
	PORTX_IRQHandler(PORTB, &PORTB_irq_list);
	PORTX_IRQHandler(PORTC, &PORTC_irq_list);
	PORTX_IRQHandler(PORTD, &PORTD_irq_list);
	PORTX_IRQHandler(PORTE, &PORTE_irq_list);
}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

