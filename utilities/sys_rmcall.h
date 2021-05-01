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
 *
 * @brief   :   远程调用组件
 * 
 * @note    :   依赖库：sys_uartmgr.hpp
 *              依赖库必须先初始化。
 */

#ifndef _SYS_RMCALL_HPP_
#define _SYS_RMCALL_HPP_
#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)
#include <sys_rmcall_port.h>

 /*!
  * @addtogroup rmcall
  * @{
  */

/** @brief : 软件版本 */
#define SYS_RMCALL_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))


class rmCall_item_t
{
public:
    enum itemFlag_t : uint8_t
    {
        flag_base = 1U << 7U;
        data_tx = 0U << 0U,
        data_rx = 1U << 0U,
        DATA_DIR = 0b11;
        xferSuccess = 0U << 2U,
        xferTimeout = 1U << 2U,
        xferFailure = 2U << 2U,
        xferNo_Item = 3U << 2U,
    };
    typedef void (*handle_t)(rmCall_item_t *_item);

    uint16_t itemId;
    handle_t handle;
    uint8_t status;

    void* dataPtr;
    size_t dataSize, ptrPos;

    rmCall_item_t(void);
    rmCall_item_t(uint32_t _itemId, void* _dataPtr, size_t _dataSize, handle_t _handle)
        : itemId(_itemId) , dataPtr(_dataPtr) , dataSize(_dataSize), handle(_handle)
    {
        if(dataPtr == nullptr && _itemId != 0)
        {
#ifdef DEBUG
            throw std::runtime_error(std::string("Invalid argument in construction of rmCall_item_t"));
#endif
        }
    }
    ~rmCall_item_t(void)
    {
    }
}

template<class rm_uart_t, class rm_uart_handle_t, class rm_uart_transfer_callback_t>
class rmCall_target_t
{
public:
    enum targetFlag_t : uint8_t
    {
        txIdle = 0b00 << 0,
        txData, = 0b01 << 0,
        txMsg_header = 0b10 << 0,
        txMsg_status = 0b11 << 0,
        rxIdle = 0b00 << 2,
        rxData, = 0b01 << 2,
        rxMsg_header = 0b10 << 2,
        rxMsg_status = 0b11 << 2,
        XFER_MASK = 0b1111;
    };

    union message_t
    {
        __PACKED struct
        {
            uint8_t magic;
            uint8_t itemId;
            uint16_t dataSize;
        }header;
        __PACKED struct
        {
            uint8_t magic;
            uint8_t itemId;
            uint8_t status;
            uint8_t custom;
        }status;
    };



    uartMgr_t &uart;
    rmCall_item_t* itemSet[256];

    message_t message;

    uint8_t status;

    uint8_t currItemId;

    rmCall_target_t(uartMgr& _uart);

    ~rmCall_target_t(void);

    status_t installItem(rmCall_item_t& _item);
    status_t uninstallItem(uint8_t _itemId);

    status_t sendItemIntr(uint8_t _itemId);

    void txStatusMachine(void);
    void rxStatusMachine(void);

    static void rmCall_uartCallBack(rm_uart_t* base, rm_uart_handle_t* handle, status_t status, void* userData);

private:
    rmCall_target_t();
};










#include <m-buffer.h>

/*! @brief Error codes for the RMCALL driver. */
enum
{
    kStatus_RMCALL_TxBusy = MAKE_STATUS(kStatusGroup_DMADVP, 0), /*!< No empty frame buffer in queue to load to CSI. */
    kStatus_RMCALL_TxError  = MAKE_STATUS(kStatusGroup_DMADVP, 1), /*!< No full frame buffer in queue to read out. */
    kStatus_RMCALL_RxBusy = MAKE_STATUS(kStatusGroup_DMADVP, 2), /*!< Queue is full, no room to save new empty buffer. */
    kStatus_RMCALL_RxError = MAKE_STATUS(kStatusGroup_DMADVP, 3), /*!< New frame received and saved to queue. */
};

typedef void (*rmcall_handler_t)(void *_recvData, void *_userData);

typedef struct _rmcall_handle
{
    uint16_t handleId;
    rmcall_handler_t handler;
    void *recvData;
    uint16_t recvDataSize, recvDataLen;
    void *userData;
}rmcall_handle_t;

__PACKED struct _rmcall_header
{
    uint8_t magic;
    uint8_t itemId;
    uint16_t dataSize;
};

typedef _rmcall_header rmcall_header_t;

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

typedef struct _rmcall
{
    uint32_t statusFlag;

    rmcall_transfer_t xfer_tx, xfer_rx;

    rmcall_header_t rxHeaderBuffer;

}rmcall_t;

/**
 * @brief : RMCALL初始化。
 *
 * @return {status_t} : 成功返回kStatus_Success，异常返回kStatus_Fail。
 */
status_t RMCALL_Init();

void RMCALL_DeInit(void);

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
status_t RMCALL_CommandSend(rmcall_t *_inst, uint8_t _handleId, void *_data, uint16_t dataSize);

/**
 * @brief : RMCALL中断的处理函数。被IRQHandler调用。
 */
void RMCALL_Isr(rmcall_t *_inst, bool _txDone, bool _rxDone, uint8_t *_rxData, uint16_t _rxDataSize);


/* @} */

#endif // HITSIC_USE_RMCALL

#endif // ! _SYS_RMCALL_HPP_
