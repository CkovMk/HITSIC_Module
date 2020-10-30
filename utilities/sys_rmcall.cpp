#include "sys_rmcall.hpp"

/*!
 * @addtogroup rmcall
 * @{
 */

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

rmCall_target_t::rmCall_target_t(uartMgr& _uart) : uart(_uart)
{
    installItem(*(new rmCall_item_t(0, nullptr, 0, nullptr)));
}

rmCall_target_t::~rmCall_target_t(void)
{
    delete[] itemSet[0];
}

status_t rmCall_target_t::installItem(rmCall_item_t& _item)
{
    if (isrSet[_item.itemId] != nullptr)
    {
        return kStatus_Fail;
    }
    itemdSet[_cmd.itemId] = &_item;
    return kStatus_Success;
}

status_t rmCall_target_t::uninstallItem(uint8_t _itemId)
{
    itemSet[_itemId] = nullptr;
    return kStatus_Success;
}

status_t rmCall_target_t::sendItemIntr(uint8_t _itemId)
{
    if ((status | XFER_MASK) != (txIdle | rxMsg_header))
    {
        return kStatus_Fail;
    }
    if (itemSet[_itemId] == NULL)
    {
        return kStatus_InvalidArgument;
    }
    currItemId = itemSet[_itemId];
    message.header.magic = HITSIC_RMCALL_HEADER_MAGIC;
    message.header.itemId = itemSet[currItemId]->itemId;
    message.header.dataSize = itemSet[currItemId]->dataSize;
    status = txMsg_header | rxIdle;
    uart.rxIntrAbort();
    uart.txIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
}

void rmCall_target_t::txStatusMachine(void)
{
    switch (status)
    {
    case (txIdle | rxMsg_header):   ///< ����״̬
        break;
    case (txMsg_header | rxIdle):   ///< ���������header������ϣ���ʼ����dataͬʱ׼������status
        if (itemSet[currItemId]->dataSize != 0)
        {//���dataSize��Ϊ0����������Data��
            status = txData | rxMsg_status;
            uart.txIntr((uint8_t*)itemSet[currItemId]->dataPtr, itemSet[currItemId]->dataSize, rmCall_uartCallBack, (void*)this);
            uart.rxIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
            break;
        }
    case (txData | rxMsg_status):   ///< data������ɣ���dataSize==0�����δ׼������status��������׼������status
        status = txIdle | rxMsg_status;
        if (status | rxIdle)
        {// �����dataSize==0������Ҫ����rx��
            uart.rxIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
        }
        break;
    case (txMsg_status | rxIdle):       ///< ������status�����ν��ս������ָ�Ĭ��״̬��׼������header
        status = txIdle | rxMsg_header;
        uart.rxIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
        break;
    }
}

void rmCall_target_t::rxStatusMachine(void)
{
    switch (status)
    {
    case (txIdle | rxMsg_header):   ///< ���յ�Header
        if (message.header.magic == HITSIC_RMCALL_HEADER_MAGIC)
        {
            currItemId = message.header.itemId;
            if (itemSet[currItemId] == nullptr)
            {
                currItemId = 0;
                itemSet[0].dataSize = message.header.dataSize;
                itemSet[0].dataPtr = (void*)new uint8_t[itemSet[0].dataSize];
            }
        }
        if (message.header.dataSize != 0)    // �����data������Data
        {
            status = txIdle | rxData;
            uart.rxIntr((uint8_t*)currentItem->dataPtr, currentItem->dataSize, rmCall_uartCallBack, (void*)this);
            break;
        }
    case (txIdle | rxData):     ///< ������data����dataSize==0������status�����ʹ����dummyBuffer��0�ۣ����ͷ��ڴ档���δʹ��dummyBuffer������handle��
        status = txMsg_status | rxIdle;
        message.status.magic = HITSIC_RMCALL_STATUS_MAGIC;
        if (currItemId == 0)
        {
            message.status.status = rmCall_item_t::flag_base | rmCall_item_t::data_rx | rmCall_item_t::xferNo_Item;
            delete[] itemSet[0]->dataPtr;
            itemSet[0]->dataSize = 0;
        }
        else
        {
            message.status.status = rmCall_item_t::flag_base | rmCall_item_t::data_rx | rmCall_item_t::xferSuccess;
        }
        uart.txIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
        if (itemSet[currItemId]->handle != nullptr)
        {
            (*itemSet[currItemId]->handle)(itemSet[currItemId]);
        }
        break;
    case (txData | rxMsg_status):       ///< ����dataʱ���յ���status��������error�����error��ȡ�����͡�
        if (message.status.itemId != currentItem.itemId || message.status.status != (rmCall_item_t::flag_base | rmCall_target_t::data_rx | rmCall_target_t::xferSuccess))
        {
            uart.txIntrAbort();
            itemSet[currItemId]->status = message.status.status;
            assert(0);
        }
    case (txIdle | rxMsg_status):       ///< ������Ͻ��յ�status���ָ�Ĭ��״̬������status
        status = txIdle | rxMsg_header;
        uart.rxIntr((uint8_t*)message, sizeof(message_t), rmCall_uartCallBack, (void*)this);
        if (message.status.itemId != currentItem.itemId || message.status.status != (rmCall_item_t::flag_base | rmCall_target_t::data_rx | rmCall_target_t::xferSuccess))
        {
            itemSet[currItemId]->status = message.status.status;
            assert(0);
        }
        break;
    }
}

void rmCall_target_t::rmCall_uartCallBack(UART_Type* base, uart_handle_t* handle, status_t status, void* userData)
{
    rmCall_target_t& target = *((rmCall_target_t*)userData);
    switch (status)
    {
    case kStatus_UART_TxIdle:
        target.uart.pptFlag &= (~pitMgr_t::txBusy);
        target.txStatusMachine();
        break;
    case kStatus_UART_RxIdle:
        target.uart.pptFlag &= (~pitMgr_t::rxBusy);
        target.rxStatusMachine();
        break;
    default:
#ifdef DEBUG
        throw std::runtime_error(std::string("uart tx/rx interrupt xfer error."));
#endif
        break;
    }
}


/* @} */

#endif // HITSIC_USE_RMCALL