#include <sys_rmcall.h>

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





/** Private Functions */



void RMCALL_TxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_txBusy)
    {
    case 0: // no tx
        break;
    case rmcall_statusFlag_txHead:
        // tx data here.
        _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_txHead)) | rmcall_statusFlag_txData;
        _inst->xfer_tx(txDataBuffer, _inst->txHeaderBuffer.dataSize);
        break;
    case rmcall_statusFlag_txData:
        // tx finished. go idle.
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txData);
        break;
    default:
        assert(0); // should never end up here.
        break;
    }
}

void RMCALL_RxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_rxBusy)
    {
    case 0: // no rx
        break;
    case rmcall_statusFlag_rxHead:
        // rx data here.
        _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxHead)) | rmcall_statusFlag_rxData;
        break;
    case rmcall_statusFlag_rxData:
        // rx finished. go idle.
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxData);
        break;
    default:
        assert(0); // should never end up here.
        break;
    }
}





/** Public Functions */

status_t RMCALL_Init(rmcall_t *_inst, rmcall_config_t *_config)
{
    assert(_config->xfer->tx);
    assert(_config->xfer->rx);

    _inst->xfer_tx = _config->xfer_tx;
    _inst->xfer_rx = _config->xfer_rx;
    _inst->xferAbort_tx = _config->xferAbort_tx;
    _inst->xferAbort_rx = _config->xferAbort_rx;

    _inst->statusFlag = 0U;

    rmcall_isrDict_init(&_inst->isrDict);
}

void RMCALL_DeInit(rmcall_t *_inst)
{
    _inst->xfer_tx = NULL;
    _inst->xfer_rx = NULL;

    _inst->statusFlag = 0U;

    rmcall_isrDict_clear(&_inst->isrDict);
}

status_t RMCALL_HandleInsert(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    rmcall_isrDict_set_at(&_inst->isrDict, _handle->handleId, _handle);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_HandleRemove(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    rmcall_isrDict_erase(&_inst->isrDict, _handle->handleId);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_CommandSend(rmcall_t *_inst, uint8_t _handleId, void *_data, uint16_t _dataSize)
{
    assert(_inst);

    status_t ret = 0U;

    if(0U != _inst->statusFlag & rmcall_statusFlag_txBusy)
    {
        return kStatus_RMCALL_TxBusy;
    }

    _inst->txHeaderBuffer.magic = HITSIC_RMCALL_HEADER_MAGIC;
    _inst->txHeaderBuffer.handleId = _handleId;
    _inst->txHeaderBuffer.dataSize = _dataSize;
    _inst->txDataBuffer = _data;

    _inst->statusFlag |= rmcall_statusFlag_txHead;
    ret = _inst->xfer_tx(&_inst->txHeaderBuffer, sizeof(rmcall_header_t));

    retrun ret;
}

status_t RMCALL_CommandRecvEnable(rmcall_t *_inst)
{
    assert(_inst);

    if(0U == _inst->statusFlag & rmcall_statusFlag_rxBusy)
    {
        _inst->statusFlag |= rmcall_statusFlag_rxHead;
        return _inst->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
    }

    return kStatus_Fail;
}

status_t RMCALL_CommandRecvDisable(rmcall_t *_inst)
{
    assert(_inst);

    if(0U != _inst->statusFlag & rmcall_statusFlag_rxBusy)
    {
        _inst->xferAbort_rx();
        _inst->statusFlag &= (~rmcall_statusFlag_rxBusy);
        return kStatus_Success;
    }

    return kStatus_Fail;
}

void RMCALL_Isr(rmcall_t *_inst, bool _txDone, bool _rxDone)
{
    if(_txDone)
    {
        RMCALL_TxStatusMachine(_inst);
    }

    if(_rxDone)
    {
        RMCALL_RxStatusMachine(_inst);

        if(0U == _inst->statusFlag & rmcall_statusFlag_rxBusy)
        {
            // run command

            // restart rx header
            RMCALL_CommandRecvEnable();
        }
    }
}


/* @} */

#endif // HITSIC_USE_RMCALL
