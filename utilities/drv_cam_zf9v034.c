#include <drv_cam_zf9v034.h>

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))

/*!
 * @addtogroup cam_zf9v034
 * @{
 */

#define SYSLOG_TAG  ("ZF9V034")
#define SYSLOG_LVL  (HITSIC_ZF9V034_LOG_LVL)
#include <inc_syslog.h>

uint16_t camera_version;

void CAM_ZF9V034_GetDefaultConfig(cam_zf9v034_configPacket_t *config)
{
    SYSLOG_I("Generate ZF9V034 config data.");
	config->autoExpSetCmd = (uint16_t)cam_zf9v034_cmd_t::AUTO_EXP;
	config->autoExpSet = 0;
    config->autoExpTimeCmd = (uint16_t)cam_zf9v034_cmd_t::EXP_TIME;
	config->autoExpTime = 32222;
    config->FrameRateCmd = (uint16_t)cam_zf9v034_cmd_t::FPS;
	config->FrameRate = 50;
    config->imageColCmd = (uint16_t)cam_zf9v034_cmd_t::SET_COL;
	config->imageCol = 188;
    config->imageRowCmd = (uint16_t)cam_zf9v034_cmd_t::SET_ROW;
	config->imageRow = 120;
    config->HorizontalOffsetCmd = (uint16_t)cam_zf9v034_cmd_t::LR_OFFSET;
	config->HorizontalOffset = 0;
    config->VerticalOffsetCmd = (uint16_t)cam_zf9v034_cmd_t::UD_OFFSET;
	config->VerticalOffset = 0;
    config->imageGainCmd = (uint16_t)cam_zf9v034_cmd_t::GAIN;
	config->imageGain = 32;
    config->initCmd = (uint16_t)cam_zf9v034_cmd_t::INIT;
	config->dummyData0 = 0;
}

void CAM_ZF9V034_CfgWrite(const cam_zf9v034_configPacket_t *config)
{
    SYSLOG_I("Config write begin.");
	uint8_t camera_uartTxBuf[8];
	uint16_t* configData = (uint16_t*)config;
	//CAM_ZF9V034_Delay_ms(200);        //等待摄像头上电初始化成功

	for (uint8_t i = 0; i < (uint16_t)cam_zf9v034_cmd_t::CONFIG_FINISH; ++i)//开始配置摄像头并重新初始化
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = configData[(i << 1) + 0];
		camera_uartTxBuf[2] = (uint8_t)(configData[(i << 1) + 1] >> 8);
		camera_uartTxBuf[3] = (uint8_t)(configData[(i << 1) + 1]);
		CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
		CAM_ZF9V034_Delay_ms(2);
	}
	SYSLOG_I("Config write end. Waiting target ready.");
	do {
	    SYSLOG_D("Attempt to get response.");
	    camera_uartTxBuf[0] = 0;
		if(kStatus_Success != CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 1))
		{
		    SYSLOG_W("Failed to get response.");
		}
	} while ((0xff != camera_uartTxBuf[0]));//TODO: 增加次数限制
	//以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
	//利用camera_setEpTime函数单独配置的曝光数据不存储在eeprom中
	SYSLOG_I("Config write complete.");
}

void CAM_ZF9V034_CfgRead(cam_zf9v034_configPacket_t *config)
{
    SYSLOG_I("Config Read begin.");
	uint8_t camera_uartTxBuf[8];
	uint16_t* configData = (uint16_t*)config;

	for (uint8_t i = 0; i < (uint16_t)cam_zf9v034_cmd_t::CONFIG_FINISH - 1; ++i)
	{
		camera_uartTxBuf[0] = 0xA5;
		camera_uartTxBuf[1] = (uint8_t)((uint16_t)cam_zf9v034_cmd_t::GET_STATUS);
		camera_uartTxBuf[2] = (uint8_t)(configData[(i << 1) + 0] >> 8);
		camera_uartTxBuf[3] = (uint8_t)(configData[(i << 1) + 0]);

		CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
		CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
		configData[(i << 1) + 1] = camera_uartTxBuf[1] << 8 | camera_uartTxBuf[2];
	}
	SYSLOG_I("Config Read complete.");
}

//get camera hardware version
uint16_t CAM_ZF9V034_GetVersion(void)
{
    uint8_t camera_uartTxBuf[8];

	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = ((uint16_t)cam_zf9v034_cmd_t::GET_STATUS);
	camera_uartTxBuf[2] = (uint8_t)((uint16_t)cam_zf9v034_cmd_t::GET_VERSION >> 8);
	camera_uartTxBuf[3] = (uint8_t)((uint16_t)cam_zf9v034_cmd_t::GET_VERSION);

	CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
	CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
	return camera_version = ((uint16_t)(camera_uartTxBuf[1] << 8) | camera_uartTxBuf[2]);
}

//set exposure time
uint16_t CAM_ZF9V034_SetExposeTime(uint16_t light)
{
    uint8_t camera_uartTxBuf[8];

	camera_uartTxBuf[0] = 0xA5;
	camera_uartTxBuf[1] = (uint8_t)cam_zf9v034_cmd_t::SET_EXP_TIME;
	camera_uartTxBuf[2] = (uint8_t)(light >> 8);
	camera_uartTxBuf[3] = (uint8_t)light;

	CAM_ZF9V034_UartTxBlocking(camera_uartTxBuf, 4);
	CAM_ZF9V034_UartRxBlocking(camera_uartTxBuf, 3);
	return ((uint16_t)(camera_uartTxBuf[1] << 8) | camera_uartTxBuf[2]);
}



// Receiver Config
#if (defined(ZF9V034_USE_DMADVP) && (ZF9V034_USE_DMADVP > 0U))
void CAM_ZF9V034_GetReceiverConfig(receiver_config_type *config, const cam_zf9v034_configPacket_t *camConfig)
{
    SYSLOG_I("Generate DMADVP config data.");
	config->width = camConfig->imageCol;
	config->height = camConfig->imageRow;
	config->bytesPerPixel = 1U;
	config->polarityFlags = DMADVP_HsyncActiveHigh/*FIXME*/ | DMADVP_VsyncActiveLow | DMADVP_DataLatchOnRisingEdge;
}
#elif(defined(ZF9V034_USE_RTCSI) && (ZF9V034_USE_RTCSI > 0U))
void CAM_ZF9V034_GetReceiverConfig(receiver_config_type *config, const cam_zf9v034_configPacket_t *camConfig); //FIXME
#endif // ! Receiver Config

/* @} */


#endif // ! HITSIC_USE_CAM_ZF9V034
