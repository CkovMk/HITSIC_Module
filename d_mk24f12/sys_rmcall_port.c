#include "sys_rmcall_port.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

extern rmcall_t rmcall_host;

volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;


const uart_config_t UART1_BLE_config = {
  .baudRate_Bps = 115200UL,
  .parityMode = kUART_ParityDisabled,
  .stopBitCount = kUART_OneStopBit,
  .txFifoWatermark = 0U,
  .rxFifoWatermark = 1U,
  .idleType = kUART_IdleTypeStartBit,
  .enableTx = true,
  .enableRx = true
};
uart_handle_t UART1_BLE_handle;
uart_transfer_t UART1_BLE_rxTransfer = {
  .rxData = NULL,
  .dataSize = 0
};
uart_transfer_t UART1_BLE_txTransfer = {
  .data = NULL,
  .dataSize = 0,
};

void RMCALL_PORT_Init(void)
{
	  UART_Init(HITSIC_RMCALL_UART, &UART1_BLE_config, HITSIC_RMCALL_UART_CLK_FREQ);
	  UART_TransferCreateHandle(HITSIC_RMCALL_UART, &UART1_BLE_handle, RMCALL_HOST_CpltCallback, NULL);
}

status_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize)
{
	if (!txOnGoing)
	{
		UART1_BLE_txTransfer.data     = (uint8_t *)_data;
		UART1_BLE_txTransfer.dataSize = _dataSize;
		txOnGoing = true;
		return UART_TransferSendNonBlocking(HITSIC_RMCALL_UART, &UART1_BLE_handle, &UART1_BLE_txTransfer);
	}
}
status_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize)
{
    if (!rxOnGoing)
    {
        rxOnGoing = true;
		UART1_BLE_rxTransfer.data     = (uint8_t *)_data;
		UART1_BLE_rxTransfer.dataSize = _dataSize;
        return UART_TransferReceiveNonBlocking(HITSIC_RMCALL_UART, &UART1_BLE_handle, &UART1_BLE_rxTransfer, _dataSize);
    }
}
void RMCALL_HOST_TxAbort(void)
{
	UART_TransferAbortSend(HITSIC_RMCALL_UART, &UART1_BLE_handle);
	txOnGoing = false;
}
void RMCALL_HOST_RxAbort(void)
{
	UART_TransferAbortReceive(HITSIC_RMCALL_UART, &UART1_BLE_handle);
	rxOnGoing = false;
}

void RMCALL_HOST_CpltCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
    if (kStatus_UART_TxIdle == status)
    {
        txOnGoing    = false;
        RMCALL_TxIsr(&rmcall_host);
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxOnGoing     = false;
        RMCALL_RxIsr(&rmcall_host);
    }
}

#endif // HITSIC_USE_RMCALL
