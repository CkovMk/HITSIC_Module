#include <sys_pitmgr.h>

#if defined(HITSIC_PITMGR_DEFAULT_IRQ) && (HITSIC_PITMGR_DEFAULT_IRQ > 0)

pitmgr_t pitmgr_main;

void LPTMR0_IRQHandler(void)
{
	LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
	PITMGR_Isr(&pitmgr_main);
}

#endif // ! HTISIC_PITMGR_USE_IRQHANDLER
