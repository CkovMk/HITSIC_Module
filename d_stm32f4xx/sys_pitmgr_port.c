#include <sys_pitmgr.h>

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#include "tim.h"

#ifdef __cplusplus
extern "C"{
#endif

#if defined(HITSIC_PITMGR_DEFAULT_IRQ) && (HITSIC_PITMGR_DEFAULT_IRQ > 0)
void TIM1_UP_TIM10_IRQHandler(void)
{
    extern pitmgr_t pitmgr_main;
    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
    PITMGR_Isr(&pitmgr_main);
}
#endif // ! HTISIC_PITMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_PITMGR
