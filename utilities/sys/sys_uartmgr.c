/**
 * Copyright 2018 - 2019 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sys_uartmgr.h"

#ifdef __cplusplus
extern "C" {
#endif

void UARTMGR_DataInit(uartmgr_t* _mgr, LPUART_Type* _base)
{
	_mgr->base = _base;
	_mgr->pptFlag = 0;
}

	uartmgr_t* UARTMGR_GetInst(LPUART_Type* instNum)
{
	static bool isInit = false;
	static uartmgr_t dbugInst;	//debug
	static uartmgr_t camrInst;	//camera
	if (!isInit)
	{
		UARTMGR_DataInit(&dbugInst, UART0);
		UARTMGR_DataInit(&camrInst, UART1);
		isInit = true;
	}
	switch ((uint32_t)instNum)
	{
	case UART0_BASE:
		return &dbugInst;
		break;
	case UART1_BASE:
		return &camrInst;
		break;
        default:
		return &dbugInst;
		break;
	}
	return &dbugInst;
}

status_t UARTMGR_TxPoll(uartmgr_t* _mgr, uint8_t* data, uint32_t size)
{
	if (_mgr->pptFlag & txBusy) { return kStatus_Fail; }
	_mgr->pptFlag |= txBusy;
	LPUART_WriteBlocking(_mgr->base, data, size);
	_mgr->pptFlag &= (~txBusy);
	return kStatus_Success;
}

status_t UARTMGR_RxPoll(uartmgr_t* _mgr, uint8_t* data, uint32_t size)
{
	if (_mgr->pptFlag & rxBusy) { return kStatus_Fail; }
	_mgr->pptFlag |= rxBusy;
	LPUART_ReadBlocking(_mgr->base, data, size);
	_mgr->pptFlag &= (~rxBusy);
	return kStatus_Success;
}

int32_t UARTMGR_Printf(uartmgr_t* _mgr, const char* _fmt, ...)
{
	va_list args;
	va_start(args, _fmt);
	static char strBuf[256];
	int32_t len = vsnprintf(strBuf, 256, _fmt, args);
	if (len > 0)
	{
		UARTMGR_TxPoll(_mgr, (uint8_t*)strBuf, len);
	}
	return len;
}

status_t UARTMGR_TxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size, lpuart_transfer_callback_t callback, void* userData)
{
	if (_mgr->pptFlag & txBusy) { return kStatus_Fail; }
	_mgr->pptFlag |= txBusy;
	LPUART_TransferCreateHandle(_mgr->base, &_mgr->txIntrHandle, callback, userData);
	static lpuart_transfer_t xfer;
	xfer.data = data;
	xfer.dataSize = size;
	return LPUART_TransferSendNonBlocking(_mgr->base, &_mgr->txIntrHandle, &xfer);
}

void UARTMGR_DefaultCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData)
{

	switch (status)
	{
	case kStatus_LPUART_TxIdle:
		UARTMGR_GetInst(base)->pptFlag &= (~txBusy);
		break;
	case kStatus_LPUART_RxIdle:
		UARTMGR_GetInst(base)->pptFlag &= (~rxBusy);
		break;
	default:
#ifdef DEBUG
		assert(0);
		//throw std::runtime_error(std::string("uart tx/rx interrupt xfer error."));
#endif
		break;
	}
}

void UARTMGR_TxIntrAbort(uartmgr_t* _mgr)
{
	if (_mgr->pptFlag & txBusy)
	{
		LPUART_TransferAbortSend(_mgr->base, &_mgr->txIntrHandle);
		_mgr->pptFlag &= (~txBusy);
	}
}

status_t UARTMGR_RxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size, lpuart_transfer_callback_t callback, void* userData)
{
	if (_mgr->pptFlag & rxBusy) { return kStatus_Fail; }
	_mgr->pptFlag |= rxBusy;
	LPUART_TransferCreateHandle(_mgr->base, &_mgr->rxIntrHandle, callback, userData);
	static lpuart_transfer_t xfer;
	xfer.data = data;
	xfer.dataSize = size;
	return LPUART_TransferReceiveNonBlocking(_mgr->base, &_mgr->rxIntrHandle, &xfer, (size_t*)& _mgr->rxIntrCnt);
}

void UARTMGR_RxIntrAbort(uartmgr_t* _mgr)
{
	if (_mgr->pptFlag & rxBusy)
	{
		LPUART_TransferAbortReceive(_mgr->base, &_mgr->rxIntrHandle);
		_mgr->pptFlag &= (~rxBusy);
	}
}

status_t UARTMGR_TpModeSetup(uartmgr_t* _mgr, uint8_t* dataBuf, uartmgr_tpDataHandler_t handler)
{
	if (_mgr->pptFlag & rxBusy || _mgr->pptFlag & txBusy) { return kStatus_Fail; }
#ifdef DEBUG
	if (dataBuf == NULL || handler == NULL)
	{
		assert(0);
		//throw std::invalid_argument(std::string("tpModeSetup pointer invalid."));
	}
#endif
	_mgr->pptFlag |= tp_enable;
	_mgr->tpDataBuf = dataBuf;
	_mgr->tpHandler = handler;

	_mgr->pptFlag |= tp_rxHead;
	UARTMGR_RxIntr(_mgr, dataBuf, 2, UARTMGR_Tp_DefaultCallback, NULL);
        return kStatus_Success;
}

void UARTMGR_Tp_DefaultCallback(LPUART_Type* base, lpuart_handle_t* handle, status_t status, void* userData)
{
	uartmgr_t* inst = UARTMGR_GetInst(base);



	switch (status)
	{
	case kStatus_LPUART_TxIdle:
		inst->pptFlag &= (~txBusy);
		break;
	case kStatus_LPUART_RxIdle:
		inst->pptFlag &= (~rxBusy);
		if (inst->pptFlag & tp_rxHead && inst->tpDataBuf[0] == UART_TPCTRL_HEADER)
		{
			inst->pptFlag &= (~tp_rxHead);
			UARTMGR_RxIntr(inst, inst->tpDataBuf, inst->tpDataBuf[1], UARTMGR_Tp_DefaultCallback, NULL);
		}
		else
		{
			(*inst->tpHandler)(inst->tpDataBuf, inst->rxIntrCnt);
			inst->pptFlag |= tp_rxHead;
			UARTMGR_RxIntr(inst, inst->tpDataBuf, 2, UARTMGR_Tp_DefaultCallback, NULL);
		}
		break;
	default:
#ifdef DEBUG
		assert(0);
		//throw std::runtime_error(std::string("uart tp tx/rx interrupt xfer error."));
#endif
		break;
	}
	inst->pptFlag &= (~rxBusy);
	if (status != kStatus_Success)
	{
#ifdef DEBUG
		assert(0);
		//throw std::runtime_error(std::string("tp uart rx interrupt xfer error."));
#endif
		return;
	}

}

void UARTMGR_Tp_TxIntr(uartmgr_t* _mgr, uint8_t* data, uint32_t size)
{
	if (!(_mgr->pptFlag & tp_enable))
	{
#ifdef DEBUG
		assert(0);
		//throw std::runtime_error(std::string("illegal use of transfer-protocal."));
#endif
		return;
	}
	static uint8_t dataBuf[255];
	if (size > 255)
	{
#ifdef DEBUG
		assert(0);
		//throw std::invalid_argument(std::string("tp tx dataSize out of range."));
#endif
		return;
	}
	dataBuf[0] = UART_TPCTRL_HEADER;
	dataBuf[1] = size;
	memcpy(dataBuf + 2, data, size);
	UARTMGR_TxIntr(_mgr, dataBuf, size + 2, UARTMGR_DefaultCallback, NULL);
	

}

#ifdef __cplusplus
}
#endif