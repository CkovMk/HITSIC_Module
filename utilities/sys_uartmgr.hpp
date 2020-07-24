#ifndef _UARTMGR_HPP_
#define _UARTMGR_HPP_

#include "sys_uartmgr_port.hpp"
#include "inc_stdlib.h"
#include "inc_gnc.h"



class uartMgr_t
{
public:
	static uartMgr_t& getInst(UART_Type* instNum)
	{
		UARTMGR_INST_DEFINITION

		switch ((uint32_t)instNum)
		{

		UARTMGR_INST_SWITCHCASE

		default:
#ifdef DEBUG
			throw std::invalid_argument("Unspecified UART Port!");
#else

#endif // ! DEBUG
			break;
		}
	}
	enum pptFlag_t : uint8_t
	{
		txBusy = 1 << 0,
		rxBusy = 1 << 1,
		tp_enable = 1 << 2,
		tp_rxHead = 1 << 3,
	};

	//controll varible
	uint8_t pptFlag;
	bool avail;	//available, mutex...

	//raw uart transfer
	UART_Type* base;
	uart_handle_t txIntrHandle, rxIntrHandle;
	uint32_t rxIntrCnt;

	//transfer protocal
	uint8_t* tpDataBuf;
	typedef void (*tpDataHandler_t)(uint8_t* dataBuf, int32_t size);
	tpDataHandler_t tpHandler;



	status_t txPoll(uint8_t* data, uint32_t size);
	status_t rxPoll(uint8_t* data, uint32_t size);
	int32_t uprintf(const char* _fmt, ...)
	{
		va_list args;
		va_start(args, _fmt);
		static char strBuf[256];
		int32_t len = vsnprintf(strBuf, 256, _fmt, args);
		if(len > 0)
		{
			txPoll((uint8_t*)strBuf, len);
		}
		return len;
	}
	status_t txIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData);
	static void defaultCallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData);
	void txIntrAbort(void);
	status_t rxIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData);
	void rxIntrAbort(void);
	status_t tpModeSetup(uint8_t* dataBuf, tpDataHandler_t handler);
	static void tp_defaultcallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData);
	void tp_txIntr(uint8_t* data, uint32_t size);




private:

	uartMgr_t(UART_Type* _base)
	{
		base = _base;
		pptFlag = 0;
	}
};


#endif // ! _UARTMGR_HPP_
