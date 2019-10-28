/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		SD卡函数
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛qq1761690868
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		使用时先调用sd_init()初始化，并将SmartCar_Sd.h中#define _SD_DISABLE_解注释，存图时只需调用sd_image_save()即可，不需要存图功能则将#define _SD_DISABLE_注释即可
		
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#include  "SmartCar_Sd.h"
#include "fsl_debug_console.h"

/**********************************************************************************************************************
*  @brief      SD初始化
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:          sd_init();
**********************************************************************************************************************/
void sd_init(void)
{
#ifndef _SD_DISABLE_
        bool isReadOnly;
	sd_save_flag = 0;
	sd_initPins();
	sd_card_t* card = &smartcar_sd;
	/* 板子引脚、时钟、调试输出初始化Board pin, clock, debug console init */
	label_sd = creat_random_num();
	SYSMPU_Enable(SYSMPU, false);//注释见清旭
	card->host.base = SD_HOST_BASEADDR;
	card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;
	//card->host.sourceClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);
	/* card detect type */
	card->usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
	card->usrParam.pwr = &s_sdCardPwrCtrl;
#endif
	/* SD host init function */
	if (SD_HostInit(card) != kStatus_Success)
	{
		PRINTF("\r\nSD host init fail\r\n");
		return;
	}
	PRINTF("\r\nPlease insert a card into board.\r\n");
	SD_PowerOffCard(card->host.base, card->usrParam.pwr);
	if (SD_CardInit(card))
	{
		PRINTF("\r\nCard don`t inserted.\r\n");
		sd_inserted_flag = 0;
		PRINTF("\r\nSD card init failed.\r\n");
		return;
	}
	else
	{
		sd_inserted_flag = 1;
	}
	if (sd_inserted_flag == 1)
	{

		/* 返回sd卡信息 */
		CardInformationLog(card);

		/* 检查sd卡是只读还是可以写 */
		isReadOnly = SD_CheckReadOnly(card);

		PRINTF("\r\nsd card is ready\r\n");
	}
#endif // !_SD_DISABLE_

}

/**********************************************************************************************************************
*  @brief      SD初始化引脚
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:          sd_initPins();
**********************************************************************************************************************/
void sd_initPins(void)
{
#ifndef _SD_DISABLE_

	const port_pin_config_t porte6_pinB3_config = {/* Internal pull-up resistor is enabled */
                                                        kPORT_PullUp,
                                                        /* Fast slew rate is configured */
                                                        kPORT_FastSlewRate,
                                                        /* Passive filter is disabled */
                                                        kPORT_PassiveFilterDisable,
                                                        /* Open drain is disabled */
                                                        kPORT_OpenDrainDisable,
                                                        /* Low drive strength is configured */
                                                        kPORT_LowDriveStrength,
                                                        /* Pin is configured as PTD10 */
                                                        kPORT_MuxAsGpio,
                                                        /* Pin Control Register fields [15:0] are not locked */
                                                        kPORT_UnlockRegister };
	/* PORTD10 (pin B3) is configured as PTD10 */
	PORT_SetPinConfig(PORTE, 6U, &porte6_pinB3_config);//cigarend

	const port_pin_config_t porte0_pinD3_config = {/* Internal pull-up resistor is enabled */
                                                       kPORT_PullUp,
                                                       /* Fast slew rate is configured */
                                                       kPORT_FastSlewRate,
                                                       /* Passive filter is disabled */
                                                       kPORT_PassiveFilterDisable,
                                                       /* Open drain is disabled */
                                                       kPORT_OpenDrainDisable,
                                                       /* Low drive strength is configured */
                                                       kPORT_LowDriveStrength,
                                                       /* Pin is configured as SDHC0_D1 */
                                                       kPORT_MuxAlt4,
                                                       /* Pin Control Register fields [15:0] are not locked */
                                                       kPORT_UnlockRegister };
	/* PORTE0 (pin D3) is configured as SDHC0_D1 */
	PORT_SetPinConfig(PORTE, 0U, &porte0_pinD3_config);

	const port_pin_config_t porte1_pinD2_config = {/* Internal pull-up resistor is enabled */
                                                       kPORT_PullUp,
                                                       /* Fast slew rate is configured */
                                                       kPORT_FastSlewRate,
                                                       /* Passive filter is disabled */
                                                       kPORT_PassiveFilterDisable,
                                                       /* Open drain is disabled */
                                                       kPORT_OpenDrainDisable,
                                                       /* Low drive strength is configured */
                                                       kPORT_LowDriveStrength,
                                                       /* Pin is configured as SDHC0_D0 */
                                                       kPORT_MuxAlt4,
                                                       /* Pin Control Register fields [15:0] are not locked */
                                                       kPORT_UnlockRegister };
	/* PORTE1 (pin D2) is configured as SDHC0_D0 */
	PORT_SetPinConfig(PORTE, 1U, &porte1_pinD2_config);

	const port_pin_config_t porte2_pinD1_config = {/* Internal pull-up resistor is enabled */
                                                         kPORT_PullUp,
                                                         /* Fast slew rate is configured */
                                                         kPORT_FastSlewRate,
                                                         /* Passive filter is disabled */
                                                         kPORT_PassiveFilterDisable,
                                                         /* Open drain is disabled */
                                                         kPORT_OpenDrainDisable,
                                                         /* Low drive strength is configured */
                                                         kPORT_LowDriveStrength,
                                                         /* Pin is configured as SDHC0_DCLK */
                                                         kPORT_MuxAlt4,
                                                         /* Pin Control Register fields [15:0] are not locked */
                                                         kPORT_UnlockRegister };
	/* PORTE2 (pin D1) is configured as SDHC0_DCLK */
	PORT_SetPinConfig(PORTE, 2U, &porte2_pinD1_config);

	const port_pin_config_t porte3_pinE4_config = {/* Internal pull-up resistor is enabled */
                                                         kPORT_PullUp,
                                                         /* Fast slew rate is configured */
                                                         kPORT_FastSlewRate,
                                                         /* Passive filter is disabled */
                                                         kPORT_PassiveFilterDisable,
                                                         /* Open drain is disabled */
                                                         kPORT_OpenDrainDisable,
                                                         /* Low drive strength is configured */
                                                         kPORT_LowDriveStrength,
                                                         /* Pin is configured as SDHC0_CMD */
                                                         kPORT_MuxAlt4,
                                                         /* Pin Control Register fields [15:0] are not locked */
                                                         kPORT_UnlockRegister };
	/* PORTE3 (pin E4) is configured as SDHC0_CMD */
	PORT_SetPinConfig(PORTE, 3U, &porte3_pinE4_config);

	const port_pin_config_t porte4_pinE3_config = {/* Internal pull-up resistor is enabled */
                                                         kPORT_PullUp,
                                                         /* Fast slew rate is configured */
                                                         kPORT_FastSlewRate,
                                                         /* Passive filter is disabled */
                                                         kPORT_PassiveFilterDisable,
                                                         /* Open drain is disabled */
                                                         kPORT_OpenDrainDisable,
                                                         /* Low drive strength is configured */
                                                         kPORT_LowDriveStrength,
                                                         /* Pin is configured as SDHC0_D3 */
                                                         kPORT_MuxAlt4,
                                                         /* Pin Control Register fields [15:0] are not locked */
                                                         kPORT_UnlockRegister };
	/* PORTE4 (pin E3) is configured as SDHC0_D3 */
	PORT_SetPinConfig(PORTE, 4U, &porte4_pinE3_config);

	const port_pin_config_t porte5_pinE2_config = {/* Internal pull-up resistor is enabled */
                                                         kPORT_PullUp,
                                                         /* Fast slew rate is configured */
                                                         kPORT_FastSlewRate,
                                                         /* Passive filter is disabled */
                                                         kPORT_PassiveFilterDisable,
                                                         /* Open drain is disabled */
                                                         kPORT_OpenDrainDisable,
                                                         /* Low drive strength is configured */
                                                         kPORT_LowDriveStrength,
                                                         /* Pin is configured as SDHC0_D2 */
                                                         kPORT_MuxAlt4,
                                                         /* Pin Control Register fields [15:0] are not locked */
                                                         kPORT_UnlockRegister };
	/* PORTE5 (pin E2) is configured as SDHC0_D2 */
	PORT_SetPinConfig(PORTE, 5U, &porte5_pinE2_config);
#endif // !_SD_DISABLE_

}

/**********************************************************************************************************************
*  @brief      存图到SD卡
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:          sd_image_save(image_Buffer_0);
**********************************************************************************************************************/
void sd_image_save(uint8_t(_IMG)[COL])
{
#ifndef _SD_DISABLE_
	if (sd_inserted_flag == 1)
	{
		if (sd_save_flag == 0)
		{
			if (num == 50000)
			{
				return;
			}
			if (num == 0)
			{
				smart_buff[DATA_BUFFER_SIZE - 2] = 0x22;
				smart_buff[DATA_BUFFER_SIZE - 1] = 0x33;
			}
			else if (num == 1)
			{
				smart_buff[DATA_BUFFER_SIZE - 2] = *((uint8*)(&label_sd));
				smart_buff[DATA_BUFFER_SIZE - 1] = *((uint8*)(&label_sd) + 1);
			}
			memcpy(smart_buff, _IMG, COL * ROW);
			sd_save_flag = 1;
		}
	}
	sd_image_save_start();
#endif // !_SD_DISABLE_
}



void sd_image_save_start()
{
#ifndef _SD_DISABLE_
	if (sd_inserted_flag == 1)
	{
		if (sd_save_flag == 1)
		{
			PRINTF("Write/read multiple data blocks......\r\n");
			if (kStatus_Success != SD_WriteBlocks(&smartcar_sd, smart_buff, DATA_BLOCK_START + num * DATA_BLOCK_COUNT, DATA_BLOCK_COUNT))
			{
				PRINTF("Write multiple data blocks failed.\r\n");
				if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, false) == kStatus_Success)
				{
					PRINTF("\r\nCard removed\r\n");
				}
				///* access card fail, due to transfer error */
			}
			else
			{
				num++;
				sd_save_flag = 0;
			}
		}
	}
#endif // !_SD_DISABLE_
}

uint32_t creat_random_num(void)
{
#ifndef _SD_DISABLE_
	//配置RNG时钟
	SIM->SCGC3 |= (SIM_SCGC3_RNGA_MASK);
	RNG->CR |= (RNG_CR_GO_MASK);

	uint16_t retry = 0xff;
	while (((RNG->SR >> RNG_SR_OREG_LVL_SHIFT) & 0x0001) == 0 && retry--);
	if (retry)
		return RNG->OR;
	else
		return 0x2233;
#endif // !_SD_DISABLE_

}

//读取sd卡信息
static void CardInformationLog(sd_card_t * card)
{
#ifndef _SD_DISABLE_
	assert(card);

	PRINTF("\r\nCard size %d * %d bytes\r\n", card->blockCount, card->blockSize);
	PRINTF("\r\nWorking condition:\r\n");
	if (card->operationVoltage == kCARD_OperationVoltage330V)
	{
		PRINTF("\r\n  Voltage : 3.3V\r\n");
	}
	else if (card->operationVoltage == kCARD_OperationVoltage180V)
	{
		PRINTF("\r\n  Voltage : 1.8V\r\n");
	}

	if (card->currentTiming == kSD_TimingSDR12DefaultMode)
	{
		if (card->operationVoltage == kCARD_OperationVoltage330V)
		{
			PRINTF("\r\n  Timing mode: Default mode\r\n");
		}
		else if (card->operationVoltage == kCARD_OperationVoltage180V)
		{
			PRINTF("\r\n  Timing mode: SDR12 mode\r\n");
		}
	}
	else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
	{
		if (card->operationVoltage == kCARD_OperationVoltage180V)
		{
			PRINTF("\r\n  Timing mode: SDR25\r\n");
		}
		else
		{
			PRINTF("\r\n  Timing mode: High Speed\r\n");
		}
	}
	else if (card->currentTiming == kSD_TimingSDR50Mode)
	{
		PRINTF("\r\n  Timing mode: SDR50\r\n");
	}
	else if (card->currentTiming == kSD_TimingSDR104Mode)
	{
		PRINTF("\r\n  Timing mode: SDR104\r\n");
	}
	else if (card->currentTiming == kSD_TimingDDR50Mode)
	{
		PRINTF("\r\n  Timing mode: DDR50\r\n");
	}

	PRINTF("\r\n  Freq : %d HZ\r\n", card->busClock_Hz);
#endif // !_SD_DISABLE_
}
