#include <sys_pitmgr.h>

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#ifdef __cplusplus
extern "C"{
#endif

#if defined(HITSIC_PITMGR_DEFAULT_IRQ) && (HITSIC_PITMGR_DEFAULT_IRQ > 0)
void LPTMR0_IRQHandler(void)
{
    extern pitmgr_t pitmgr_main;
	LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
	PITMGR_Isr(&pitmgr_main);
}
#endif // ! HTISIC_PITMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_PITMGR
