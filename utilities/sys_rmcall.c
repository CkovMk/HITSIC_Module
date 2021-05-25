#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

/*!
 * @addtogroup rmcall
 * @{
 */

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

/** Private Functions */

void RMCALL_TxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_txBusy)
    {
    case 0: // no tx
        break;
    case rmcall_statusFlag_txHead:
        // tx data here.
        SYSLOG_D("Tx Head Done, Tx Data. Size = %4.4d.", _inst->txHeaderBuffer.dataSize);
        _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_txHead)) | rmcall_statusFlag_txData;
        _inst->xfer_tx(_inst->txDataBuffer, _inst->txHeaderBuffer.dataSize);
        break;
    case rmcall_statusFlag_txData:
        // tx finished. go idle.
        SYSLOG_D("Tx Data Done.");
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
        if(HITSIC_RMCALL_HEADER_MAGIC != _inst->rxHeaderBuffer.magic)
        {
            SYSLOG_W("Rx Head Magic Error. Expected 0x%8.8x, Got 0x%8.8x.", HITSIC_RMCALL_HEADER_MAGIC, _inst->rxHeaderBuffer.magic);
            _inst->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            break;
        }
        
        SYSLOG_D("Rx Head Done. ID = 0x%4.4x, Size = %4.4d.", _inst->rxHeaderBuffer.handleId, _inst->rxHeaderBuffer.dataSize);
        
        rmcall_handle_t **p_handle = rmcall_isrDict_get(_inst->isrDict, _inst->rxHeaderBuffer.handleId);
        if(NULL == p_handle)
        {
            SYSLOG_W("Rx HandleID 0x%4.4x Not Found.", _inst->rxHeaderBuffer.handleId);
            //_inst->rxDataBuffer = NULL;
            if(0U == _inst->rxHeaderBuffer.dataSize) // No data. Start another rx head immediately.
            {
                _inst->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            }
            // recv dummy data.
            else if(_inst->rxHeaderBuffer.dataSize <= HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2) 
              // If the dummy data size is within the capablity of a single transfer:
              // Allocate (dataSize + 2) byte of ram, receive (dataSize) bytes of data and discard. 
              // The first 2 bytes is used to indicate remaining bytes to be recveved,
              // in this case, will always be 0.
            {
                _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxHead)) | rmcall_statusFlag_rxData | rmcall_statusFlag_rxIdMissing;
                _inst->rxDataBuffer = malloc(_inst->rxHeaderBuffer.dataSize + 2U);
                if(NULL == _inst->rxDataBuffer)
                {
                    //memory allocation error !
                }
                *((uint16_t*)_inst->rxDataBuffer) = 0U;
                _inst->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), _inst->rxHeaderBuffer.dataSize);
            }
            else
              // if the dummy data size exceeds the capablity of a single transfer:
              // Allocate (HITSIC_RMCALL_PUBLIC_BUF_SIZE) byte of ram, receive (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2) bytes of data and discard,
              // then continue this process until no data is left.
              // The first 2 bytes is used to indicate remaining bytes to be recveved, in this case,
              // will be set to (dataSize - (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2)) and continue decreases.
            {
                _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxHead)) | rmcall_statusFlag_rxData | rmcall_statusFlag_rxIdMissing;
                _inst->rxDataBuffer = malloc(HITSIC_RMCALL_PUBLIC_BUF_SIZE);
                if(NULL == _inst->rxDataBuffer)
                {
                    //memory allocation error !
                }
                *((uint16_t*)_inst->rxDataBuffer) = _inst->rxHeaderBuffer.dataSize - (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
                _inst->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
            }
        }
        else if(0U == _inst->rxHeaderBuffer.dataSize) // No data. go idle.
        {
            // rx finished. go idle.
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
        }
        else //recv data.
        {
            _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxHead)) | rmcall_statusFlag_rxData;
            
            if(NULL != (*p_handle)->recvData && (*p_handle)->recvDataLen < _inst->rxHeaderBuffer.dataSize)
            {
                free((*p_handle)->recvData);
                (*p_handle)->recvData = NULL;
                (*p_handle)->recvDataLen = 0U;
            }
            
            if(NULL == (*p_handle)->recvData)
            {
                (*p_handle)->recvDataLen = _inst->rxHeaderBuffer.dataSize;
                (*p_handle)->recvData = malloc((*p_handle)->recvDataLen);
            }
            
            _inst->rxDataBuffer = (*p_handle)->recvData;
            _inst->xfer_rx(_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize);
        }
        
        
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);// for test purpose only. //FIXME
        //TODO
        //if(_inst->rxHeaderBuffer.handleId)
        break;
        
    case rmcall_statusFlag_rxData:
      
        if(_inst->statusFlag & rmcall_statusFlag_rxIdMissing)
        {
            if(0U == *((uint16_t*)_inst->rxDataBuffer)) // rx dummy data finished.
            {
                free(_inst->rxDataBuffer);
                _inst->rxDataBuffer = NULL; 
                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxIdMissing);
                _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_rxHead;
                _inst->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            }
            else if (*((uint16_t*)_inst->rxDataBuffer) <= (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2))
            {
                _inst->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), *((uint16_t*)_inst->rxDataBuffer));
                *((uint16_t*)_inst->rxDataBuffer) = 0U;
            }
            else
            {
                _inst->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
                *((uint16_t*)_inst->rxDataBuffer) -= (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
            }
            break;
        }
        // rx finished. go idle.
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
        break;
    default:
        assert(0); // should never end up here.
        break;
    }
}





/** Public Functions */

status_t RMCALL_Init(rmcall_t *_inst, rmcall_config_t const * const _config)
{
    assert(_config->xfer_tx);
    assert(_config->xfer_rx);

    _inst->xfer_tx = _config->xfer_tx;
    _inst->xfer_rx = _config->xfer_rx;
    _inst->xferAbort_tx = _config->xferAbort_tx;
    _inst->xferAbort_rx = _config->xferAbort_rx;

    _inst->statusFlag = 0U;

    rmcall_isrDict_init(_inst->isrDict);
    
    return kStatus_Success;
}

void RMCALL_DeInit(rmcall_t *_inst)
{
    _inst->xfer_tx = NULL;
    _inst->xfer_rx = NULL;

    _inst->statusFlag = 0U;

    rmcall_isrDict_clear(_inst->isrDict);
}

status_t RMCALL_HandleInsert(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);
    
    if(_handle->handleId > 65533)
    {
        SYSLOG_W("Insert Fail. Handle ID Out of Range !");
        return kStatus_Fail; // 65534 & 65535 is used for OOR detection.
    }

    HAL_EnterCritical();
    rmcall_isrDict_set_at(_inst->isrDict, _handle->handleId, _handle);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_HandleRemove(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    rmcall_isrDict_erase(_inst->isrDict, _handle->handleId);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_CommandSend(rmcall_t *_inst, uint16_t _handleId, void *_data, uint16_t _dataSize)
{
    assert(_inst);

    status_t ret = 0U;

    if(0U != (_inst->statusFlag & rmcall_statusFlag_txBusy))
    {
        return kStatus_RMCALL_TxBusy;
    }

    _inst->txHeaderBuffer.magic = HITSIC_RMCALL_HEADER_MAGIC;
    _inst->txHeaderBuffer.handleId = _handleId;
    _inst->txHeaderBuffer.dataSize = _dataSize;
    _inst->txDataBuffer = _data;

    _inst->statusFlag |= rmcall_statusFlag_txHead;
    SYSLOG_I("Tx Head. ID = 0x%4.4x, Size = %4.4d.", _handleId, _dataSize);
    ret = _inst->xfer_tx(&_inst->txHeaderBuffer, sizeof(rmcall_header_t));
    
    return ret;
}

status_t RMCALL_CommandRecvEnable(rmcall_t *_inst)
{
    assert(_inst);
    status_t ret = kStatus_Success;

    if(0U == (_inst->statusFlag & rmcall_statusFlag_rxBusy))
    {
        _inst->statusFlag |= rmcall_statusFlag_rxHead;
        ret =  _inst->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
        if(kStatus_Success == ret)
        {
            SYSLOG_I("Recv Enabled.");
        }
        else
        {
            SYSLOG_I("Recv Enable Failed. Transfer Error.");
        }
        
    }
    
    return ret;
}

status_t RMCALL_CommandRecvDisable(rmcall_t *_inst)
{
    assert(_inst);
    //FIXME
    if(0U != (_inst->statusFlag & rmcall_statusFlag_rxBusy))
    {
        _inst->xferAbort_rx();
        _inst->statusFlag &= (~rmcall_statusFlag_rxBusy);
        SYSLOG_I("Recv Disabled.");
        return kStatus_Success;
    }
    
    SYSLOG_W("Recv Disable failed. Rx Busy.");
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
        

        if(0U == (_inst->statusFlag & rmcall_statusFlag_rxBusy))
        {
            // run command

            // restart rx header
            SYSLOG_I("Handler Executed. Enable Rx Head.");
            RMCALL_CommandRecvEnable(_inst);
        }
    }
}


/* @} */

#endif // HITSIC_USE_RMCALL
