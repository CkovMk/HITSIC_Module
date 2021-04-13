#include <drv_dmadvp.h>

#if defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0)

/*!
 * @addtogroup dmadvp
 * @{
 */

#define SYSLOG_TAG  ("DMADVP")
#define SYSLOG_LVL  (HITSIC_DMADVP_LOG_LVL)
#include <inc_syslog.h>

static dmadvp_handle_t *dmadvp_handleList[DMADVP_CNT];

status_t DMADVP_Init(DMADVP_Type *base, const dmadvp_config_t *config)
{
    SYSLOG_I("Init begin. v%d.%d.%d", HITSIC_VERSION_MAJOR(DRV_DMADVP_VERSION),
            HITSIC_VERSION_MINOR(DRV_DMADVP_VERSION), HITSIC_VERSION_PATCH(DRV_DMADVP_VERSION));
    assert(base);
    assert(config);

    base->imgSize = config->width * config->height * config->bytesPerPixel;
    SYSLOG_V("Image Size: %d (w) * %d (h) * %d = %d Byte.", config->width, config->height, config->bytesPerPixel, base->imgSize);
    if (config->polarityFlags & DMADVP_VsyncActiveLow)
    {
        base->vsncInterruptCfg = kPORT_InterruptFallingEdge;
        SYSLOG_V("VSNC active low.");
    }
    else
    {
        base->vsncInterruptCfg = kPORT_InterruptRisingEdge;
        SYSLOG_V("VSNC active high.");
    }

    if (config->polarityFlags & DMADVP_DataLatchOnRisingEdge)
    {
        base->pclkInterruptCfg = kPORT_DMARisingEdge;
        SYSLOG_V("PCLK data on rising edge.");
    }
    else
    {
        base->pclkInterruptCfg = kPORT_DMAFallingEdge;
        SYSLOG_V("PCLK data on falling edge.");
    }

    PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin,
            kPORT_InterruptOrDMADisabled);

    base->extintHandle.handler = DMADVP_VsncExtIntHandler;
    EXTINT_HandleInsert(EXTINT_GetInst(base->vsnc_intc), &base->extintHandle);

    //DisableIRQ(base->dmaIrqn);

    DMAMUX_SetSource(DMADVP0_DMAMUX_INST, base->dmauxChannel,
            base->dmamuxRequestSrc);
    DMAMUX_EnableChannel(DMADVP0_DMAMUX_INST, base->dmauxChannel); //启动DMAMUX通道0，按DMAMUX_SetSource设置

    EDMA_SetBandWidth(DMADVP0_DMA_INST, base->dmaChannel,
            kEDMA_BandwidthStall8Cycle); //休息
    //EDMA_SetChannelPreemptionConfig(DMA0,0u,&ecpc);//优先级很高
    EDMA_EnableAutoStopRequest(DMADVP0_DMA_INST, base->dmaChannel, true);
    NVIC_SetPriority(base->dmaIrqn, base->dmaIrqPrio);
    EnableIRQ(base->dmaIrqn);

    SYSLOG_I("DMADVP init success.");
    return kStatus_Success;
}

//void DMADVP_Deinit(DMADVP_Type *base)

// void DMADVP_EnableInterrupts(DMADVP_Type *base, uint32_t mask);

// void DMADVP_DisableInterrupts(DMADVP_Type *base, uint32_t mask);


/** Private Functions Begin */

void DMADVP_BufferQueInit(dmadvp_bufferQue_t *_buffer)
{
    _buffer->bufferHead = 0U;
    _buffer->bufferTail = 0U;
}

bool DMADVP_BufferQueEmpty(dmadvp_bufferQue_t *_buffer)
{
    return _buffer->bufferHead == _buffer->bufferTail;
}

bool DMADVP_BufferQueFull(dmadvp_bufferQue_t *_buffer)
{
    int16_t var = _buffer->bufferTail - _buffer->bufferHead;
    return (DMADVP_DRIVER_QUEUE_SIZE == var || -1 ==  var);
}

status_t DMADVP_BufferQuePush(dmadvp_bufferQue_t *_buffer, uint8_t *_data)
{
    if(DMADVP_BufferQueFull(_buffer))
    {
        return kStatus_Fail;
    }
    _buffer->buffer[_buffer->bufferTail] = _data;
    if(++_buffer->bufferTail == DMADVP_DRIVER_QUEUE_SIZE + 1U)
    {
        _buffer->bufferTail = 0U;
    }
    return kStatus_Success;
}

status_t DMADVP_BufferQuePop(dmadvp_bufferQue_t *_buffer)
{
    if(DMADVP_BufferQueEmpty(_buffer))
    {
        return kStatus_Fail;
    }
    if(++_buffer->bufferHead == DMADVP_DRIVER_QUEUE_SIZE + 1U)
    {
        _buffer->bufferHead = 0U;
    }
    return kStatus_Success;
}

//uint8_t *DMADVP_BufferQueBack(dmadvp_buffer_t *_buffer)
//{
//    return _buffer->buffer[_buffer->bufferTail - 1];
//}

uint8_t *DMADVP_BufferQueFront(dmadvp_bufferQue_t *_buffer)
{
    return _buffer->buffer[_buffer->bufferHead];
}

/** Private Functions End */

void DMADVP_TransferCreateHandle(dmadvp_handle_t *handle, DMADVP_Type *base,
        edma_callback callback)
{
    SYSLOG_I("DMADVP creating handle.");
    assert(base);
    assert(handle);

    uint32_t instance = DMADVP_GetInstance(base);

    dmadvp_handleList[instance] = handle;

    base->extintHandle.userData = (void*) handle;

    handle->base = base;
    EDMA_CreateHandle(&handle->dmaHandle, DMADVP0_DMA_INST, base->dmaChannel);//句柄存有使用哪个通道的信息
    EDMA_SetCallback(&handle->dmaHandle, callback, (void*) handle);	//回调函数设置,userData设置为handle

    handle->transferStarted = false;

    DMADVP_BufferQueInit(&handle->emptyBuffer);
    DMADVP_BufferQueInit(&handle->fullBuffer);

    SYSLOG_I("DMADVP create handle complete.");
}

status_t DMADVP_TransferSubmitEmptyBuffer(DMADVP_Type *base,
        dmadvp_handle_t *handle, uint8_t *buffer)
{
    assert(buffer);
    status_t ret = DMADVP_BufferQuePush(&handle->emptyBuffer, buffer);
    if(kStatus_Success != ret)
    {
        SYSLOG_E("BufferQue \"emptyBuffer\" is full. head = %d, tail = %d", handle->emptyBuffer.bufferHead, handle->emptyBuffer.bufferTail);
        return ret;
    }
    return kStatus_Success;
}

status_t DMADVP_TransferGetFullBuffer(DMADVP_Type *base,
        dmadvp_handle_t *handle, uint8_t **buffer)
{
    if (DMADVP_BufferQueEmpty(&handle->fullBuffer))
    {
        SYSLOG_D("No full buffer to get !");
        return kStatus_DMADVP_NoFullBuffer;
    }
    *buffer = DMADVP_BufferQueFront(&handle->fullBuffer);
    //PRINTF("get full buffer: 0x%-8.8x = 0x%-8.8x\n", buffer, handle->fullBuffer.front());
    DMADVP_BufferQuePop(&handle->fullBuffer);
    return kStatus_Success;
}

status_t DMADVP_TransferStart(DMADVP_Type *base, dmadvp_handle_t *handle)
{
    SYSLOG_V("Try to start transfer.");
    if (DMADVP_BufferQueEmpty(&handle->emptyBuffer))
    {
        SYSLOG_D("No empty buffer to use !");
        return kStatus_DMADVP_NoEmptyBuffer;
    }
    status_t result = 0;
    EDMA_PrepareTransfer(&handle->xferCfg, (void*) (base->dmaDataAddress), 1,
            DMADVP_BufferQueFront(&handle->emptyBuffer), 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
    DMADVP_BufferQuePop(&handle->emptyBuffer);
    result = EDMA_SubmitTransfer(&handle->dmaHandle, &handle->xferCfg);
    if(kStatus_Success != result)
    {
        SYSLOG_W("Submit DMA transfer failed (%8.8x) !", result);
        return result;
    }
    if(handle->transferStarted)
    {   //repeated start: 启动传输
        PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
                    handle->base->pclkInterruptCfg);
        EDMA_StartTransfer(&handle->dmaHandle);
        SYSLOG_V("Repeated start, No VSNC required. DMA transfer start.");
    }
    else
    {   //first start: 使能VSNC中断
        PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin,
                base->vsncInterruptCfg);
        handle->transferStarted = true;
        SYSLOG_V("First-time start. Sync to VSNC.");
    }
    SYSLOG_V("Start transfer success.");
    return kStatus_Success;
}

void DMADVP_TransferStop(DMADVP_Type *base, dmadvp_handle_t *handle)
{
    PORT_SetPinInterruptConfig(handle->base->vsnc_intc, handle->base->vsnc_pin,
                kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
                kPORT_InterruptOrDMADisabled);
    EDMA_AbortTransfer(&handle->dmaHandle);
    handle->transferStarted = false;
    SYSLOG_V("Stop transfer success.");
}

void DMADVP_VsncExtIntHandler(void *userData)
{
    assert(userData);
    dmadvp_handle_t *handle = (dmadvp_handle_t*) userData;
    PORT_SetPinInterruptConfig(handle->base->vsnc_intc, handle->base->vsnc_pin,
            kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
            handle->base->pclkInterruptCfg);
    EDMA_StartTransfer(&handle->dmaHandle);	//启动传输
    SYSLOG_V("VSNC asserted. DMA transfer start.");
}

void DMADVP_EdmaCallbackService(dmadvp_handle_t *handle, bool transferDone)
{
    if (transferDone)
    {
        if(kStatus_Success != DMADVP_BufferQuePush(&handle->fullBuffer, (uint8_t*)(handle->xferCfg.destAddr)))
        {
            SYSLOG_E("BufferQue \"fullBuffer\" is full. head = %d, tail = %d", handle->fullBuffer.bufferHead, handle->fullBuffer.bufferTail);
        }
    }
    else
    {
        SYSLOG_W("Callback without transfer done.");
    }
    PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
                 kPORT_InterruptOrDMADisabled);
    SYSLOG_V("Callback service done.");
}

/* @} */

#endif // ! HITSIC_USE_DMADVP
