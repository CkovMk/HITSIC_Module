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
 * @version :	v1.0.0
 *
 * @date 	:	v0.2-beta.0 	2019.10.20
 * @date 	:	v0.2.1			2019.11.05
 * @date	:	v1.0-beta.0		2020.07.25
 * @date    :   v1.0.1          2020.10.30
 * @date    :   v2.0.0          2021.03.30
 *
 * @brief    :   外部中断管理器
 */

#pragma once
#ifndef UTILITIES_SYS_EXTINT_HPP
#define UTILITIES_SYS_EXTINT_HPP
#include "hitsic_common.h"

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)
#include <sys_extint_port.h>

#include <m-list.h>

/*!
 * @addtogroup extint
 * @{
 */

/** @brief : 软件版本 */
#define SYS_EXTINT_VERSION (HITSIC_MAKE_VERSION(2U, 0U, 0U))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : PIT管理器中断服务函数。
 */
typedef void (*extint_handler_t)(void *userData);

typedef struct _extint_handle
{
    uint32_t index;
    extint_handler_t handler;   /*< 指向中断服务函数的指针 */
    void *userData;  /*< 用户变量 */
}extint_handle_t;

LIST_DEF(extint_isrList, extint_handle_t* ,M_PTR_OPLIST)

typedef struct _extint
{

    extint_isrList_t isrList;
}extint_t;

/**
 * @brief : 查找EXTINT实例。
 * 该函数在extint_port中实现。
 *
 * @param  {INTC_Type*} : 用于管理 外部中断的外设地址。
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
extint_t *EXTINT_GetInst(INTC_Type *base);

/**
 * @brief : EXTINT初始化。
 *
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t EXTINT_Init(extint_t *_inst);

void EXTINT_Deinit(extint_t *_inst);

/**
 * @brief : EXTINT中断的处理函数。被IRQHandler调用。
 */
void EXTINT_Isr(extint_t *_inst, uint32_t flag);

/**
 * @brief : 向EXTINT中断表末尾插入一个新的任务描述符。
 *  该函数仅做数据检查并赋值。需要互斥保护。
 *
 * @param {extint_t*} _inst          : 要操作的EXTINT实例。
 * @param {extint_handle_t*} _handle : 该EXTINT任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t EXTINT_HandleInsert(extint_t *_inst, extint_handle_t *_handle);

/**
 * @brief : 从EXTINT中断表中移除一个任务描述符。
 *
 * @param {extint_t*} _inst          : 要操作的EXTINT实例。
 * @param {extint_handle_t*} _handle : 该EXTINT任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t EXTINT_HandleRemove(extint_t *_inst, extint_handle_t *_handle);

#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_EXTINT

#endif // ! UTILITIES_SYS_EXTINT_HPP


