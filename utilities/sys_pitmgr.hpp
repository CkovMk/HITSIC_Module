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
 * @file 	:	sys_pitmgr.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v1.0.0
 *
 * @date 	:	v0.1-beta.0 2019.10.28
 * @date 	:	v0.1.1		2019.11.02
 * @date	:	v1.0.0		2020.07.25
 *
 * @brief   :   定时中断管理器
 */

#pragma once
#ifndef UTILITIES_SYS_PITMGR_HPP_
#define UTILITIES_SYS_PITMGR_HPP_
#include "inc_stdlib.h"
#include "hitsic_common.h"

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)
#include "sys_pitmgr_port.hpp"

/*!
 * @addtogroup pitmgr
 * @{
 */

//CPU Selection
#if defined(D_RT1052_SYS_PITMGR_PORT_HPP_) || defined (D_MK66F18_SYS_PITMGR_PORT_HPP_) || defined (D_KV10Z7_SYS_PITMGR_PORT_HPP_)


#include "list"

class pitMgr_t
{
public:

	//isr service manager
	typedef void (*handler_t)(void *userData);
	enum pptFlag_t : uint32_t
	{
		enable = 1 << 0, runOnce = 1 << 1,
	//msgWake = 1 << 2,
	};

	static std::list<pitMgr_t> isrSet;



	static uint32_t timer_ms;

	static uint32_t getTimer_ms(void)
	{
		return timer_ms;
	}
	static void delay_ms(uint32_t _t)
	{
		uint32_t cnt = timer_ms + _t;
		while(timer_ms < cnt);
	}


	static status_t init(void);
	static pitMgr_t* insert(uint32_t _ms, uint32_t _mso, handler_t _handler,
			uint32_t _ppt);
	static status_t remove(pitMgr_t &_handle);
	static void isr(void);

	//isr service content
	uint32_t ms, mso;
	handler_t handler;
	uint32_t pptFlag;
	uint64_t prevTime_ms;
	void *userData;

	

	void setup(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);
	void setEnable(bool _b)
	{
		if (_b)
		{
			pptFlag |= enable;
		}
		else
		{
			pptFlag &= (~enable);
		}
	}
	
	void setUserData(void *_userData)
	{
		userData = _userData;
	}

	pitMgr_t(void);

private:

	pitMgr_t(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt)
	{
		setup(_ms, _mso, _handler, _ppt);
	}

};

// C wrapper
#define PITMGR_Init()			pitMgr_t::init()
//#define PITMGR_GetLTC()			pitMgr_t::getLTC()
//#define PITMGR_GetLTC_us()		pitMgr_t::getLTC_us()
#define PITMGR_GetTimer_ms()		pitMgr_t::getTimer_ms()
//#define PITMGR_Delay(_t)		pitMgr_t::delay(_t)
//#define PITMGR_Delay_us(_t)		pitMgr_t::delay_us(_t)
#define PITMGR_Delay_ms(_t)		pitMgr_t::delay_ms(_t)

#define pitmgr_handle_t pitMgr_t
#define pitmgr_pptEnable		pitMgr_t::enable
#define pitmgr_pptRunOnce		pitMgr_t::runOnce

#define PITMGR_HandleSetup(_h, _ms, _mso, _handler, _ppt)	_h->setup(_ms, _mso, _handler, _ppt)
#define PITMGR_HandleInsert(_ms, _mso, _handler, _ppt) 		pitMgr_t::insert(_ms, _mso, _handler, _ppt)
#define PITMGR_HandleSetEnable(_h, b)		_h->setEnable(b)
#define PITMGR_HandleRemove(_h)				pitMgr_t::remove(_h)
#define PITMGR_Isr()						pitMgr_t::isr()

#else	//CPU Selection

#error "C++ API does NOT support this CPU!"

#endif // CPU Selection

/* @} */

#endif // ! HITSIC_USE_PITMGR

#endif // ! UTILITIES_SYS_PITMGR_HPP_
