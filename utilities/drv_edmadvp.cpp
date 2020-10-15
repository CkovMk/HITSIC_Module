#include "drv_edmadvp.hpp"


#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

void CAMERA_Init()
{
/*****************UART*INIT*******************/
	const uart_config_t camera_uart_config = {
		.baudRate_Bps = 9600,
		.parityMode = kUART_ParityDisabled,
		.stopBitCount = kUART_OneStopBit,
		.txFifoWatermark = 0,
		.rxFifoWatermark = 0,
		.idleType = kUART_IdleTypeStartBit,
		.enableTx = true,
		.enableRx = true
	};

	while (kStatus_Success != UART_Init(CAM_UART_BASE, &camera_uart_config, CAMERA_UART_CLOCK_SOURCE));

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