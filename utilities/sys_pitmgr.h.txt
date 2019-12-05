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
 * @note：本模块使用了PIT Chnl 0,1,2。使用时须注意设置中断优先级，并
		避免在同一个中断周期内运行多个任务导致超时。超时将造成控制时序
		紊乱，进而干扰控制的正常运行。
		本模块可以实现亚us级精确计时、可重入延时、ms级定时中断管理。精
		确计时可用于测量函数运行时间。延时函数可嵌套使用，不会互相冲突。
		但仍应该避免在中断服务函数中使用延时。
****************************************************************/

#pragma once

#ifndef UTILITIES_SYS_PITMGR_H_
#define UTILITIES_SYS_PITMGR_H_

#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"
#include "sys_pitmgr_port.hpp"

#include "lib_list.h"

#if defined(D_MK66F18_SYS_PITMGR_PORT_HPP_) //CPU Selection

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief : PIT管理器中断服务函数。
 */
typedef void (*pitmgr_handler_t)(void);

/**
 * @brief : PIT管理器属性设置，按标志位枚举。
 */
typedef enum pitmgr_pptFlag// : uint32_t
{
	pitmgr_pptEnable = 1 << 0,
	pitmgr_pptRunOnce = 1 << 1,
}pitmgr_pptFlag_t;

/**
 * @brief : PIT管理器句柄，用于存储每个PIT中断的数据。
 */
typedef struct pitmgr_handle
{
	uint32_t ms, mso;			/* 此中断在每ms毫秒的第_mso毫秒运行。 */
	pitmgr_handler_t handler;	/* 指向中断服务函数的指针。 */
	uint32_t pptFlag;			/* 属性标志位，是pitmgr_pptFlag_t的按位组合。 */
	uint64_t prevTime_us;		/* 本中断上次运行耗时。 */
}pitmgr_handle_t;

/**
 * @brief : 中断服务函数的计时变量，单位为ms
 */
extern uint32_t pitmgr_timer_ms;

/**
 * PIT中断表，以单向链表存储。
 */
extern list_t pitmgr_isrSet;

/**
 * @brief : PIT管理器初始化。
 *	初始化变量、PIT外设、PIT中断。
 *
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t PITMGR_Init(void);

/**
 * @brief : 获取LifeTimeCounter当前计时数。
 *
 *	PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的时钟数
 */
uint64_t PITMGR_GetLTC(void);

/**
 * @brief : 获取LifeTimeCounter当前计时微秒数。
 *
 *	PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的微秒数
 */
uint64_t PITMGR_GetLTC_us(void);

/**
 * @brief : 获取LifeTimeCounter当前计时毫秒数。
 *
 *	PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的毫秒数
 */
uint64_t PITMGR_GetLTC_ms(void);

/**
 * @brief : 利用LifeTimeCounter进行时钟级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的时钟数
 */
void PITMGR_Delay(uint64_t _t);

/**
 * @brief : 利用LifeTimeCounter进行微秒级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的微秒数
 */
void PITMGR_Delay_us(uint64_t _t);

/**
 * @brief : 利用LifeTimeCounter进行毫秒级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的毫秒数
 */
void PITMGR_Delay_ms(uint64_t _t);

/**
 * @brief : 初始化一个PIT定时中断句柄。
	该函数仅做数据检查并赋值。需要互斥保护。

 * @param {pitmgr_handle_t*} _h       : 要设置的PIT中断句柄
 * @param {uint32_t} _ms              : 该PIT中断的中断间隔。
 * @param {uint32_t} _mso             : 该PIT中断的中断相位。用于手动负载均衡。
 * @param {pitmgr_handler_t} _handler : 该PIT中断的中断服务函数。
 * @param {uint32_t} _ppt             : 该PIT中断的属性Flag。
 */
void PITMGR_HandleSetup(pitmgr_handle_t* _h, uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

/**
 * @brief : 向PIT中断表末尾插入一个新的中断句柄。
 *	该函数仅做数据检查并赋值。需要互斥保护。
 * 
 * @param {uint32_t} _ms              : 该PIT中断的中断间隔。
 * @param {uint32_t} _mso             : 该PIT中断的中断相位。用于手动负载均衡。
 * @param {pitmgr_handler_t} _handler : 该PIT中断的中断服务函数。
 * @param {uint32_t} _ppt             : 该PIT中断的属性Flag。
 * @return {pitmgr_handle_t*}         : 返回指向新插入的PIT中断句柄的指针。如果分配失败则返回空指针。用户须自行检查。
 */
pitmgr_handle_t* PITMGR_HandleInsert(uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

/**
 * @brief : 设置某个PIT中断句柄的属性。
 * 
 * @param {pitmgr_handle_t*} _h : 要设置的PIT中断句柄
 * @param {bool} b              : 是否启用该中断句柄
 */
void PITMGR_HandleSetEnable(pitmgr_handle_t* _h, bool b);
status_t PITMGR_HandleRemove(pitmgr_handle_t* _h);


/**
 * @brief : PIT中断的处理函数。被IRQHandler调用。
 */
void PITMGR_Isr(void);

#ifdef __cplusplus
}
#endif

#else	// CPU Selection
#error "C API does NOT support this CPU!"
#endif 	// CPU Selection



#endif // ! UTILITIES_SYS_PITMGR_H_
