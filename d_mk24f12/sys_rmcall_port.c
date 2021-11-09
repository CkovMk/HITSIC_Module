#include "sys_rmcall_port.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

extern rmcall_t rmcall_host;
extern ECG_Data_Packet_t config_info;

uart_handle_t UART_BLE_handle;
uart_transfer_t UART_BLE_rxTransfer = {
  .rxData = NULL,
  .dataSize = 0
};
uart_transfer_t UART_BLE_txTransfer = {
  .data = NULL,
  .dataSize = 0,
};

rmcall_teleport_t teleport_host =
{
	RMCALL_HOST_Tx,
	RMCALL_HOST_Rx,
	RMCALL_HOST_TxAbort,
	RMCALL_HOST_RxAbort,
};
const rmcall_config_t config_host = 	{
		.teleport = &teleport_host
};

void ECG_recevieDataPacke(void *_recvData, uint16_t _recvSize, void *_userData)
{
    assert(_recvSize == 11);
    config_info.waveform = *(uint8_t *)_recvData;
    config_info.func     = *((uint8_t *)_recvData + 1);
    config_info.lead     = *((uint8_t *)_recvData + 2);
    config_info.voltage  = *((float *)(((uint8_t *)_recvData) + 3));
    config_info.period   = *((float *)(((uint8_t *)_recvData) + 7));
    static uint32_t retstatus = 0;
    RMCALL_CommandSend(&rmcall_host, GEN_STATUS_ID,&retstatus, sizeof(retstatus));
}

rmcall_handle_t ECG_rDPhandle =
{
    .handleId = ECG_INFO_ID,
    .handler = ECG_recevieDataPacke,
    .userData = NULL,
};

/* 通用命令 - 握手命令 */
void ECG_HandShake(void *_recvData, uint16_t _recvSize, void *_userData)
{
    assert(0U == _recvSize);
    static uint32_t retStatus = GEN_HSHAKE_ID;
    RMCALL_CommandSend(&rmcall_host, GEN_HSHAKE_ID, &retStatus, 4U);
    SYSLOG_I("ECG_HandShake.");
}
rmcall_handle_t ECG_handShakehandle =
{
    .handleId = GEN_HSHAKE_ID,
    .handler = ECG_HandShake,
    .userData = NULL,
};

void RMCALL_PORT_Init(void)
{
	UART_TransferCreateHandle(HITSIC_RMCALL_UART, &UART_BLE_handle, RMCALL_HOST_CpltCallback, NULL);
	RMCALL_Init(&rmcall_host, &config_host);
    RMCALL_HandleInsert(&rmcall_host, &ECG_rDPhandle);
    RMCALL_HandleInsert(&rmcall_host, &ECG_handShakehandle);
    RMCALL_CommandRecvEnable(&rmcall_host);
}

status_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize)
{
	UART_BLE_txTransfer.data     = (uint8_t *)_data;
	UART_BLE_txTransfer.dataSize = _dataSize;
	return UART_TransferSendNonBlocking(HITSIC_RMCALL_UART, &UART_BLE_handle, &UART_BLE_txTransfer);
}
status_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize)
{
	UART_BLE_rxTransfer.data     = (uint8_t *)_data;
	UART_BLE_rxTransfer.dataSize = _dataSize;
	return UART_TransferReceiveNonBlocking(HITSIC_RMCALL_UART, &UART_BLE_handle, &UART_BLE_rxTransfer, NULL);

}
void RMCALL_HOST_TxAbort(void)
{
	UART_TransferAbortSend(HITSIC_RMCALL_UART, &UART_BLE_handle);
}
void RMCALL_HOST_RxAbort(void)
{
	UART_TransferAbortReceive(HITSIC_RMCALL_UART, &UART_BLE_handle);
}

void RMCALL_HOST_CpltCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
	(void)(userData);
	if (kStatus_UART_TxIdle == status)
    {
        RMCALL_TxIsr(&rmcall_host);
    }

    if (kStatus_UART_RxIdle == status)
    {
        RMCALL_RxIsr(&rmcall_host);
    }
}

#endif // HITSIC_USE_RMCALL
