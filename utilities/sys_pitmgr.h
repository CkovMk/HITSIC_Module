/**
 * Copyright 2018 - 2021 HITSIC
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
 * @version :	v2.0.0
 *
 * @date 	:	v0.4-beta.0 2018.12.20
 * @date 	:	v0.4-beta.0 2019.10.28
 * @date 	:	v0.4.1		2019.11.02
 * @date	:	v1.0.0		2020.07.25
 * @date    :   v1.0.1      2020.10.30
 * @date    :   v2.0.0      2021.03.27
 *
 * @brief   :   定时中断管理器
 */

#pragma once
#ifndef UTILITIES_SYS_PITMGR_HPP
#define UTILITIES_SYS_PITMGR_HPP
#include "hitsic_common.h"

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)
#include <sys_pitmgr_port.h>
#include <m-list.h>

/*!
 * @addtogroup pitmgr
 * @{
 */

/** @brief : 软件版本 */
#define SYS_PITMGR_VERSION (HITSIC_MAKE_VERSION(2U, 0U, 0U))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : PIT管理器属性设置，按标志位枚举。
 */
typedef enum pitmgr_pptFlag// : uint32_t
{
    pitmgr_pptEnable = 1 << 0,
    pitmgr_pptRunOnce = 1 << 1,
}pitmgr_pptFlag_t;

/**
 * @brief : PIT管理器中断服务函数。
 */
typedef void (*pitmgr_handler_t)(void *userData);

/**
 * @brief : PIT管理器句柄，用于存储每个PIT中断的数据。
 */
typedef struct pitmgr_handle
{
    uint32_t tickInterval, tickOffset;           /*< 此中断在每ms毫秒的第_mso毫秒运行 */
    pitmgr_handler_t handler;   /*< 指向中断服务函数的指针 */
    uint32_t pptFlag;           /*< 属性标志位，是pitmgr_pptFlag_t的按位组合 */
    void *userData;  /*< 用户变量 */
}pitmgr_handle_t;

LIST_DEF(pitmgr_isrList, pitmgr_handle_t* ,M_PTR_OPLIST)

typedef struct _pitmgr
{
    uint64_t tickCounter; ///< 中断服务函数的计时变量
    uint32_t tickPeriod_us; ///< 中短周期（微秒）
    pitmgr_isrList_t isrList; ///< 定时任务列表
}pitmgr_t;

/**
 * @brief : PITMGR初始化。
 *  初始化变量、PIT外设、PIT中断。
 *
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t PITMGR_Init(pitmgr_t *_inst, uint32_t _period_us);

void PITMGR_Deinit(pitmgr_t *_inst);

/**
 * @brief : PITMGR中断的处理函数。被IRQHandler调用。
 */
void PITMGR_Isr(pitmgr_t *_inst);

/**
 * @brief : 向PITMGR中断表末尾插入一个新的任务描述符。
 *  该函数仅做数据检查并赋值。需要互斥保护。
 *
 * @param {pitmgr_t*} _inst          : 要操作的PITMGR实例。
 * @param {pitmgr_handle_t*} _handle : 该PITMGR任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t PITMGR_HandleInsert(pitmgr_t *_inst, pitmgr_handle_t *_handle);

/**
 * @brief : 从PITMGR中断表中移除一个任务描述符。
 *
 * @param {pitmgr_t*} _inst          : 要操作的PITMGR实例。
 * @param {pitmgr_handle_t*} _handle : 该PITMGR任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t PITMGR_HandleRemove(pitmgr_t *_inst, pitmgr_handle_t *_handle);


#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_PITMGR

#endif // ! UTILITIES_SYS_PITMGR_HPP
