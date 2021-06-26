#ifndef D_STM32F4XX_SYS_RMCALL_PORT_H
#define D_STM32F4XX_SYS_RMCALL_PORT_H
#include "hitsic_common.h"
#include "inc_stdlib.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

#include "usart.h"
#include "stm32f4xx_hal_uart.h"

#define RMCALL_SYSLOG_LVL (2U)

//#define HITSIC_RMCALL_UART LPUART2

#define HITSIC_RMCALL_PUBLIC_BUF_SIZE (256U)

#define HITSIC_RMCALL_HEADER_MAGIC (0x554768A0U)

#define kStatusGroup_RMCALL (201U)

status_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize);
status_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize);
void RMCALL_HOST_TxAbort(void);
void RMCALL_HOST_RxAbort(void);

void RMCALL_HOST_TxCpltCallback(UART_HandleTypeDef *huart);
void RMCALL_HOST_RxCpltCallback(UART_HandleTypeDef *huart);

#endif // HITSIC_USE_RMCALL

#endif // ! D_STM32F4XX_SYS_RMCALL_PORT_H
