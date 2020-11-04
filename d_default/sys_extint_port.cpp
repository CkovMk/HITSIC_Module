#include "sys_extint.hpp"

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)

#ifdef __cplusplus
extern "C"{
#endif

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
void Default_IRQHandler(void){}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_EXTINT
