#include "sys_extint.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
void GPIO1_Combined_0_15_IRQHandler(void)
{
    extInt_t::isr(GPIO1);
}

void GPIO1_Combined_16_31_IRQHandler(void)
{
    extInt_t::isr(GPIO1);
}

void GPIO2_Combined_0_15_IRQHandler(void)
{
    extInt_t::isr(GPIO2);
}

void GPIO2_Combined_16_31_IRQHandler(void)
{
    extInt_t::isr(GPIO2);
}

void GPIO3_Combined_0_15_IRQHandler(void)
{
    extInt_t::isr(GPIO3);
}

void GPIO3_Combined_16_31_IRQHandler(void)
{
    extInt_t::isr(GPIO3);
}
//TODO: fix this
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

