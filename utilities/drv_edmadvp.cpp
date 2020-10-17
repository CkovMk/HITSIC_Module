#include "drv_edmadvp.hpp"


#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

static edmadvp_handle_t *edmadvp_handleList[EDMADVP_CNT];

status_t EDMADVP_Init(EDMADVP_Type *base, const edmadvp_config_t *config)
{
	assert(base);
	assert(config);
	/*TODO: more assert*/

	base->imgSize = config->width * config->height * config->bytesPerPixel;

	PORT_SetPinInterruptConfig(base->vsnc_intc, base->vsnc_pin, kPORT_InterruptOrDMADisabled);
	
	extInt_t::insert(base->vsnc_intc, base->vsnc_pin, __handler/*TODO: fix this*/);
	DisableIRQ(base->dmaIrqn);
	NVIC_SetPriority(base->dmaIrqn, base->dmaIrqPrio);

	DMAMUX_SetSource(EDMADVP0_DMAMUX_INST, base->dmauxChannel, base->dmamuxRequestSrc);
	DMAMUX_EnableChannel(EDMADVP0_DMAMUX_INST, base->dmauxChannel);//启动DMAMUX通道0，按DMAMUX_SetSource设置

	EDMA_SetBandWidth(EDMADVP0_DMA_INST, base->dmaChannel, kEDMA_BandwidthStall8Cycle);//休息
	//EDMA_SetChannelPreemptionConfig(DMA0,0u,&ecpc);//优先级很高
	EDMA_EnableAutoStopRequest(EDMADVP0_DMA_INST, base->dmaChannel, true);
	
	EDMA_CreateHandle(&base->edmaHandle, EDMADVP0_DMA_INST, base->dmaChannel);//句柄存有使用那个通道的信息
	EDMA_SetCallback(&base->edmaHandle, CAMERA_Xfer_EdmaCallback/*TODO:Fix*/, base);//回调函数设置
	

}

void EDMADVP_Deinit(EDMADVP_Type *base);

// void EDMADVP_EnableInterrupts(EDMADVP_Type *base, uint32_t mask);

// void EDMADVP_DisableInterrupts(EDMADVP_Type *base, uint32_t mask);

status_t EDMADVP_TransferCreateHandle(EDMADVP_Type *base, csi_handle_t *handle, csi_transfer_callback_t callback, void *userData)
{
	assert(base);
	assert(handle);

	uint32_t instance = EDMADVP_GetInstance(base);

	handle->callback = callback;
	handle->userData = userData;
	
	edmadvp_handleList[instance] = handle;

	EDMA_PrepareTransfer(&handle->xferCfg, (void *)(base->dmaDataAddress)), 1,
		4096/*this is incorrect, will be set to NULL later.*/, 1, 1, base->imgSize, kEDMA_PeripheralToMemory);
	handle->xferCfg.destAddr = NULL;
	EnableIRQ(base->dmaIrqn);

}

status_t EDMADVP_TransferStart(EDMADVP_Type *base, edmadvp_handle_t handle)
{
	if(handle->xferCfg.destAddr != NULL)
	{
		return kStatus_Fail;
	}
	if(handle->emptyBufferQueue.empty())
	{
		return kStatus_EDMADVP_NoEmptyBuffer;
	}
	handle->xferCfg.destAddr = handle->emptyBufferQueue.front();
	handle->emptyBufferQueue.pop();
}

status_t EDMADVP_TransferStop(EDMADVP_Type *base, edmadvp_handle_t handle)
{
	
}

status_t EDMADVP_TransferSubmitEmptyBuffer(EDMADVP_Type *base, edmadvp_handle_t *handle, uint8_t* frameBuffer)
{
	handle->emptyBufferQueue.push(frameBuffer);
}

status_t EDMADVP_TransferGetFullBuffer(EDMADVP_Type *base, edmadvp_handle_t *handle, uint8_t *frameBuffer)
{
	frameBuffer;
}

void EDMADVP_ExtIntHandler(void)//vsnc
{

}

void EDMADVP_DmaIntHandler(void)//dma finish
{

}

















void EDMADVP_Init()
{

/**************GPIO****INIT******************/
	camera_disable_vsync_irq();
	EXTINT_PortSetup(GPIO_CAM_VSNC_PORT, GPIO_CAM_VSNC_PIN, 0, CAMERA_VSNC_ExtIntHandler);
	camera_disable_pclk_irq();
	PORT_SetPinInterruptConfig(GPIO_CAM_PCLK_PORT, GPIO_CAM_PCLK_PIN, kPORT_DMARisingEdge);
	NVIC_SetPriority(CAM_PCLK_DMA_IRQn, CAM_PCLK_DMA_PRIO);
/***************DMA*INIT*************************/
	DMAMUX_SetSource(DMAMUX0, CAM_DMAMUX_Chnl, CAM_DMA_REQUEST_SOURCE);
	DMAMUX_EnableChannel(DMAMUX0, CAM_DMAMUX_Chnl);//启动DMA通道0，按DMAMUX_SetSource设置

	/*  Configure EDMA one shot transfer
	* camera_edmaCfg.enableRoundRobinArbitration = false;
	* camera_edmaCfg.enableHaltOnError = true;
	* camera_edmaCfg.enableContinuousLinkMode = false;
	* camera_edmaCfg.enableDebugMode = false;
	*/
	//  edma_channel_Preemption_config_t ecpc =
	//  {
	//    .enableChannelPreemption=false, /*!< If true: a channel can be suspended by other channel with higher priority */
	//    .enablePreemptAbility=true,    /*!< If true: a channel can suspend other channel with low priority */
	//    .channelPriority=10,      /*!< Channel priority */
	//  };
	//EDMA_GetDefaultConfig(&camera_edmaCfg);//获取默认配置
	//camera_edmaCfg.enableHaltOnError = true;//出错时：停
	//EDMA_Init(DMA0, &camera_edmaCfg);//初始化
	EDMA_SetBandWidth(DMA0, CAM_DMA_Chnl, kEDMA_BandwidthStall8Cycle);//休息
	//EDMA_SetChannelPreemptionConfig(DMA0,0u,&ecpc);//优先级很高
	EDMA_EnableAutoStopRequest(DMA0, CAM_DMA_Chnl, true);
	EDMA_CreateHandle(&camera_edmaHandle, DMA0, CAM_DMA_Chnl);//句柄存有使用那个通道的信息
	EDMA_SetCallback(&camera_edmaHandle, CAMERA_Xfer_EdmaCallback, NULL);//回调函数设置
	
	//camera_imgBufSwap = false;
	camera_finishFlag = false;
	camera_xferFlag = false;
	//camera_imgData = &camera_imgBuf[1];
	//camera_imgXfer = &camera_imgBuf[0];
	EDMA_PrepareTransfer(&camera_edmaXferCfg, (void *)(uint32_t)(CAM_DATA_DMA_ADDR), 1,
		&(camera_imgBuf.img), 1, 1, CAM_IMG_C*CAM_IMG_R, kEDMA_PeripheralToMemory);
}


void CAMERA_BeginXfer(void)
{
	camera_finishFlag = false;
	camera_xferFlag = false;
	camera_enable_vsync_irq();
}

void CAMERA_VSNC_ExtIntHandler(void)
{
	camera_disable_vsync_irq();
	camera_xferFlag = true;
	EDMA_SubmitTransfer(&camera_edmaHandle, &camera_edmaXferCfg);
	EDMA_StartTransfer(&camera_edmaHandle);//启动
	camera_enable_pclk_irq();
}

void CAMERA_Xfer_EdmaCallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	if (transferDone)
	{
		camera_disable_pclk_irq();
		camera_finishFlag = true;
		camera_xferFlag = false;
		// (*camera_imgXfer).tStamp = sysirq_sysTimerCnt;
		// //switch buffer, avoid read-while-write problem.
		// if (camera_imgBufSwap) 
		// {
		// 	camera_imgData = &camera_imgBuf[0];
		// 	camera_imgXfer = &camera_imgBuf[1];
		// }
		// else 
		// {
		// 	camera_imgData = &camera_imgBuf[1];
		// 	camera_imgXfer = &camera_imgBuf[0];
		// }
		// camera_edmaXferCfg.destAddr = (uint32_t)(&(*camera_imgXfer).img);
		// camera_imgBufSwap = ~camera_imgBufSwap;
		// camera_enable_vsync_irq();//场中断
	}
}


#endif // ! HITSIC_USE_EDMADVP