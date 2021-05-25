#ifndef D_MK66F18_SYS_RMCALL_PORT_H
#define D_MK66F18_SYS_RMCALL_PORT_H
#include "hitsic_common.h"
#include "inc_stdlib.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

//#define HITSIC_RMCALL_UART LPUART2

#define HITSIC_RMCALL_USE_HEARTBEAT (1U)

#if defined(HITSIC_RMCALL_USE_HEARTBEAT) && (HITSIC_RMCALL_USE_HEARTBEAT > 0)
// #define HITSIC_RMCALL_PITISR_TIMELAG 100
// #define HITSIC_RMCALL_HEARTBEAT_TIMELAG 1000
// #define HITSIC_RMCALL_HEARTBEAT_FAIL_TH 2000
// #define HITSIC_RMCALL_HEARTBEAT_Callback() HardFault_IRQHandler()
// #endif // ! HITSIC_RMCALL_USE_HEARTBEAT



#endif // ! HITSIC_RMCALL_USE_HEARTBEAT

#define HITSIC_RMCALL_HEADER_MAGIC 0x554768A0U

void RMCALL_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

status_t RMCALL1_Tx(void *_data, uint32_t dataSize);
status_t RMCALL1_Rx(void *_data, uint32_t dataSize);

#endif // HITSIC_USE_RMCALL

#endif // ! D_MK66F18_SYS_RMCALL_PORT_H
