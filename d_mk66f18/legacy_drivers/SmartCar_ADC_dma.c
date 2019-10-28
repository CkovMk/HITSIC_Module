#include"SmartCar_ADC_dma.h"

int AD_DMA_Init(void)
{
	//初始化链表和内部变量
	listc_constructor(&AdcDmaList, sizeof(AD_DMA_Sys_t));
	stopping = 1;

	/* Initialize EDMA. */
	edma_config_t userConfig;
	EDMA_GetDefaultConfig(&userConfig);
	if (if_EDMA_Init == 0)
	{
		if_EDMA_Init = 1;
		EDMA_Init(ADC_DMA_DMA_BASEADDR, &userConfig);//初始化
	}
	EDMA_CreateHandle(&EDMA_Handle_adc0, ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0);
	EDMA_SetCallback(&EDMA_Handle_adc0, ADC_Edma_Callback, NULL);
	//edma_channel_Preemption_config_t ecpc =
	//{
	//  .enableChannelPreemption = true, /*!< If true: a channel can be suspended by other channel with higher priority */
	//  .enablePreemptAbility = true,    /*!< If true: a channel can suspend other channel with low priority */
	//  .channelPriority = 15,      /*!< Channel priority */
	//};
	//EDMA_SetChannelPreemptionConfig(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, &ecpc);//优先级很低，不能打断摄像头的传输
	edma_handle_t* handle = &EDMA_Handle_adc0;
	/* Enable auto disable request feature */
	handle->base->TCD[handle->channel].CSR |= DMA_CSR_DREQ_MASK;
	/* Enable major interrupt */
	handle->base->TCD[handle->channel].CSR |= DMA_CSR_INTMAJOR_MASK;
	/* Enable interrupt when transfer is done. */
	EDMA_EnableChannelInterrupts(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, kEDMA_MajorInterruptEnable);
#if defined(FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT) && FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT
	/* Enable async DMA request. */
	EDMA_EnableAsyncRequest(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, true);
#endif /* FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT */

	/* Initialize DMAMUX. */
	if (if_DMAMUX_Init == 0)
	{
		if_DMAMUX_Init = 1;
		DMAMUX_Init(ADC_DMA_DMAMUX_BASEADDR);
	}
	DMAMUX_SetSource(ADC_DMA_DMAMUX_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, ADC_DMA_DMA_ADC_SOURCE_adc0); /* Map ADC source to channel 0 */
	DMAMUX_EnableChannel(ADC_DMA_DMAMUX_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0);

	/* Initialize ADC16. */
	adc16_config_t adcUserConfig;
	/*
	 * Initialization ADC for 16bit resolution, DMA mode, normal convert speed, VREFH/L as reference,
	 * enable continuous convert mode.
	 */
	ADC16_GetDefaultConfig(&adcUserConfig);
	adcUserConfig.resolution = ADC_DMA_resolution;
	adcUserConfig.enableContinuousConversion = true;
	adcUserConfig.clockSource = kADC16_ClockSourceAsynchronousClock;
	adcUserConfig.enableLowPower = true;
#if ((defined BOARD_ADC_USE_ALT_VREF) && BOARD_ADC_USE_ALT_VREF)
	adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
	ADC16_Init(ADC_DMA_ADC_BASEADDR, &adcUserConfig);
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	/* Auto calibration */
	if (kStatus_Success == ADC16_DoAutoCalibration(ADC_DMA_ADC_BASEADDR))
	{
		PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
	}
	else
	{
		PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
	}
#endif
	/* Enable software trigger.  */
	ADC16_EnableHardwareTrigger(ADC_DMA_ADC_BASEADDR, false);
	/* Enable DMA. */
	ADC16_EnableDMA(ADC_DMA_ADC_BASEADDR, true);
	return 0;
}

int AD_DMA_StartTransfer(AD_DMA_t* s)
{
	if (AdcDmaList.size>32)
	{
		return -10;
	}
	ADC_DMA_ENTER_CRITICAL();
	AD_DMA_Sys_t buf;
	//memset(&buf, 0, sizeof(AdcDmaSys_t));
	buf.p_userAD_DMA_t = s;
	
	listc_push_back(&AdcDmaList, &buf);
	if (stopping == 1)
	{
		stopping = 0;
		ADC_DMA_NEXT();
	}
	ADC_DMA_EXIT_CRITICAL();
	return 0;
}

void ADC_DMA_NEXT(void)
{
	if (AdcDmaList.size == 0) { return; }
	ADC_DMA_ENTER_CRITICAL();
	AD_DMA_Sys_t* b = listc_front(&AdcDmaList);

	//设置adc通道
	/* Configure channel and SW trigger ADC16. */
	adcChnConfig_adc0.channelNumber = b->p_userAD_DMA_t->channelNumber;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	adcChnConfig_adc0.enableDifferentialConversion = false;
#endif
	adcChnConfig_adc0.enableInterruptOnConversionCompleted = false;
	ADC16_SetChannelConfig(b->p_userAD_DMA_t->ADCx, b->p_userAD_DMA_t->channelGroup, &adcChnConfig_adc0);

	/* Setup transfer */
	EDMA_PrepareTransfer(&transferConfig_adc0, (void*) & (b->p_userAD_DMA_t->ADCx->R[b->p_userAD_DMA_t->channelGroup]), sizeof(uint32_t),
		(void*) & (b->p_userAD_DMA_t->SampleDataArray[0]), sizeof(uint32_t), sizeof(uint32_t),
		ADC_DMA_ADC_SAMPLE_COUNT * sizeof(uint32_t), kEDMA_PeripheralToMemory);
	EDMA_SetTransferConfig(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, &transferConfig_adc0, NULL);
	/* Enable transfer. */
	EDMA_StartTransfer(&EDMA_Handle_adc0);
	ADC_DMA_EXIT_CRITICAL();
}

void ADC_Edma_Callback(edma_handle_t* handle, void* userData, bool transferDone, uint32_t tcds)
{
	AD_DMA_Sys_t* b = listc_front(&AdcDmaList);
	//清理上次传输的结果
	/* Clear Edma interrupt flag. */
	EDMA_ClearChannelStatusFlags(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, kEDMA_InterruptFlag);
	//调用CallBack通知用户传输完成
	b->p_userAD_DMA_t->userCallBack(b->p_userAD_DMA_t);
	listc_pop_front(&AdcDmaList);

	//传输完成，开始下一次
	if (AdcDmaList.size > 0)
	{
		ADC_DMA_NEXT();
	}
	else//如果链表里面没有传输任务了就退出
	{
		stopping = 1;
	}
}

void ADC_DMA_ENTER_CRITICAL()
{
	EDMA_DisableChannelInterrupts(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, kEDMA_MajorInterruptEnable);
	ADC_DMA_CRITICAL_COUNT++;
}

void ADC_DMA_EXIT_CRITICAL()
{
	ADC_DMA_CRITICAL_COUNT--;
	if (ADC_DMA_CRITICAL_COUNT<=0)
	{
		EDMA_EnableChannelInterrupts(ADC_DMA_DMA_BASEADDR, ADC_DMA_DMA_CHANNEL_adc0, kEDMA_MajorInterruptEnable);
		ADC_DMA_CRITICAL_COUNT = 0;
	}
}

AD_DMA_t ADC_DMA_example;
void AD_DMA_DEMO(void)
{
	ADC_DMA_example.TransferDone = 0;
	ADC_DMA_example.ADCx = ADC0;
	ADC_DMA_example.channelGroup = 0;
	ADC_DMA_example.channelNumber = 11;//PTA8
	//ADC_DMA_example.userCallBack = AD_DMA_userCallBack_demo;
	ADC_DMA_example.userCallBack = AD_DMA_userCallBack_demo2;
	AD_DMA_Init();
	AD_DMA_StartTransfer(&ADC_DMA_example);
}

void AD_DMA_userCallBack_demo(AD_DMA_t* self)
{
	float result;
	for (int i = 0; i < ADC_DMA_ADC_SAMPLE_COUNT; i++)
	{
		result += self->SampleDataArray[i];
	}
	result /= ADC_DMA_ADC_SAMPLE_COUNT;
	result *= 3.3;
	result /= 65536.0;
	//result /= 256.0;
	PRINTF("PTA8:%fv\r\n", result);
	AD_DMA_StartTransfer(self);
}

void AD_DMA_userCallBack_demo2(AD_DMA_t* self)
{
	self->TransferDone = 1;
}
