#include "drv_dmadvp.hpp"

#if defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0)

/*!
 * @addtogroup dmadvp
 * @{
 */

static dmadvp_handle_t *dmadvp_handleList[DMADVP_CNT];

status_t DMADVP_Init(DMADVP_Type *base, const dmadvp_config_t *config)
{
    assert(base);
    assert(config);

    base->imgSize = config->width * config->height * config->bytesPerPixel;

    if (config->polarityFlags & DMADVP_VsyncActiveLow)
    {
        base->vsncInterruptCfg = kPORT_InterruptFallingEdge;
    }
    else
    {
        base->vsncInterruptCfg = kPORT_InterruptRisingEdge;
    }

    if (config->polarityFlags & DMADVP_DataLatchOnRisingEdge)
    {
        base->pclkInterruptCfg = kPORT_DMARisingEdge;
    }
    else
    {
        base->pclkInterruptCfg = kPORT_DMAFallingEdge;
    }

    PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin,
            kPORT_InterruptOrDMADisabled);

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

    return kStatus_Success;
}

//void DMADVP_Deinit(DMADVP_Type *base)

// void DMADVP_EnableInterrupts(DMADVP_Type *base, uint32_t mask);

// void DMADVP_DisableInterrupts(DMADVP_Type *base, uint32_t mask);

void DMADVP_TransferCreateHandle(dmadvp_handle_t *handle, DMADVP_Type *base,
        edma_callback callback)
{
    assert(base);
    assert(handle);

    uint32_t instance = DMADVP_GetInstance(base);

    dmadvp_handleList[instance] = handle;

    handle->extIntHandle = extInt_t::insert(base->vsnc_intc, base->vsnc_pin,
            DMADVP_VsncExtIntHandler);
    assert(handle->extIntHandle);
    handle->extIntHandle->setUserData((void*) handle);

    handle->base = base;
    EDMA_CreateHandle(&handle->dmaHandle, DMADVP0_DMA_INST, base->dmaChannel);//句柄存有使用哪个通道的信息
    EDMA_SetCallback(&handle->dmaHandle, callback, (void*) handle);	//回调函数设置,userData设置为handle

    handle->transferStarted = false;
}

status_t DMADVP_TransferSubmitEmptyBuffer(DMADVP_Type *base,
        dmadvp_handle_t *handle, uint8_t *buffer)
{
    assert(buffer);
    handle->emptyBuffer.push(buffer);
    return kStatus_Success;
}

status_t DMADVP_TransferGetFullBuffer(DMADVP_Type *base,
        dmadvp_handle_t *handle, uint8_t **buffer)
{
    if (handle->fullBuffer.empty())
    {
        return kStatus_DMADVP_NoFullBuffer;
    }
    *buffer = handle->fullBuffer.front();
    //PRINTF("get full buffer: 0x%-8.8x = 0x%-8.8x\n", buffer, handle->fullBuffer.front());
    handle->fullBuffer.pop();
    return kStatus_Success;
}

status_t DMADVP_TransferStart(DMADVP_Type *base, dmadvp_handle_t *handle)
{
    if (handle->emptyBuffer.empty())
    {
        return kStatus_DMADVP_NoEmptyBuffer;
    }
    status_t result = 0;
    EDMA_PrepareTransfer(&handle->xferCfg, (void*) (base->dmaDataAddress), 1,
            handle->emptyBuffer.front(), 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
    handle->emptyBuffer.pop();
    result = EDMA_SubmitTransfer(&handle->dmaHandle, &handle->xferCfg);
    if(kStatus_Success != result)
    {
        return result;
    }
    if(handle->transferStarted)
    {   //repeated start: 启动传输
        PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
                    handle->base->pclkInterruptCfg);
        EDMA_StartTransfer(&handle->dmaHandle);
    }
    else
    {   //first start: 使能VSNC中断
        PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin,
                base->vsncInterruptCfg);
        handle->transferStarted = true;
    }
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
}

void DMADVP_EdmaCallbackService(dmadvp_handle_t *handle, bool transferDone)
{
    if (transferDone)
    {
        handle->fullBuffer.push((uint8_t*)(handle->xferCfg.destAddr)); 
    }
    PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin,
                 kPORT_InterruptOrDMADisabled);
}

/* @} */

#endif // ! HITSIC_USE_DMADVP
