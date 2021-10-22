#ifndef D_MK24F12_SYS_RMCALL_PORT_H
#define D_MK24F12_SYS_RMCALL_PORT_H
#include "hitsic_common.h"
#include "inc_stdlib.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)


#define HITSIC_RMCALL_UART UART1
#define HITSIC_RMCALL_CLKSRC   UART1_CLK_SRC
#define HITSIC_RMCALL_UART_CLK_FREQ CLOCK_GetFreq(UART1_CLK_SRC)

extern uart_handle_t UART_BLE_handle;
extern uart_transfer_t UART_BLE_rxTransfer;
extern uart_transfer_t UART_BLE_txTransfer;

#define HITSIC_RMCALL_HEADER_MAGIC 0x554768A0U
#define RMCALL_SYSLOG_LVL (2U)
#define HITSIC_RMCALL_PUBLIC_BUF_SIZE (256U)
#define kStatusGroup_RMCALL (201U)


status_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize);
status_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize);
void RMCALL_HOST_TxAbort(void);
void RMCALL_HOST_RxAbort(void);

void RMCALL_HOST_CpltCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

void RMCALL_PORT_Init(void);

#endif // HITSIC_USE_RMCALL

#endif // ! D_MK24F12_SYS_RMCALL_PORT_H
