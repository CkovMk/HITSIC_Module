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
#include "inc_stdlib.h"
#include "hitsic_common.h"
#include "pin_mux.h"

#define OLED_SPI_BASE			SPI2
#define OLED_SPI_CLKFREQ 		CLOCK_GetFreq(DSPI2_CLK_SRC)
#define OLED_SPI_Pcsn		    kDSPI_Pcs0
#define OLED_SPI_MasterPcsn		kDSPI_MasterPcs0
#define OLED_SPI_Ctarn			kDSPI_Ctar0
#define OLED_SPI_MasterCtarn	kDSPI_MasterCtar0

inline void DISP_SSD1306_gpioSetRST(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_BASIC_OLED_RST_GPIO, RTEPIN_BASIC_OLED_RST_PIN, x);
}

inline void DISP_SSD1306_gpioSetD_C(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_BASIC_OLED_D_C_GPIO, RTEPIN_BASIC_OLED_D_C_PIN, x);
}

inline void DISP_SSD1306_delay_ms(uint32_t ms)
{

}

//内部使用用户无需调用
inline void OLED_SCL(uint8_t data)
{
    if (data)
    {
        GPIO_PortSet(BOARD_OLED_SCL_GPIO, 1U << BOARD_OLED_SCL_GPIO_PIN);
    }
    else
    {
        GPIO_PortClear(BOARD_OLED_SCL_GPIO, 1U << BOARD_OLED_SCL_GPIO_PIN);
    }
}
//内部使用用户无需调用
inline void OLED_SDA(uint8_t data)
{
    if (data)
    {
        GPIO_PortSet(BOARD_OLED_SDA_GPIO, 1U << BOARD_OLED_SDA_GPIO_PIN);
    }
    else
    {
        GPIO_PortClear(BOARD_OLED_SDA_GPIO, 1U << BOARD_OLED_SDA_GPIO_PIN);
    }
}

//extern dspi_master_config_t spi_m_cfg;
//inline void DISP_SSD1306_spiInit(void)
//{
//	const dspi_master_config_t oled_spi_cfg = {
//  		.whichCtar = OLED_SPI_Ctarn,
//  		.ctarConfig = {
//		.baudRate = 20000000,
//		.bitsPerFrame = 8,
//		.cpol = kDSPI_ClockPolarityActiveHigh,
//		.cpha = kDSPI_ClockPhaseFirstEdge,
//		.direction = kDSPI_MsbFirst,
//		.pcsToSckDelayInNanoSec = 1000,
//		.lastSckToPcsDelayInNanoSec = 1000,
//		.betweenTransferDelayInNanoSec = 1000
//  		},
//  	.whichPcs = OLED_SPI_Pcsn,
//  	.pcsActiveHighOrLow = kDSPI_PcsActiveLow,
//  	.enableContinuousSCK = false,
//  	.enableRxFifoOverWrite = false,
//  	.enableModifiedTimingFormat = false,
//  	.samplePoint = kDSPI_SckToSin0Clock,
//	};
//	DSPI_MasterInit(OLED_SPI_BASE, &oled_spi_cfg, OLED_SPI_CLKFREQ);
//}
inline void DISP_SSD1306_spiWrite(uint8_t data)
{

//    static dspi_transfer_t oled_spi_xfer =
//		{
//			.txData = NULL,
//			.rxData = NULL,
//			.dataSize = 1,
//			.configFlags = OLED_SPI_MasterCtarn | OLED_SPI_MasterPcsn | kDSPI_MasterPcsContinuous,
//		};
//	oled_spi_xfer.txData = &data;
//	DSPI_MasterTransferBlocking(OLED_SPI_BASE, &oled_spi_xfer);

    uint8_t i=8;

    DISP_SSD1306_gpioSetD_C(1);
    OLED_SCL(0);
    while(i--)
    {
        if(data&0x80){OLED_SDA(1);}
        else{OLED_SDA(0);}
        OLED_SCL(1);

        OLED_SCL(0);
        data<<=1;
    }

}






#endif // ! D_MK66F18_DRVOLED_H_
