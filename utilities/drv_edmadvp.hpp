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

#define EDMADVP_CNT (1U);

#define EDMADVP0_DAT0_GPIO		RTEPIN_BOARD_CAM_D0_GPIO
#define EDMADVP0_DAT0_PORT		RTEPIN_BOARD_CAM_D0_PORT
#define EDMADVP0_DAT0_PIN		RTEPIN_BOARD_CAM_D0_PIN

#define EDMADVP0_DMA_INST        (DMA0)
#define EDMADVP0_DMAMUX_INST        (DMA0)
#define EDMADVP0_DATA_DMA_ADDR		(((uint32_t)RTEPIN_BOARD_CAM_D0_GPIO)+0x10+RTEPIN_BOARD_CAM_D0_PIN/8)

#define kStatusGroup_EDMADVP (200U)

struct EDMADVP_Type
{
    INTC_Type* vsnc_intc;
    uint32_t vsnc_pin
    INTC_Type* pclk_intc;
    uint32_t pclk_pin;

    uint32_t dmaDataAddress;
    uint32_t dmaChannel;
    uint32_t dmauxChannel;
    uint32_t dmamuxRequestSrc;
    uint32_t dmaIrqn;
    uint32_t dmaIrqPrio;

    uint32_t interruptFlag;
    uint32_t interruptMask;

    uint32_t imgSize;
    edma_handle_t edmaHandle;
};

EDMADVP_Type __EDMADVP0 = 
{
    .vsnc_intc = RTEPIN_BOARD_CAM_VSNC_PORT,
    .vsnc_pin = RTEPIN_BOARD_CAM_VSNC_PIN,
    .pclk_intc = RTEPIN_BOARD_CAM_PCLK_PORT,
    .pclk_pin = RTEPIN_BOARD_CAM_PCLK_PIN,

    .dmaDataAddress = EDMADVP0_DATA_DMA_ADDR,
    .dmaChannel = 0U,
    .dmauxChannel = 0U,
    .dmamuxRequestSrc = 0U;
    .dmaIrqn = DMA0_DMA16_IRQn,
    .dmaIrqPrio = 2U,

    .interruptFlag = 0,
    .interruptMask = 0,

    .imgSize = 0U,
    .edmaHandle = 
    {
        .callback = NULL,
        .userData = NULL,
        .base = DMA0,
        .tcdPool = NULL,
        .channel = 0U,
        .header = 0U,
        .tail = 0U,
        .tcdUsed = 0U,
        .tcdSize = 0U,
        .flags = 0U,
    },
};

static constexpr EDMADVP_Type* EDMADVP0 = &__EDMADVP0;

inline void EDMADVP_Delay_ms(uint32_t ms)
{
	for(int i = 0; i < ms, ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

inline uint32_t EDMADVP_GetInstance(EDMADVP_Type* base)
{
    switch(base)
    {
    case EDMADVP0:
        return 0;
    default:
        assert(0);//TODO: print assert message HERE!
    }
}

#endif // ! HITSIC_USE_EDMADVP
/** ! port */

#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

// #define EDMADVP_VSNC_IRQ_ENABLE() (PORT_SetPinInterruptConfig(EDMADVP_VSNC_PORT, EDMADVP_VSNC_PIN, kPORT_InterruptFallingEdge))
// #define EDMADVP_VSNC_IRQ_DISABLE() (PORT_SetPinInterruptConfig(EDMADVP_VSNC_PORT, EDMADVP_VSNC_PIN, kPORT_InterruptOrDMADisabled))

// #define EDMADVP_PCLK_IRQ_ENABLE()	(EnableIRQ(EDMADVP_PCLK_DMA_IRQn))
// #define EDMADVP_PCLK_IRQ_DISABLE()	(DisableIRQ(EDMADVP_PCLK_DMA_IRQn))



/*! @brief Error codes for the CSI driver. */
enum
{
    kStatus_EDMADVP_NoEmptyBuffer = MAKE_STATUS(kStatusGroup_EDMADVP, 0), /*!< No empty frame buffer in queue to load to CSI. */
    kStatus_EDMADVP_NoFullBuffer  = MAKE_STATUS(kStatusGroup_EDMADVP, 1), /*!< No full frame buffer in queue to read out. */
    kStatus_EDMADVP_QueueFull = MAKE_STATUS(kStatusGroup_EDMADVP, 2), /*!< Queue is full, no room to save new empty buffer. */
    kStatus_EDMADVP_FrameDone = MAKE_STATUS(kStatusGroup_EDMADVP, 3), /*!< New frame received and saved to queue. */
};

/*! @brief CSI signal polarity. */
enum _edmadvp_polarity_flags    //TODO: fix this
{
    EDMADVP_HsyncActiveLow         = 0U,                        /*!< HSYNC is active low. */
    EDMADVP_HsyncActiveHigh        = 1U,                        /*!< HSYNC is active high. */
    EDMADVP_DataLatchOnRisingEdge  = 1U << 1,                   /*!< Pixel data latched at rising edge of pixel clock. */
    EDMADVP_DataLatchOnFallingEdge = 0U,                        /*!< Pixel data latched at falling edge of pixel clock. */
    EDMADVP_VsyncActiveHigh        = 0U,                        /*!< VSYNC is active high. */
    EDMADVP_VsyncActiveLow         = 1U << 2,                   /*!< VSYNC is active low. */
};

struct edmadvp_config_t
{
    uint16_t width;           /*!< Pixels of the input frame. */
    uint16_t height;          /*!< Lines of the input frame.  */
    uint8_t bytesPerPixel;    /*!< Bytes per pixel            */
    uint16_t linePitch_Bytes; /*!< Frame buffer line pitch, must be 8-byte aligned. */
    uint32_t polarityFlags;   /*!< Timing signal polarity flags, OR'ed value of @ref _csi_polarity_flags. */
};

struct edmadvp_handle_t;

typedef void (*edmadvp_transfer_callback_t)(EDMADVP_Type *base, edmadvp_handle_t *handle, status_t status, void *userData);

struct edmadvp_handle_t
{//TODO: update this according to fsl_csi.h
    
    std::queue<uint8_t*> emptyBufferQueue;
    edma_transfer_config_t xferCfg;
}

status_t EDMADVP_Init(EDMADVP_Type *base, const edmadvp_config_t *config);

void EDMADVP_Deinit(EDMADVP_Type *base);

// void EDMADVP_EnableInterrupts(EDMADVP_Type *base, uint32_t mask);

// void EDMADVP_DisableInterrupts(EDMADVP_Type *base, uint32_t mask);

status_t EDMADVP_TransferCreateHandle(EDMADVP_Type *base, csi_handle_t *handle, csi_transfer_callback_t callback, void *userData);

status_t EDMADVP_TransferStart(EDMADVP_Type *_inst, edmadvp_handle_t _handle);

status_t EDMADVP_TransferStop(EDMADVP_Type *_inst, edmadvp_handle_t _handle);

status_t EDMADVP_TransferSubmitEmptyBuffer(EDMADVP_Type *base, edmadvp_handle_t *handle, uint32_t frameBuffer);

status_t EDMADVP_TransferGetFullBuffer(EDMADVP_Type *base, edmadvp_handle_t *handle, uint32_t *frameBuffer);

void EDMADVP_VsncExtIntHandler(EDMADVP_Type* _inst);

//void EDMADVP_EdmaXferCallback(edma_handle_t * handle, void * param, bool transferDone, uint32_t tcds);


#endif // ! HITSIC_USE_EDMADVP

#endif // ! UTILITIES_DRV_EDMADVP_HPP_