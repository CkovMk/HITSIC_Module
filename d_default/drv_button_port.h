#ifndef D_DEFAULT_DRV_BUTTON_PORT_H
#define D_DEFAULT_DRV_BUTTON_PORT_H
#include "hitsic_common.h"
#include "sys_extint.hpp"
#include "sys_pitmgr.hpp"

#if defined(HITSIC_USE_DRV_BUTTON) && (HITSIC_USE_DRV_BUTTON > 0)

#define BUTTON_TIME_SHRT 50u     //short press
#define BUTTON_SHRT_TOUT 200u   //short timeout
#define BUTTON_TIME_LONG 500u   //long press
#define BUTTON_TIME_LRPT 100u   //long press repeat, time calculated from key press. the actual time for repeat will be REPT-LONG
#define BUTTON_REPT_TOUT 550u   //repeat timeout
#define BUTTON_TIME_INTV 50u

#define BUTTON_PRESSDN_LOGIC 0u
#define BUTTON_RELEASE_LOGIC 1u
#define BUTTON_PRESSDN_EXTINT EXTINT_InterruptFallingEdge //define which edge indicates button pressed.
#define BUTTON_RELEASE_EXTINT EXTINT_InterruptRisingEdge  //define which edge indicates button release.

#define BUTTON_PITMGR_TIME_MS 10

#define BUTTON_TIMER_MS     (pitMgr_t::timer_ms)

#endif // ! HITSIC_USE_DRV_BUTTON

#endif // ! D_DEFAULT_DRV_BUTTON_PORT_H
