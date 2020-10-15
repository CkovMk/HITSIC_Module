 /**
 * Copyright 2018 - 2019 HITSIC
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
/**
 * @file 	:	drv_edmadvp.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2020.10.13
 *
 * @brief    :   DVP摄像头数据EDMA接收器
 */
#ifndef UTILITIES_DRV_EDMADVP_HPP_
#define UTILITIES_DRV_EDMADVP_HPP_
#include "inc_stdlib.h"
#include "hitsic_common.h"

/** port */
#define HITSIC_USE_EDMADVP (1U) //TODO: REMOVE this after complete
#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

#define EDMADVP_DAT0_GPIO		RTEPIN_BOARD_CAM_D0_GPIO
#define EDMADVP_DAT0_PORT		RTEPIN_BOARD_CAM_D0_PORT
#define EDMADVP_DAT0_PIN		RTEPIN_BOARD_CAM_D0_PIN

#define EDMADVP_DATA_DMA_ADDR		(((uint32_t)GPIO_CAM_D0_GPIO)+0x10+GPIO_CAM_D0_PIN/8)

struct EDMADVP_Type
{
    INTC_Type* vsnc_intc;
    uint32_t vsnc_pin
    INTC_Type* pclk_intc;
    uint32_t pclk_pin;

    uint32_t dmaAddress;
    uint32_t dmaChannel;
    uint32_t dmauxChannel;
    uint32_t dmaIrqn;
    uint32_t dmaIrqPrio;
};

EDMADVP_Type __EDMADVP0 = 
{
    .vsnc_intc = RTEPIN_BOARD_CAM_VSNC_PORT;
    .vsnc_pin = RTEPIN_BOARD_CAM_VSNC_PIN;
    .pclk_intc = RTEPIN_BOARD_CAM_PCLK_PORT;
    .pclk_pin = RTEPIN_BOARD_CAM_PCLK_PIN;

    .dmaAddress = EDMADVP_DATA_DMA_ADDR;
    .dmaChannel = 0U;
    .dmauxChannel = 0U;
    .dmaIrqn = DMA0_DMA16_IRQn;
    .dmaIrqPrio = 2U;
};

static constexpr EDMADVP_Type* EDMADVP0 = &__EDMADVP0;

inline void EDMADVP_Delay_ms(uint32_t ms)
{
	for(int i = 0; i < ms, ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

#endif // ! HITSIC_USE_EDMADVP
/** ! port */

#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

#define EDMADVP_VSNC_IRQ_ENABLE() (PORT_SetPinInterruptConfig(EDMADVP_VSNC_PORT, EDMADVP_VSNC_PIN, kPORT_InterruptFallingEdge))
#define EDMADVP_VSNC_IRQ_DISABLE() (PORT_SetPinInterruptConfig(EDMADVP_VSNC_PORT, EDMADVP_VSNC_PIN, kPORT_InterruptOrDMADisabled))

#define EDMADVP_PCLK_IRQ_ENABLE()	(EnableIRQ(EDMADVP_PCLK_DMA_IRQn))
#define EDMADVP_PCLK_IRQ_DISABLE()	(DisableIRQ(EDMADVP_PCLK_DMA_IRQn))

struct edmadvp_xferHandle_t
{//TODO: update this according to fsl_csi.h
    uint16_t img_row, img_col;
}

status_t EDMADVP_Init(EDMADVP_Type* _inst);
status_t EDMADVP_PrepareXfer(edmadvp_xferHandle_t* _handle, uint32_t _bufferCnt, callback_t _callback);
status_t EDMADVP_BeginXfer(EDMADVP_Type* _inst, edmadvp_xferHandle_t _handle);
void EDMADVP_VSNC_ExtIntHandler(EDMADVP_Type* _inst);
void EDMADVP_EdmaXferCallback(edma_handle_t * handle, void * param, bool transferDone, uint32_t tcds);


#endif // ! HITSIC_USE_EDMADVP

#endif // ! UTILITIES_DRV_EDMADVP_HPP_