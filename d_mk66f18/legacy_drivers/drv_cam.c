/*
 *	@by:		SmartCar SJMC
 *	@name:		drvcam.cpp
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2018.12.30 05:44
 */

#include "drv_cam.h"

#ifdef __cplusplus
extern "C" {
#endif

camera_img_t camera_imgBuf;
bool camera_finishFlag;
bool camera_xferFlag;


uint8_t camera_uartRxBuf[8];
uint8_t camera_uartTxBuf[8];
uint16_t camera_version;

edma_handle_t camera_edmaHandle;
edma_transfer_config_t camera_edmaXferCfg;
edma_config_t camera_edmaCfg;

//exInt_t* camera_vsync_exIntCfg;

//需要配置到摄像头的数据
int16_t camera_cfg[(int16_t)CAM_CMD_CONFIG_FINISH][2] =
{
  {(int16_t)CAM_CMD_AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
  //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
  {(int16_t)CAM_CMD_EXP_TIME,          32222}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
  {(int16_t)CAM_CMD_FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
  {(int16_t)CAM_CMD_SET_COL,           CAM_IMG_C}, //图像列数量        范围1-752     K60采集不允许超过188
  {(int16_t)CAM_CMD_SET_ROW,           CAM_IMG_R}, //图像行数量        范围1-480
  {(int16_t)CAM_CMD_LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {(int16_t)CAM_CMD_UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {(int16_t)CAM_CMD_GAIN,              32},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


  {(int16_t)CAM_CMD_INIT,              0}    //摄像头开始初始化
};
//从摄像头内部获取到的配置数据
int16_t get_camera_cfg[(int16_t)CAM_CMD_CONFIG_FINISH - 1][2] =
{
  {(int16_t)CAM_CMD_AUTO_EXP,          0},   //自动曝光设置      
  {(int16_t)CAM_CMD_EXP_TIME,          0},   //曝光时间          
  {(int16_t)CAM_CMD_FPS,               0},   //图像帧率          
  {(int16_t)CAM_CMD_SET_COL,           0},   //图像列数量        
  {(int16_t)CAM_CMD_SET_ROW,           0},   //图像行数量        
  {(int16_t)CAM_CMD_LR_OFFSET,         0},   //图像左右偏移量    
  {(int16_t)CAM_CMD_UD_OFFSET,         0},   //图像上下偏移量    
  {(int16_t)CAM_CMD_GAIN,              0},   //图像增益          
};


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

void CAMERA_CfgWrite(void)
{
	/*设置参数    具体请参看使用手册*/
	//uint16_t temp, i;
	//uint8_t  send_buffer[4];
	//uart_init();	                 //初始换串口 配置摄像头    
	systick_delay_ms(200);        //等待摄像头上电初始化成功
	//uart_receive_flag = 0;

	for (uint8_t i = 0; i < (int16_t)(CAM_CMD_CONFIG_FINISH); i++)//开始配置摄像头并重新初始化
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = camera_cfg[i][0];
		camera_uartTxBuf[2] = (uint8_t)(camera_cfg[i][1] >> 8);
		camera_uartTxBuf[3] = (uint8_t)camera_cfg[i][1];
		UART_WriteBlocking(CAM_UART_BASE, camera_uartTxBuf, 4);
		//systick_delay_ms(2);
	}

	//while (!uart_receive_flag); //等待摄像头初始化成功
	//uart_receive_flag = 0;
	//while ((0xff != camera_uartRxBuf[1]) || (0xff != camera_uartRxBuf[2]));
	do {
		UART_ReadBlocking(CAM_UART_BASE, camera_uartRxBuf, 1);
	} while ((0xff != camera_uartRxBuf[0]));
	//以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
	//利用camera_setEpTime函数单独配置的曝光数据不存储在eeprom中
	//获取配置便于查看配置是否正确
	CAMERA_CfgRead();
}

void CAMERA_CfgRead(void)
{
	for (uint8_t i = 0; i < (int16_t)(CAM_CMD_CONFIG_FINISH) - 1; i++)
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = (int16_t)(CAM_CMD_GET_STATUS);
		camera_uartTxBuf[2] = (uint8_t)(get_camera_cfg[i][0] >> 8);
		camera_uartTxBuf[3] = (uint8_t)get_camera_cfg[i][0];

		UART_WriteBlocking(CAM_UART_BASE, camera_uartTxBuf, 4);
		UART_ReadBlocking(CAM_UART_BASE, camera_uartRxBuf, 3);
		get_camera_cfg[i][1] = camera_uartRxBuf[1] << 8 | camera_uartRxBuf[2];
	}
}

//get camera hardware version
uint16_t CAMERA_GetVersion(void)
{
	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = (int16_t)(CAM_CMD_GET_STATUS);
	camera_uartTxBuf[2] = (uint8_t)((int16_t)CAM_CMD_GET_VERSION >> 8);
	camera_uartTxBuf[3] = (uint8_t)((int16_t)CAM_CMD_GET_VERSION);

	UART_WriteBlocking(CAM_UART_BASE, camera_uartTxBuf, 4);
	UART_ReadBlocking(CAM_UART_BASE, camera_uartRxBuf, 3);
	return camera_version = ((uint16_t)(camera_uartRxBuf[1] << 8) | camera_uartRxBuf[2]);
}

//set exposure time
uint16_t CAMERA_SetExposeTime(uint16_t light)
{
	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = (int16_t)(CAM_CMD_SET_EXP_TIME);
	camera_uartTxBuf[2] = (uint8_t)(light >> 8);
	camera_uartTxBuf[3] = (uint8_t)light;

	UART_WriteBlocking(CAM_UART_BASE, camera_uartTxBuf, 4);
	UART_ReadBlocking(CAM_UART_BASE, camera_uartRxBuf, 3);
	return ((uint16_t)(camera_uartRxBuf[1] << 8) | camera_uartRxBuf[2]);
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




#ifdef __cplusplus
}
#endif

