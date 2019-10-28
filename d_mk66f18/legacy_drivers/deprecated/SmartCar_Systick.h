#ifndef _SMARTCAR_SYSTICK_H
#define _SMARTCAR_SYSTICK_H


//#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_ftm.h"
#include "clock_config.h"
#include "pin_mux.h"

#define     DELAY_MS(ms)                         systick_delay_ms(ms);

void    systick_delay(uint32_t time);
void    systick_delay_ms(uint32_t ms);
void    systick_timing(uint32_t time);
uint32_t  systick_getval(void);


#endif
