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
 * @file 	:	sys_rmcall.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2020.08.08 马奇科
 * @date    :   v1.0-beta.0 2021.06.25 马奇科
 *
 * @brief   :   远程调用组件
 * 
 */

#ifndef _SYS_RMCALL_HPP_
#define _SYS_RMCALL_HPP_
#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)
#include <sys_rmcall_port.h>

#include <m-dict.h>

 /*!
  * @addtogroup rmcall
  * @{
  */

/** @brief : 软件版本 */
#define SYS_RMCALL_VERSION (HITSIC_MAKE_VERSION(1U, 0U, 0U))


/*! @brief Error codes for the RMCALL driver. */
enum
{
    kStatus_RMCALL_TxBusy = MAKE_STATUS(kStatusGroup_RMCALL, 0), /*!< No empty frame buffer in queue to load to CSI. */
    kStatus_RMCALL_TxError  = MAKE_STATUS(kStatusGroup_RMCALL, 1), /*!< No full frame buffer in queue to read out. */
    kStatus_RMCALL_RxBusy = MAKE_STATUS(kStatusGroup_RMCALL, 2), /*!< Queue is full, no room to save new empty buffer. */
    kStatus_RMCALL_RxError = MAKE_STATUS(kStatusGroup_RMCALL, 3), /*!< New frame received and saved to queue. */
};

typedef void (*rmcall_handler_t)(void *_recvData, uint16_t _recvSize, void *_userData);

typedef struct _rmcall_handle
{
    uint16_t handleId;
    rmcall_handler_t handler;
    void *userData;
}rmcall_handle_t;

__PACKED struct _rmcall_header
{
    uint32_t magic;
    uint16_t handleId;
    uint16_t dataSize;
};

typedef struct _rmcall_header rmcall_header_t;

enum rmcall_statusFlag_t
{
    rmcall_statusFlag_txHead = 1U << 0U,
    rmcall_statusFlag_txData = 2U << 0U,
    rmcall_statusFlag_txBusy = 3U << 0U,
    rmcall_statusFlag_rxHead = 1U << 2U,
    rmcall_statusFlag_rxData = 2U << 2U,
    rmcall_statusFlag_rxBusy = 3U << 2U,

    rmcall_statusFlag_rxIdMissing = 1U << 8U,
};

typedef status_t(*rmcall_transfer_t)(void *_data, uint32_t dataSize);
typedef void(*rmcall_transferAbort_t)(void);

typedef struct 
{
    status_t(*xfer_tx)(void *_data, uint32_t dataSize);
    status_t(*xfer_rx)(void *_data, uint32_t dataSize);
    void(*xferAbort_tx)(void);
    void(*xferAbort_rx)(void);
    //rmcall_transfer_t xfer_tx, xfer_rx;
    //rmcall_transferAbort_t xferAbort_tx, xferAbort_rx;
}rmcall_teleport_t;

typedef struct _rmcall_config
{
    rmcall_teleport_t *teleport;//TODO: const
}rmcall_config_t;
    

static inline bool oor_equal_p(uint16_t k, unsigned char n) {
  return k == 65535 - n;//FIXME
}
static inline void oor_set(uint16_t *k, unsigned char n) {
  *k = 65535 - n;//FIXME
}

DICT_OA_DEF2(rmcall_isrDict, uint16_t, M_OPEXTEND(M_DEFAULT_OPLIST, OOR_EQUAL(oor_equal_p), OOR_SET(oor_set M_IPTR)) , rmcall_handle_t*, M_PTR_OPLIST)

typedef struct _rmcall
{
    uint32_t statusFlag;

    //rmcall_transfer_t xfer_tx, xfer_rx;
    //rmcall_transferAbort_t xferAbort_tx, xferAbort_rx;
    rmcall_teleport_t *teleport; //TODO: const

    rmcall_header_t txHeaderBuffer;
    void *txDataBuffer;
    rmcall_header_t rxHeaderBuffer;
    void *rxDataBuffer;
    
    rmcall_handle_t *rxHandle;

    rmcall_isrDict_t isrDict;

}rmcall_t;

/**
 * @brief : RMCALL初始化。
 *
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_Init(rmcall_t *_inst, rmcall_config_t const * const _config);

void RMCALL_DeInit(rmcall_t *_inst);

/**
 * @brief : 向RMCALL中断表末尾插入一个新的任务描述符。
 *  该函数仅做数据检查并赋值。需要互斥保护。
 *
 * @param {rmcall_t*} _inst          : 要操作的RMCALL实例。
 * @param {rmcall_handle_t*} _handle : 该RMCALL任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_HandleInsert(rmcall_t *_inst, rmcall_handle_t *_handle);

/**
 * @brief : 从RMCALL中断表中移除一个任务描述符。
 *
 * @param {rmcall_t*} _inst          : 要操作的RMCALL实例。
 * @param {rmcall_handle_t*} _handle : 该RMCALL任务的任务描述符指针。
 * @return {status_t}                : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_HandleRemove(rmcall_t *_inst, rmcall_handle_t *_handle);


/**
 * @brief : 向远端发送RMCALL命令。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @param {uint8_t} _handleId       : 要发送的命令ID。
 * @param {void*} _data             : 要发送的数据，无数据填NULL。
 * @param {uint16_t} dataSize       : 要发送的数据长度，无数据填0。
 */
status_t RMCALL_CommandSend(rmcall_t *_inst, uint16_t _handleId, void *_data, uint16_t _dataSize);

/**
 * @brief : RMCALL接收使能。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_CommandRecvEnable(rmcall_t *_inst);

/**
 * @brief : RMCALL接收失能。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_CommandRecvDisable(rmcall_t *_inst);

/**
 * @brief : RMCALL中断的处理函数。被IRQHandler调用。
 */
void RMCALL_TxIsr(rmcall_t *_inst);
void RMCALL_RxIsr(rmcall_t *_inst);


/* @} */

#endif // HITSIC_USE_RMCALL

#endif // ! _SYS_RMCALL_HPP_
