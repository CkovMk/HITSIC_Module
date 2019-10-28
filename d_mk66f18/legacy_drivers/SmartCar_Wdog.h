#ifndef _SMARTCAR_WDOG_H
#define _SMARTCAR_WDOG_H
#include "stdafx.h"

#define WDOG_WCT_INSTRUCITON_COUNT (256U)

#ifdef __cplusplus
extern "C" {
#endif

void Wdog_Init(uint32_t ms);
void Wdog_Feed(void);

#ifdef __cplusplus
}
#endif

#endif // WDOG_H_INCLUDED
