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

	if(config->polarityFlags & DMADVP_VsyncActiveLow){base->vsncInterruptCfg = kPORT_InterruptFallingEdge;}
	else{base->vsncInterruptCfg = kPORT_InterruptRisingEdge;}

	if(config->polarityFlags & DMADVP_DataLatchOnRisingEdge){base->pclkInterruptCfg = kPORT_DMARisingEdge;	}
	else{base->pclkInterruptCfg = kPORT_DMAFallingEdge;}

	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, kPORT_InterruptOrDMADisabled);
	
	extInt_t::insert(base->vsnc_intc, base->vsnc_pin, DMADVP_VsncExtIntHandler/*TODO: fix this*/);
	DisableIRQ(base->dmaIrqn);
	NVIC_SetPriority(base->dmaIrqn, base->dmaIrqPrio);

	DMAMUX_SetSource(DMADVP0_DMAMUX_INST, base->dmauxChannel, base->dmamuxRequestSrc);
	DMAMUX_EnableChannel(DMADVP0_DMAMUX_INST, base->dmauxChannel);//启动DMAMUX通道0，按DMAMUX_SetSource设置

	EDMA_SetBandWidth(DMADVP0_DMA_INST, base->dmaChannel, kEDMA_BandwidthStall8Cycle);//休息
	//EDMA_SetChannelPreemptionConfig(DMA0,0u,&ecpc);//优先级很高
	EDMA_EnableAutoStopRequest(DMADVP0_DMA_INST, base->dmaChannel, true);
	EnableIRQ(base->dmaIrqn);

	return kStatus_Success;
}

//void DMADVP_Deinit(DMADVP_Type *base)


// void DMADVP_EnableInterrupts(DMADVP_Type *base, uint32_t mask);

// void DMADVP_DisableInterrupts(DMADVP_Type *base, uint32_t mask);

void DMADVP_TransferCreateHandle(dmadvp_handle_t *handle, DMADVP_Type *base, edma_callback callback)
{
	assert(base);
	assert(handle);

	uint32_t instance = DMADVP_GetInstance(base);
	
	dmadvp_handleList[instance] = handle;

	handle->base = base;
	EDMA_CreateHandle(&handle->dmaHandle, DMADVP0_DMA_INST, base->dmaChannel);//句柄存有使用哪个通道的信息
	EDMA_SetCallback(&handle->dmaHandle, callback, (void*)handle);//回调函数设置,userData设置为handle
	
	handle->transferStarted = true;
}

status_t DMADVP_TransferSubmitEmptyBuffer(DMADVP_Type *base, dmadvp_handle_t *handle, uint8_t *destAddr)
{
	if(handle->xferCfg.destAddr != NULL)
	{
		return kStatus_DMADVP_QueueFull;
	}
	EDMA_PrepareTransfer(&handle->xferCfg, (void *)(base->dmaDataAddress),
	        1, destAddr, 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
	EDMA_SubmitTransfer(&handle->dmaHandle, &handle->xferCfg);
	return kStatus_Success;
}

void DMADVP_TransferStart(DMADVP_Type *base, dmadvp_handle_t *handle)
{
	if(handle->xferCfg.destAddr == NULL)
	{
		return ;
	}
	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, base->vsncInterruptCfg);
}

void DMADVP_TransferStop(DMADVP_Type *base, dmadvp_handle_t *handle)
{
	EDMA_AbortTransfer(&handle->dmaHandle);
}

void DMADVP_VsncExtIntHandler(dmadvp_handle_t *handle)
{
	PORT_SetPinInterruptConfig(handle->base->vsnc_intc, handle->base->vsnc_pin, kPORT_InterruptOrDMADisabled);
	PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin, handle->base->pclkInterruptCfg);
	EDMA_StartTransfer(&handle->dmaHandle);//启动传输
}

void DMADVP_EdmaCallbackService(dmadvp_handle_t *handle, bool transferDone)
{
	handle->transferStarted = false;
	PORT_SetPinInterruptConfig(handle->base->pclk_intc, handle->base->pclk_pin, kPORT_InterruptOrDMADisabled);
	if(!transferDone)
	{
		//TODO: do something here to report error.
	}
}

/* @} */

#endif // ! HITSIC_USE_DMADVP
