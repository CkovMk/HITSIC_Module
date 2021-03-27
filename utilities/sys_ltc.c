#include "sys_ltc.h"

#if (defined(HITSIC_USE_LTC) && (HITSIC_USE_LTC != 0U))

uint64_t PITMGR_GetLTC(void)
{
    return ULLONG_MAX - PIT_GetLifetimeTimerCount(PIT);
}

uint64_t PITMGR_GetLTC_us(void)
{
    return COUNT_TO_USEC(PITMGR_GetLTC(), RTE_PIT_CLKFREQ);
}

uint64_t PITMGR_GetLTC_ms(void)
{
    return COUNT_TO_MSEC(PITMGR_GetLTC(), RTE_PIT_CLKFREQ);
}

void PITMGR_Delay(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + _t;
    while (PITMGR_GetLTC() < cnt);
}

void PITMGR_Delay_us(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + USEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
    while (PITMGR_GetLTC() < cnt);
}

void PITMGR_Delay_ms(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + MSEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
    while (PITMGR_GetLTC() < cnt);
}

#endif // ! HITSIC_USE_LTC
