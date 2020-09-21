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
/**
 * @file 	:	sys_rmcall.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2020.08.08
 *
 * @brief   :   远程调用组件
 */

#ifndef _UARTMGR_HPP_
#define _UARTMGR_HPP_

#include "inc_stdlib.h"
#include "hitsic_common.h"

#if defined(HITSIC_USE_UARTMGR) && (HITSIC_USE_UARTMGR > 0)
#include "sys_uartmgr_port.hpp"

//CPU Selection
#if defined(D_RT1052_SYS_UARTMGR_PORT_HPP_) || defined (D_MK66F18_SYS_UARTMGR_PORT_HPP_) || defined (D_KV10Z7_SYS_UARTMGR_PORT_HPP_)

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

#endif // ! CPU Selection

#endif // ! HITSIC_USE_UARTMGR

#endif // ! _UARTMGR_HPP_
