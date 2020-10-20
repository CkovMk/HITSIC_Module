#include "drv_cam_zf9v034.hpp"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))

//uint8_t camera_uartRxBuf[8];
//uint8_t camera_uartTxBuf[8];
uint16_t camera_version;

////需要配置到摄像头的数据
//int16_t camera_cfg[(int16_t)CAM_CMD_CONFIG_FINISH][2] =
//{
//  {(int16_t)CAM_CMD_AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
//  //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
//  {(int16_t)CAM_CMD_EXP_TIME,          32222}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
//  {(int16_t)CAM_CMD_FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
//  {(int16_t)CAM_CMD_SET_COL,           CAM_IMG_C}, //图像列数量        范围1-752     K60采集不允许超过188
//  {(int16_t)CAM_CMD_SET_ROW,           CAM_IMG_R}, //图像行数量        范围1-480
//  {(int16_t)CAM_CMD_LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//  {(int16_t)CAM_CMD_UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//  {(int16_t)CAM_CMD_GAIN,              32},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
//
//
//  {(int16_t)CAM_CMD_INIT,              0}    //摄像头开始初始化
//};
////从摄像头内部获取到的配置数据
//int16_t get_camera_cfg[(int16_t)CAM_CMD_CONFIG_FINISH - 1][2] =
//{
//  {(int16_t)CAM_CMD_AUTO_EXP,          0},   //自动曝光设置
//  {(int16_t)CAM_CMD_EXP_TIME,          0},   //曝光时间
//  {(int16_t)CAM_CMD_FPS,               0},   //图像帧率
//  {(int16_t)CAM_CMD_SET_COL,           0},   //图像列数量
//  {(int16_t)CAM_CMD_SET_ROW,           0},   //图像行数量
//  {(int16_t)CAM_CMD_LR_OFFSET,         0},   //图像左右偏移量
//  {(int16_t)CAM_CMD_UD_OFFSET,         0},   //图像上下偏移量
//  {(int16_t)CAM_CMD_GAIN,              0},   //图像增益
//};

void CAM_ZF9V034_GetDefaultConfig(cam_zf9v034_configPacket_t *config)
{
	config->autoExpSetCmd = CAM_CMD_AUTO_EXP;
	config->autoExpSet = 0;
    config->autoExpTimeCmd = CAM_CMD_EXP_TIME;
	config->autoExpTime = 32222;//FIXME: check this value
    config->FrameRateCmd = CAM_CMD_FPS;
	config->FrameRate = 50;
    config->imageColCmd = CAM_CMD_SET_COL;
	config->imageCol = 188;
    config->imageRowCmd = CAM_CMD_SET_ROW;
	config->imageRow = 120;
    config->HorizontalOffsetCmd = CAM_CMD_LR_OFFSET;
	config->HorizontalOffset = 0;
    config->VerticalOffsetCmd = CAM_CMD_UD_OFFSET;
	config->VerticalOffset = 0;
    config->imageGainCmd = CAM_CMD_GAIN;
	config->imageGain = 32;
    config->initCmd = CAM_CMD_INIT;
	config->dummyData0 = 0;
}

void CAM_ZF9V034_CfgWrite(const cam_zf9v034_configPacket_t *config)
{
	uint8_t camera_uartTxBuf[8];
	uint16_t* configData = (uint16_t*)config;
	CAM_ZF9V034_Delay_ms(200);        //等待摄像头上电初始化成功

	for (uint8_t i = 0; i < (int16_t)(CAM_CMD_CONFIG_FINISH); ++i)//开始配置摄像头并重新初始化
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = configData[(i << 1) + 0];
		camera_uartTxBuf[2] = (uint8_t)(configData[(i << 1) + 1] >> 8);
		camera_uartTxBuf[3] = (uint8_t)(configData[(i << 1) + 1]);
		CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
	}

	do {
		CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 1);
	} while ((0xff != camera_uartTxBuf[0]));
	//以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
	//利用camera_setEpTime函数单独配置的曝光数据不存储在eeprom中
}

void CAM_ZF9V034_CfgRead(cam_zf9v034_configPacket_t *config)
{
	uint8_t camera_uartTxBuf[8];
	uint16_t* configData = (uint16_t*)config;

	for (uint8_t i = 0; i < (int16_t)(CAM_CMD_CONFIG_FINISH) - 1; ++i)
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = (uint8_t)(CAM_CMD_GET_STATUS);
		camera_uartTxBuf[2] = (uint8_t)(configData[(i << 1) + 0] >> 8);
		camera_uartTxBuf[3] = (uint8_t)(configData[(i << 1) + 0]);

		CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
		CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
		configData[(i << 1) + 1] = camera_uartTxBuf[1] << 8 | camera_uartTxBuf[2];
	}
}

//get camera hardware version
uint16_t CAM_ZF9V034_GetVersion(void)
{
    uint8_t camera_uartTxBuf[8];

	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = (int16_t)(CAM_CMD_GET_STATUS);
	camera_uartTxBuf[2] = (uint8_t)((int16_t)CAM_CMD_GET_VERSION >> 8);
	camera_uartTxBuf[3] = (uint8_t)((int16_t)CAM_CMD_GET_VERSION);

	CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
	CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
	return camera_version = ((uint16_t)(camera_uartTxBuf[1] << 8) | camera_uartTxBuf[2]);
}

//set exposure time
uint16_t CAM_ZF9V034_SetExposeTime(uint16_t light)
{
    uint8_t camera_uartTxBuf[8];

	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = (int16_t)(CAM_CMD_SET_EXP_TIME);
	camera_uartTxBuf[2] = (uint8_t)(light >> 8);
	camera_uartTxBuf[3] = (uint8_t)light;

	CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
	CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
	return ((uint16_t)(camera_uartTxBuf[1] << 8) | camera_uartTxBuf[2]);
}



// Receiver Config
#if (defined(ZF9V034_USE_DMADVP) && (ZF9V034_USE_DMADVP > 0U))
void CAM_ZF9V034_GetReceiverConfig(dmadvp_config_t* config, const cam_zf9v034_configPacket_t *camConfig)
{
	config->width = camConfig->imageCol;
	config->height = camConfig->imageRow;
	config->bytesPerPixel = 1U;
	config->polarityFlags = DMADVP_HsyncActiveHigh/*FIXME*/ | DMADVP_VsyncActiveHigh | DMADVP_DataLatchOnRisingEdge;
}
#elif(defined(ZF9V034_USE_RTCSI) && (ZF9V034_USE_RTCSI > 0U))
void CAM_ZF9V034_GetReceiverConfig(csi_config_t* config, const cam_zf9v034_configPacket_t *camConfig); //FIXME
#endif // ! Receiver Config



#endif // ! HITSIC_USE_CAM_ZF9V034
