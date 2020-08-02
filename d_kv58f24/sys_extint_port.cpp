#include "sys_extint.hpp"



#ifdef __cplusplus
extern "C"{
#endif

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
void PORTA_IRQHandler(void){extInt_t::isr(PORTA);}
void PORTB_IRQHandler(void){extInt_t::isr(PORTB);}
void PORTC_IRQHandler(void){extInt_t::isr(PORTC);}
void PORTD_IRQHandler(void){extInt_t::isr(PORTD);}
void PORTE_IRQHandler(void){extInt_t::isr(PORTE);}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

