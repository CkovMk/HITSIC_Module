/**
 * Copyright 2018 - 2020 HITSIC
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
#include "sys_rmcall_port.hpp"

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

/* @} */

#endif // HITSIC_USE_RMCALL

#endif // ! _SYS_RMCALL_HPP_
