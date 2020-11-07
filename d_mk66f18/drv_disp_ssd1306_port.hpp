/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 *	@by:		SmartCar SJMC
 *	@name:		drvoled.h
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2019.01.15 04:48
 */
#pragma once
#ifndef D_MK66F18_DRVOLED_H_
#define D_MK66F18_DRVOLED_H_
#include "inc_stdlib.hpp"
#include "hitsic_common.h"
#include "sys_pitmgr.hpp"
#include "pin_mux.h"

#define HITSIC_DISP_SSD1306_FLIP_X (0U)	///< 屏幕左右翻转
#define HITSIC_DISP_SSD1306_FLIP_Y (0U)	///< 屏幕上下翻转

#define OLED_SPI_BASE			SPI2
#define OLED_SPI_CLKFREQ 		CLOCK_GetFreq(DSPI0_CLK_SRC)
#define OLED_SPI_Pcsn		    kDSPI_Pcs0
#define OLED_SPI_MasterPcsn		kDSPI_MasterPcs0
#define OLED_SPI_Ctarn			kDSPI_Ctar0
#define OLED_SPI_MasterCtarn	kDSPI_MasterCtar0

/**
 * @brief 设置RST脚电平。
 * 
 * @param x RST脚的电平状态，正逻辑。
 */
inline void DISP_SSD1306_gpioSetRST(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_DIGITAL_OLED_RST_GPIO, RTEPIN_DIGITAL_OLED_RST_PIN, x);
}

/**
 * @brief 设置D/C脚电平。
 * 
 * @param x D/C脚的电平状态，正逻辑。
 */
inline void DISP_SSD1306_gpioSetD_C(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_DIGITAL_OLED_D_C_GPIO, RTEPIN_DIGITAL_OLED_D_C_PIN, x);
}

/**
 * @brief 毫秒级阻塞延迟函数。
 * 
 * @param ms 延迟的毫秒数。
 */
inline void DISP_SSD1306_delay_ms(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; ++i)
	{
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	}
}

/**
 * @brief SPI接口发送一个字节。
 * 
 * @param data 要发送的数据
 */
inline void DISP_SSD1306_spiWrite(uint8_t data)
{
	static dspi_transfer_t oled_spi_xfer =
		{
			.txData = NULL,
			.rxData = NULL,
			.dataSize = 1,
			.configFlags = OLED_SPI_MasterCtarn | OLED_SPI_MasterPcsn | kDSPI_MasterPcsContinuous,
		};
	oled_spi_xfer.txData = &data;
	DSPI_MasterTransferBlocking(OLED_SPI_BASE, &oled_spi_xfer);
}


#endif // ! D_MK66F18_DRVOLED_H_
