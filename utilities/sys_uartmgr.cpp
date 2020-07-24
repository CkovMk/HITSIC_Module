#include <sys_uartmgr.hpp>

status_t uartMgr_t::txPoll(uint8_t* data, uint32_t size)
{
	if (pptFlag & txBusy) { return kStatus_Fail; }
	pptFlag |= txBusy;
	UART_WriteBlocking(base, data, size);
	pptFlag &= (~txBusy);
	return kStatus_Success;
}

status_t uartMgr_t::rxPoll(uint8_t* data, uint32_t size)
{
	if (pptFlag & rxBusy) { return kStatus_Fail; }
	pptFlag |= rxBusy;
	UART_ReadBlocking(base, data, size);
	pptFlag &= (~rxBusy);
	return kStatus_Success;
}

status_t uartMgr_t::txIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData)
{
	if (pptFlag & txBusy) { return kStatus_Fail; }
	pptFlag |= txBusy;
	UART_TransferCreateHandle(base, &txIntrHandle, callback, userData);
	static uart_transfer_t xfer;
	xfer.data = data;
	xfer.dataSize = size;
	return UART_TransferSendNonBlocking(base, &txIntrHandle, &xfer);
}

void uartMgr_t::defaultCallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData)
{

	switch (status)
	{
	case kStatus_UART_TxIdle:
		getInst(base).pptFlag &= (~txBusy);
		break;
	case kStatus_UART_RxIdle:
		getInst(base).pptFlag &= (~rxBusy);
		break;
	default:
#ifdef DEBUG
		throw std::runtime_error(std::string("uart tx/rx interrupt xfer error."));
#endif
		break;
	}
}

void uartMgr_t::txIntrAbort(void)
{
	if (pptFlag & txBusy)
	{
		UART_TransferAbortSend(base, &txIntrHandle);
		pptFlag &= (~txBusy);
	}
}

status_t uartMgr_t::rxIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData)
{
	if (pptFlag & rxBusy) { return kStatus_Fail; }
	pptFlag |= rxBusy;
	UART_TransferCreateHandle(base, &rxIntrHandle, callback, userData);
	static uart_transfer_t xfer;
	xfer.data = data;
	xfer.dataSize = size;
	return UART_TransferReceiveNonBlocking(base, &rxIntrHandle, &xfer, (size_t*)& rxIntrCnt);
}

void uartMgr_t::rxIntrAbort(void)
{
	if (pptFlag & rxBusy)
	{
		UART_TransferAbortReceive(base, &rxIntrHandle);
		pptFlag &= (~rxBusy);
	}
}

status_t uartMgr_t::tpModeSetup(uint8_t* dataBuf, tpDataHandler_t handler)
{
	if (pptFlag & rxBusy || pptFlag & txBusy) { return kStatus_Fail; }
#ifdef DEBUG
	if (dataBuf == NULL || handler == NULL)
	{
		throw std::invalid_argument(std::string("tpModeSetup pointer invalid."));
	}
#endif
	pptFlag |= tp_enable;
	tpDataBuf = dataBuf;
	tpHandler = handler;

	pptFlag |= tp_rxHead;
	rxIntr(dataBuf, 2, tp_defaultcallback, NULL);
	return kStatus_Success;
}

void uartMgr_t::tp_defaultcallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData)
{
	uartMgr_t& inst = getInst(base);



	switch (status)
		{
		case kStatus_UART_TxIdle:
			getInst(base).pptFlag &= (~txBusy);
			break;
		case kStatus_UART_RxIdle:
			getInst(base).pptFlag &= (~rxBusy);
			if (inst.pptFlag & tp_rxHead && inst.tpDataBuf[0] == UART_TPCTRL_HEADER)
			{
				inst.pptFlag &= (~tp_rxHead);
				inst.rxIntr(inst.tpDataBuf, inst.tpDataBuf[1], tp_defaultcallback, NULL);
			}
			else
			{
				(*inst.tpHandler)(inst.tpDataBuf, inst.rxIntrCnt);
				inst.pptFlag |= tp_rxHead;
				inst.rxIntr(inst.tpDataBuf, 2, tp_defaultcallback, NULL);
			}
			break;
		default:
	#ifdef DEBUG
			throw std::runtime_error(std::string("uart tp tx/rx interrupt xfer error."));
	#endif
			break;
		}
	inst.pptFlag &= (~rxBusy);
	if (status != kStatus_Success)
	{
#ifdef DEBUG
		throw std::runtime_error(std::string("tp uart rx interrupt xfer error."));
#endif
		return;
	}

}

void uartMgr_t::tp_txIntr(uint8_t* data, uint32_t size)
{
	if (!(pptFlag & tp_enable))
	{
#ifdef DEBUG
		throw std::runtime_error(std::string("illegal use of transfer-protocal."));
#endif
		return;

		static uint8_t dataBuf[255];
		if (size > 255)
		{
#ifdef DEBUG
			throw std::invalid_argument(std::string("tp tx dataSize out of range."));
#endif
			return;
		}
		dataBuf[0] = UART_TPCTRL_HEADER;
		dataBuf[1] = size;
		memcpy(dataBuf + 2, data, size);
		txIntr(dataBuf, size + 2, defaultCallback, NULL);
	}

}
