#include <drv_dmadvp.h>

#if defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0)

/*!
 * @addtogroup dmadvp
 * @{
 */

#define SYSLOG_TAG  ("DMADVP")
#define SYSLOG_LVL  (HITSIC_DMADVP_LOG_LVL)
#include "inc_syslog.hpp"

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

void DMADVP_TransferCreateHandle(dmadvp_handle_t *handle, DMADVP_Type *base,
        edma_callback callback)
{
    SYSLOG_I("DMADVP creating handle.");
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
    SYSLOG_I("DMADVP create handle complete.");
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
        SYSLOG_D("No full buffer to get !");
        return kStatus_DMADVP_NoFullBuffer;
    }
    *buffer = handle->fullBuffer.front();
    //PRINTF("get full buffer: 0x%-8.8x = 0x%-8.8x\n", buffer, handle->fullBuffer.front());
    handle->fullBuffer.pop();
    return kStatus_Success;
}

status_t DMADVP_TransferStart(DMADVP_Type *base, dmadvp_handle_t *handle)
{
    SYSLOG_V("Try to start transfer.");
    if (handle->emptyBuffer.empty())
    {
        SYSLOG_D("No empty buffer to use !");
        return kStatus_DMADVP_NoEmptyBuffer;
    }
    status_t result = 0;
    EDMA_PrepareTransfer(&handle->xferCfg, (void*) (base->dmaDataAddress), 1,
            handle->emptyBuffer.front(), 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
    handle->emptyBuffer.pop();
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
        handle->fullBuffer.push((uint8_t*)(handle->xferCfg.destAddr)); 
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
