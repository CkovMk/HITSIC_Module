#include "drv_edmadvp.hpp"


#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

static edmadvp_handle_t *edmadvp_handleList[EDMADVP_CNT];

status_t EDMADVP_Init(EDMADVP_Type *base, const edmadvp_config_t *config)
{
	assert(base);
	assert(config);

	base->imgSize = config->width * config->height * config->bytesPerPixel;

	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, kPORT_InterruptOrDMADisabled);
	
	extInt_t::insert(base->vsnc_intc, base->vsnc_pin, EDMADVP_VsncExtIntHandler/*TODO: fix this*/);
	DisableIRQ(base->dmaIrqn);
	NVIC_SetPriority(base->dmaIrqn, base->dmaIrqPrio);

	DMAMUX_SetSource(EDMADVP0_DMAMUX_INST, base->dmauxChannel, base->dmamuxRequestSrc);
	DMAMUX_EnableChannel(EDMADVP0_DMAMUX_INST, base->dmauxChannel);//启动DMAMUX通道0，按DMAMUX_SetSource设置

	EDMA_SetBandWidth(EDMADVP0_DMA_INST, base->dmaChannel, kEDMA_BandwidthStall8Cycle);//休息
	//EDMA_SetChannelPreemptionConfig(DMA0,0u,&ecpc);//优先级很高
	EDMA_EnableAutoStopRequest(EDMADVP0_DMA_INST, base->dmaChannel, true);
	EnableIRQ(handle->dmaIrqn);

	return kStatus_Success;
}

//void EDMADVP_Deinit(EDMADVP_Type *base)


// void EDMADVP_EnableInterrupts(EDMADVP_Type *base, uint32_t mask);

// void EDMADVP_DisableInterrupts(EDMADVP_Type *base, uint32_t mask);

void EDMADVP_TransferCreateHandle(edmadvp_handle_t *handle, EDMADVP_Type *base, edma_callback callback)
{
	assert(base);
	assert(handle);

	uint32_t instance = EDMADVP_GetInstance(base);
	
	edmadvp_handleList[instance] = handle;

	handle->base = base;
	EDMA_CreateHandle(&handle->dmaHandle, EDMADVP0_DMA_INST, base->dmaChannel);//句柄存有使用哪个通道的信息
	EDMA_SetCallback(&handle->dmaHandle, callback, (void*)handle);//回调函数设置,userData设置为handle
	
	handle->transferStarted = true;
}

status_t EDMADVP_TransferSubmitEmptyBuffer(EDMADVP_Type *base, edmadvp_handle_t *handle, uint8_t *destAddr)
{
	if(handle->xerCfg.destAddr != NULL)
	{
		return kStatus_EDMADVP_QueueFull;
	}
	EDMA_PrepareTransfer(&handle->xferCfg, (void *)(base->dmaDataAddress)), 1,
		destAddr, 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
	EDMA_SubmitTransfer(&base->dmaHandle, &handle->xferCfg);
	return kStatus_Success;
}

void EDMADVP_TransferStart(EDMADVP_Type *base, edmadvp_handle_t *handle)
{
	if(handle->xerCfg.destAddr == NULL)
	{
		return ;
	}
	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, base->vsncInterruptCfg);
}

void EDMADVP_TransferStop(EDMADVP_Type *base, edmadvp_handle_t *handle)
{
	EDMA_AbortTransfer(&base->dmaHandle);
}

void EDMADVP_VsncExtIntHandler(edmadvp_handle_t *handle)
{
	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, kPORT_InterruptOrDMADisabled);
	PORT_SetPinInterruptConfig(base->pclk_intc, base->pclk_pin, base->pclkInterruptCfg);
	EDMA_StartTransfer(&handle->base->dmaHandle);//启动传输
}

/**
 * @brief DMA回调预处理函数。必须在EDMA回调函数中调用。
 * 
 * @param handle EDMADVP句柄指针，应由回调函数中的userData经类型转换得到。
 * @param transferDone 标志位，标志是否传输完成。
 */
void EDMADVP_EdmaCallbackService(edmadvp_handle_t *handle, bool transferDone)
{
	handle->transferStarted = false;
	PORT_SetPinInterruptConfig(base->pclk_intc, base->pclk_pin, kPORT_InterruptOrDMADisabled);
	if(!transferDone)
	{
		//TODO: do something here to report error.
	}
}



















#endif // ! HITSIC_USE_EDMADVP