/*
 * sys_extint_port.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#include "sys_extint.hpp"

PORT_Type *EXTINT_GetPortInst(GPIO_Type *gpio)
{
	static PORT_Type *lut[5] = PORT_BASE_PTRS;
	return lut[((uint32_t)gpio - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)];
}


#ifdef __cplusplus
extern "C"{
#endif

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
void PORTA_IRQHandler(void){PORTX_IRQHandler(PORTA, &PORTA_irq_list);}
void PORTB_IRQHandler(void){PORTX_IRQHandler(PORTB, &PORTB_irq_list);}
void PORTC_IRQHandler(void){PORTX_IRQHandler(PORTC, &PORTC_irq_list);}
void PORTD_IRQHandler(void){PORTX_IRQHandler(PORTD, &PORTD_irq_list);}
void PORTE_IRQHandler(void){PORTX_IRQHandler(PORTE, &PORTE_irq_list);}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

