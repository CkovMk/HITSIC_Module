/***************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018, HITSIC/哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @brief           PIT中断管理器 C语言版本
 * @file       		sys_pitmgr.h
 * @company	        HITSIC/哈工大智能车创新俱乐部
 * @author     		C.M. (QQ:905497173)
 * @version    		v3.2
 * @Software
 * @Target_core		NXP MK66F18 or other K Series MCU
 * @date            ver1.0 2018.11.25 by C.M.
 * @date            ver1.5 2019.02.07 by C.M.
 * @date            ver2.1 2019.04.12 by C.M.
 * @date            ver3.1 2019.09.28 by C.M.
 * @date            ver3.2 2019.09.29 by C.M.
 *
 * @note：
****************************************************************/

#pragma once

#ifndef SYS_UARTMGR_H
#define SYS_UARTMGR_H

#include "include.h"

#define	LPUART_Type UART_Type
#define lpuart_handle_t uart_handle_t
#define	lpuart_transfer_callback_t uart_transfer_callback_t
#define LPUART_WriteBlocking UART_WriteBlocking
#define LPUART_ReadBlocking UART_ReadBlocking
#define LPUART_TransferCreateHandle UART_TransferCreateHandle
#define lpuart_transfer_t uart_transfer_t
#define LPUART_TransferSendNonBlocking UART_TransferSendNonBlocking
#define LPUART_TransferReceiveNonBlocking UART_TransferReceiveNonBlocking
#define kStatus_LPUART_TxIdle kStatus_UART_TxIdle
#define kStatus_LPUART_RxIdle kStatus_UART_RxIdle
#define LPUART_TransferAbortSend UART_TransferAbortSend
#define LPUART_TransferAbortReceive UART_TransferAbortReceive



#define UART_TPCTRL_HEADER 0xae



typedef enum uartmgr_pptFlag
{
	txBusy = 1 << 0,
	rxBusy = 1 << 1,
	tp_enable = 1 << 2,
	tp_rxHead = 1 << 3,
}uartmgr_pptFlag_t;

typedef void (*uartmgr_tpDataHandler_t)(uint8_t* dataBuf, int32_t size);

typedef struct uartmgr
{
	//controll varible
	uint8_t pptFlag;
	bool avail;	//available, mutex...

	//raw uart transfer
	LPUART_Type* base;
	lpuart_handle_t txIntrHandle, rxIntrHandle;
	uint32_t rxIntrCnt;

	//transfer protocal
	uint8_t* tpDataBuf;
	uartmgr_tpDataHandler_t tpHandler;

}uartmgr_t;



#ifdef __cplusplus
extern "C" {
#endif

void UARTMGR_DataInit(uartmgr_t* _mgr, LPUART_Type* _base);

uartmgr_t* UARTMGR_GetInst(LPUART_Type* instNum);

status_t UARTMGR_TxPoll(uartmgr_t* _mgr, uint8_t* data, uint32_t size);
status_t UARTMGR_RxPoll(uartmgr_t* _mgr, uint8_t* data, uint32_t size);
int32_t UARTMGR_Printf(uartmgr_t* _mgr, const char* _fmt, ...);
status_t UARTMGR_TxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size, lpuart_transfer_callback_t callback, void* userData);
void UARTMGR_DefaultCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData);
void UARTMGR_TxIntrAbort(uartmgr_t* _mgr);
status_t UARTMGR_RxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size, lpuart_transfer_callback_t callback, void* userData);
void UARTMGR_RxIntrAbort(uartmgr_t* _mgr);
status_t UARTMGR_TpModeSetup(uartmgr_t* _mgr, uint8_t* dataBuf, uartmgr_tpDataHandler_t handler);
void UARTMGR_Tp_DefaultCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData);
void UARTMGR_Tp_TxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size);



#ifdef __cplusplus
}
#endif





#endif // ! SYS_UARTMGR_H