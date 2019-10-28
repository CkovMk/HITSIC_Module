/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		摄像头相关函数（单缓存版）
 * @company	        哈工大智能车创新俱乐部
 * @author     		王学嘉 qq512924543
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		摄像头引脚固定，最好不要轻易更改，使用时调用camera_init_MT9V032()
		函数进行初始化，每次图像采集结束后会将mt9v032_finish_flag置为1，图像
		采集结果保存在image_Buffer_0内。每次读取完图像记得将mt9v032_finish_flag
		置零。

		哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#include "SmartCar_Camera.h"

volatile bool CameraON = false; //控制标志位，内部控制用，记录当前摄像头的是同情况
volatile bool CameraInitialized = false;//记录是否经过初始化
//volatile bool CameraDoingTransmission = false;//记录当前状态，true意味着当前正在采集一副图像中，从发生场中断开始到dma传输结束
volatile uint8_t if_EDMA_Init = 0;
volatile uint8_t if_DMAMUX_Init = 0;
volatile uint8_t mt9v032_finish_flag = 0;//一场图像采集完成标志位，用于同步最新的一帧图像，完成时会被置1，需手动清0
uint8_t image_Buffer_0[ROW][COL];
edma_transfer_config_t transferToBuffer0Config;
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
uint8_t image_Buffer_index = 0;//标志当前mt9v032_finish_flag = 1;时，哪个缓存有效
uint8_t image_Buffer_1[ROW][COL];
edma_transfer_config_t transferToBuffer1Config;
#endif

uint8_t image_size_half[ROW / 2][COL / 2];

//uint16_t version;
uint8_t   receive[3];
//uint8_t   receive_num = 0;
//uint8_t   uart_receive_flag = 1;
edma_handle_t Camera_Mission_EDMA_Handle;
edma_transfer_config_t transferConfig;
edma_config_t userConfig;

//需要配置到摄像头的数据
int16_t MT9V032_CFG[CONFIG_FINISH][2] =
{
  {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
  //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
  {EXP_TIME,          900}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
  {FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
  {SET_COL,           COL}, //图像列数量        范围1-752     K60采集不允许超过188
  {SET_ROW,           ROW}, //图像行数量        范围1-480
  {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {GAIN,              64},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


  {INIT,              0}    //摄像头开始初始化
};
//从摄像头内部获取到的配置数据
int16_t GET_CFG[CONFIG_FINISH - 1][2] =
{
  {AUTO_EXP,          0},   //自动曝光设置      
  {EXP_TIME,          0},   //曝光时间          
  {FPS,               0},   //图像帧率          
  {SET_COL,           0},   //图像列数量        
  {SET_ROW,           0},   //图像行数量        
  {LR_OFFSET,         0},   //图像左右偏移量    
  {UD_OFFSET,         0},   //图像上下偏移量    
  {GAIN,              0},   //图像增益          
};


void camera_uart_init()
{
	uart_init(CAMERA_UART, 9600U);
}

//void uart_getchar(UART_Type* base, uint8_t* ch)
//{
//	UART_ReadBlocking(base, ch, 1);
//}
//void uart_putchar(UART_Type* base, uint8_t ch)
//{
//	UART_WriteBlocking(base, &ch, 1);
//}
//void simple_delay(long t)
//{
//	while (t--);
//}


gpio_pin_config_t Input_port = {
	 .pinDirection = kGPIO_DigitalInput,
	 .outputLogic = 0U
};
port_pin_config_t Port_PULLDOWN_config = {
	   kPORT_PullDown,
	   kPORT_FastSlewRate,
	   kPORT_PassiveFilterDisable,
	   kPORT_OpenDrainDisable,
	   kPORT_LowDriveStrength,
	   kPORT_MuxAsGpio,
};
//uint32_t* debug;

/**********************************************************************************************************************
*  @brief      摄像头引脚初始化
*  @return     void
*  @since      v1.0
*  Sample usage:          camera_port_initialize();
**********************************************************************************************************************/
void camera_port_initialize()
{
	/*以下八根并行数据线初始化*/
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 0, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 1, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 2, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 3, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 4, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 5, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 6, kPORT_MuxAsGpio);
	PORT_SetPinMux(DATALINE_PORT, DATALINE_PORT_NUM + 7, kPORT_MuxAsGpio);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 0, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 1, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 2, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 3, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 4, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 5, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 6, &Port_PULLDOWN_config);
	PORT_SetPinConfig(DATALINE_PORT, DATALINE_PORT_NUM + 7, &Port_PULLDOWN_config);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 0, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 1, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 2, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 3, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 4, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 5, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 6, &Input_port);
	GPIO_PinInit(DATALINE_GPIO, DATALINE_PORT_NUM + 7, &Input_port);
	/*以上八根并行数据线初始化*/
	/*以下场中断线初始化*/
	//DisableIRQ(VSY_IRQn);//关场中断
	PORT_SetPinConfig(VSY_PORT, VSY_PIN, &Port_PULLDOWN_config);//设置引脚功能
	//PORT_SetPinInterruptConfig(VSY_PORT, VSY_PIN, kPORT_InterruptFallingEdge);//设置中断
	GPIO_PinInit(VSY_GPIO, VSY_PIN, &Input_port);//设置方向输入（默认便是输入，不必要）
	//NVIC_SetPriority(VSY_IRQn, VSY_Prio);//中断优先级，优先级要高一点
	//EnableIRQ(VSY_IRQn);//开场中断
	/*以上场中断线初始化*/
	/*以下像素中断线初始化*/
	DisableIRQ(PIX_DMA_CHANNEL_IRQn);
	PORT_SetPinConfig(PIX_PORT, PIX_PIN, &Port_PULLDOWN_config);//设置引脚功能
	PORT_SetPinInterruptConfig(PIX_PORT, PIX_PIN, kPORT_DMARisingEdge);//设置中断，dma中断
	GPIO_PinInit(PIX_GPIO, PIX_PIN, &Input_port);//设置方向输入（默认便是输入，不必要）
	NVIC_SetPriority(PIX_DMA_CHANNEL_IRQn, PIX_DMA_Prio);//中断优先级，优先级要高一点
	//EnableIRQ(PIX_DMA_CHANNEL_IRQn);
	/*以上像素中断线初始化*/
}

/**********************************************************************************************************************
*  @brief      MT9V032摄像头初始化
*  @return     void
*  @since      v1.0
*  Sample usage:          camera_init_MT9V032();
**********************************************************************************************************************/
int camera_init_MT9V032(void)
{
	CameraON = false;
	CameraInitialized = false;
	/*设置参数    具体请参看使用手册*/
	camera_uart_init();	                 //初始换串口 配置摄像头    
	systick_delay_ms(50);        //等待摄像头上电初始化成功
	int16_t temp = 0;
	uint32_t delay_times = 0;
	uint8_t RXtemp = 0;
	uint8_t  send_buffer[4];
	for (int i = 0; i < CONFIG_FINISH; i++)//开始配置摄像头并重新初始化
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = MT9V032_CFG[i][0];
		temp = MT9V032_CFG[i][1];
		send_buffer[2] = temp >> 8;
		send_buffer[3] = (uint8_t)temp;
		UART_WriteBlocking(CAMERA_UART, send_buffer, 4);
		systick_delay_ms(1);
	}
	while (!(RXtemp == 0xA5 || RXtemp == 0xff))
	{
		while (!CAMERA_UART->RCFIFO)//fifo为空
		{
			delay_times++;
			systick_delay_ms(1);
			if (delay_times >= 1000)
			{
				PRINTF("time out! Did not receive a reply from camera\r\n!");
				return -1000;
			}
		}
		RXtemp = CAMERA_UART->D;
	}
	//利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
	//获取配置便于查看配置是否正确
	camera_config_get();
	//检查配置是否一致
	PRINTF("camera AUTO_EXP	= %d\n", (int)GET_CFG[AUTO_EXP - 1][1]);
	PRINTF("camera EXP_TIME	= %d\n", (int)GET_CFG[EXP_TIME - 1][1]);
	PRINTF("camera FPS		= %d\n", (int)GET_CFG[FPS - 1][1]);
	PRINTF("camera SET_COL	= %d\n", (int)GET_CFG[SET_COL - 1][1]);
	PRINTF("camera SET_ROW	= %d\n", (int)GET_CFG[SET_ROW - 1][1]);
	PRINTF("camera LR_OFFSET	= %d\n", (int)GET_CFG[LR_OFFSET - 1][1]);
	PRINTF("camera UD_OFFSET	= %d\n", (int)GET_CFG[UD_OFFSET - 1][1]);
	PRINTF("camera GAIN		= %d\n", (int)GET_CFG[GAIN - 1][1]);
	if ((MT9V032_CFG[SET_COL - 1][1] != GET_CFG[SET_COL - 1][1])
		|| (MT9V032_CFG[SET_ROW - 1][1] != GET_CFG[SET_ROW - 1][1]))
	{
		PRINTF("camera cfg error\r\n!");
		return -1001;
	}
	

	/*初始化摄像头所有引脚*/
	camera_port_initialize();


	/* Configure DMAMUX DMA触发源路由 */
	if (if_DMAMUX_Init == 0)
	{
		if_DMAMUX_Init = 1;
		DMAMUX_Init(DMAMUX_SELECT);
	}
#if defined(FSL_FEATURE_DMAMUX_HAS_A_ON) && FSL_FEATURE_DMAMUX_HAS_A_ON//无视此行
	DMAMUX_EnableAlwaysOn(DMAMUX_SELECT, DMA_CHANNEL_SELECT, true);//无视此行
#else//无视此行
	DMAMUX_SetSource(DMAMUX_SELECT, DMA_CHANNEL_SELECT, DMAPORTCode);
	//信号选择
	//参数一没得选，参数二DMA通道（共“FSL_FEATURE_DMAMUX_MODULE_CHANNEL”个：32个）
	//参数三DMA触发源
#endif /* FSL_FEATURE_DMAMUX_HAS_A_ON *///无视此行
	DMAMUX_EnableChannel(DMAMUX_SELECT, DMA_CHANNEL_SELECT);//启动DMA通道0，按DMAMUX_SetSource设置

	EDMA_GetDefaultConfig(&userConfig);//获取默认配置
	if (if_EDMA_Init == 0)
	{
		if_EDMA_Init = 1;
		EDMA_Init(DMA_SELECT, &userConfig);//初始化
	}
	EDMA_SetBandWidth(DMA_SELECT, DMA_CHANNEL_SELECT, kEDMA_BandwidthStall8Cycle);//休息
	//EDMA_SetChannelPreemptionConfig(DMA_SELECT,0u,&ecpc);//优先级很高
	EDMA_EnableAutoStopRequest(DMA_SELECT, DMA_CHANNEL_SELECT, true);
	EDMA_CreateHandle(&Camera_Mission_EDMA_Handle, DMA_SELECT, DMA_CHANNEL_SELECT);//句柄存有使用那个通道的信息
	EDMA_SetCallback(&Camera_Mission_EDMA_Handle, camera_EDMA_Callback, NULL);//回调函数设置
	EDMA_PrepareTransfer(&transferToBuffer0Config, (void*)(uint32_t)(DATALINE_LOCATION), 1,
		image_Buffer_0, 1, 1, COL * ROW, kEDMA_PeripheralToMemory);//写到缓存0
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
	EDMA_PrepareTransfer(&transferToBuffer1Config, (void*)(uint32_t)(DATALINE_LOCATION), 1,
		image_Buffer_1, 1, 1, COL * ROW, kEDMA_PeripheralToMemory);//写到缓存1
	image_Buffer_index = 0;
#endif
	port_irq_init(VSY_PORT, VSY_PIN, kPORT_InterruptFallingEdge, 0, camera_VSYNC);
	EnableIRQ(PIX_DMA_CHANNEL_IRQn);//开像素中断/DMA触发
	CameraON = true;
	CameraInitialized = true;
	EnableInterrupts;
	return 0;
}


/**********************************************************************************************************************
*  @brief      MT9V032摄像头初始化
*  @return     void
*  @since      v1.0
*  Sample usage:          camera_init_MT9V032();
**********************************************************************************************************************/
extern void dis_bmp(uint16_t high, uint16_t width, uint8_t* p, uint8_t value);

void camera_VSYNC(void)
{
	port_irq_deinit(VSY_PORT, VSY_PIN);//关闭场中断，将在采集完成之后开启
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
	if (image_Buffer_index == 0)//如果缓存0有效，提交缓存1
	{
		EDMA_SubmitTransfer(&Camera_Mission_EDMA_Handle, &transferToBuffer1Config);
	}
	else
	{
		EDMA_SubmitTransfer(&Camera_Mission_EDMA_Handle, &transferToBuffer0Config);
	}
#else
	EDMA_SubmitTransfer(&Camera_Mission_EDMA_Handle, &transferToBuffer0Config);
#endif
	EDMA_StartTransfer(&Camera_Mission_EDMA_Handle);//启动
}

void camera_StartTransmittion()
{
	CameraON = true;
	port_irq_init(VSY_PORT, VSY_PIN, kPORT_InterruptFallingEdge, 0, camera_VSYNC);
}



inline void camera_StopTransmittion()
{
	CameraON = false;
	port_irq_deinit(VSY_PORT, VSY_PIN);//关闭场中断，将在采集完成之后开启
}

void show_image_on_oled(uint8_t* image, uint16_t ro, uint16_t co)
{
	image_size_half_forOLED((uint8_t*)image, (uint8_t*)image_size_half, ro, co);
	dis_bmp(ro / 2, co / 2, (uint8_t*)image_size_half, 100);//先显示，完成后再启动下一次中断
}
uint8_t GetAvailableImageBufferIndex()
{
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
	return image_Buffer_index;
#else
	return 0;
#endif
}
///*强制立即更新标志位CameraDoingTransmission，返回仍需要等待采集的像素数目*/
//int UpdateStatusNOW()
//{
//	int t = EDMA_GetRemainingMajorLoopCount(DMA_SELECT, DMA_CHANNEL_SELECT);
//	//获取剩余的DMA循环数
//	if (t != 0)
//	{
//		CameraDoingTransmission = true;
//	}
//	else
//	{//耗尽，一副图像的DMA传输已经完成
//		CameraDoingTransmission = false;
//	}
//	return t;
//}

/**********************************************************************************************************************
*  @brief      MT9V032摄像头DMA完成中断   User callback function for EDMA transfer.
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
************************************************************************************************************************/
void camera_EDMA_Callback(edma_handle_t* handle, void* param, bool transferDone, uint32_t tcds)
{
	if (transferDone)
	{
		EDMA_ClearChannelStatusFlags(DMA_SELECT, DMA_CHANNEL_SELECT, kEDMA_InterruptFlag);
		mt9v032_finish_flag = 1;
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
		image_Buffer_index = image_Buffer_index ^ 0x01;//异或运算
#else
#endif
		if (CameraON)
		{
			port_irq_init(VSY_PORT, VSY_PIN, kPORT_InterruptFallingEdge, 0, camera_VSYNC);
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////以下辅助初始化，不用看///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//void camera_config_handle()
//{
//	/* If new data arrived. */
//	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(CAMERA_UART))
//	{
//		mt9v032_cof_uart_interrupt();
//	}
//
//}
/***********************************************************************************************************************
*  @brief      MT9V032摄像头串口中断函数
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:
//*  @note       该函数在ISR文件 串口3中断程序被调用
//************************************************************************************************************************/
//void mt9v032_cof_uart_interrupt(void)
//{
//	uart_getchar(CAMERA_UART, &receive[receive_num]);;
//	receive_num++;
//
//	if (1 == receive_num && 0XA5 != receive[0])  receive_num = 0;
//	if (3 == receive_num)
//	{
//		receive_num = 0;
//		uart_receive_flag = 1;
//	}
//
//}

/***********************************************************************************************************************
*  @brief      获取摄像头内部配置信息
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:				调用该函数前请先初始化uart2
************************************************************************************************************************/
void camera_config_get(void)
{
	int16_t temp, i;
	uint8_t  send_buffer[4];
	uint8_t tempppp = 0;
	uint8_t txlen;
	uint8_t rxlen;
	while (CAMERA_UART->RCFIFO)//fifo为空
	{
		tempppp = CAMERA_UART->D;
	}
	for (i = 0; i < CONFIG_FINISH - 1; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = GET_STATUS;
		temp = GET_CFG[i][0];
		send_buffer[2] = temp >> 8;
		send_buffer[3] = (uint8_t)temp;
		txlen = 4;
		rxlen = 3;
		while (txlen || rxlen)
		{
			if ((CAMERA_UART->RCFIFO) && rxlen != 0)
			{
				receive[3 - rxlen] = CAMERA_UART->D;
				if (receive[0] == 0xa5) {
					rxlen--;
				}
			}
			if ((CAMERA_UART->S1 & UART_S1_TDRE_MASK) && txlen != 0)
			{
				CAMERA_UART->D = send_buffer[4 - txlen];
				txlen--;
			}
		}
		//UART_WriteBlocking(CAMERA_UART, send_buffer, 4);
		//UART_ReadBlocking(CAMERA_UART, receive, 3);
		GET_CFG[i][1] = receive[1] << 8 | receive[2];
		//PRINTF("b0=%#x b1=%#x b2=%#x\n", receive[0], receive[1], receive[2]);
		//PRINTF("%d\n", (int)GET_CFG[i][1]);
	}
}

/***********************************************************************************************************************
*  @brief      获取摄像头固件版本
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:				调用该函数前请先初始化uart2
************************************************************************************************************************/
//uint16_t get_version(void)
//{
//	uint16_t temp;
//	uint8_t  send_buffer[4];
//	send_buffer[0] = 0xA5;
//	send_buffer[1] = GET_STATUS;
//	temp = GET_VERSION;
//	send_buffer[2] = temp >> 8;
//	send_buffer[3] = (uint8_t)temp;
//
//	UART_WriteBlocking(CAMERA_UART, send_buffer, 4);
//
//	uart_receive_flag = 1;  //张亚楼组添加
//
//	//等待接受回传数据
//	while (!uart_receive_flag);
//	uart_receive_flag = 0;
//
//	return ((uint16_t)(receive[1] << 8) | receive[2]);
//}

/***********************************************************************************************************************
*  @brief      单独设置摄像头曝光时间
*  @param      light   设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
*  @return     uint16_t  当前曝光值，用于确认是否正确写入
*  @since      v1.0
*  Sample usage:
************************************************************************************************************************/
//uint16_t set_exposure_time(uint16_t light)
//{
//	uint16_t temp;
//	uint8_t  send_buffer[4];
//
//	send_buffer[0] = 0xA5;
//	send_buffer[1] = SET_EXP_TIME;
//	temp = light;
//	send_buffer[2] = temp >> 8;
//	send_buffer[3] = (uint8_t)temp;
//
//	UART_WriteBlocking(CAMERA_UART, send_buffer, 4);
//
//
//	//等待接受回传数据
//	while (!uart_receive_flag);
//	uart_receive_flag = 0;
//
//	temp = receive[1] << 8 | receive[2];
//	return temp;
//
//}

